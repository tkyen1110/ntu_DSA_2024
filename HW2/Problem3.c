#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct node{
	unsigned int degree, nodes, job_id, priority;
    struct node *child;
    struct node *sibling;
};
typedef struct node Node;

struct max_node{
	struct node *current;
    struct node *prev;
    // struct node *sibling;
};
typedef struct max_node MaxNode;

Node* create_node(unsigned int job_id, unsigned int priority) {
    Node *new;
    new = (Node*)malloc(sizeof(Node));
    new -> degree = 0;
    new -> nodes = 1;
    new -> job_id = job_id;
    new -> priority = priority;
    new -> child = NULL;
    new -> sibling = NULL;
    return new;
}

MaxNode* create_max_node() {
    MaxNode *new;
    new = (MaxNode*)malloc(sizeof(MaxNode));
    new -> current = NULL;
    new -> prev = NULL;
    // new -> sibling = NULL;
    return new;
}

void print_binomial_tree(Node* tree, bool root) {
    if (tree==NULL) {
        return;
    }
    printf("%u(job_id=%u, num_nodes=%u) ", tree->priority, tree->job_id, tree->nodes);
    print_binomial_tree(tree->child, false);
    if (!root) {
        print_binomial_tree(tree->sibling, false);
    }
}

void print_binomial_heap(Node* heap) {
    while (heap!=NULL) {
        printf("%u: ", heap->degree);
        print_binomial_tree(heap, true);
        heap = heap->sibling;
        printf("\n");
    }
    printf("\n");
}

void print_binomial_heap_first_node(Node* heap) {
    while (heap!=NULL) {
        printf("%u: ", heap->degree);
        printf("%u(job_id=%u, num_nodes=%u) ", heap->priority, heap->job_id, heap->nodes);
        heap = heap->sibling;
        printf("\n");
    }
    printf("\n");
}

Node* binomial_heap_merge(Node* heap1, Node* heap2) {
    Node *head=NULL, *tail=NULL;
    if (heap1==NULL && heap2==NULL) {
        return NULL;
    } else if (heap1==NULL) {
        return heap2;
    } else if (heap2==NULL) {
        return heap1;
    }

    while (heap1!=NULL && heap2!=NULL) {
        if (heap1->degree <= heap2->degree) {
            if (head==NULL) {
                head = heap1;
                tail = heap1;
            } else {
                tail->sibling = heap1;
                tail = tail->sibling;
            }
            heap1 = heap1->sibling;
            tail->sibling = NULL;
        } else {
            if (head==NULL) {
                head = heap2;
                tail = heap2;
            } else {
                tail->sibling = heap2;
                tail = tail->sibling;
            }
            heap2 = heap2->sibling;
            tail->sibling = NULL;
        }
    }
    if (heap1!=NULL) {
        tail->sibling = heap1;
    } else if (heap2!=NULL) {
        tail->sibling = heap2;
    }
    return head;
}

Node* binomial_heap_union(Node* heap1, Node* heap2, MaxNode* heapmax, unsigned int printer_id, unsigned int op) {
    heapmax->current = NULL;
    heapmax->prev = NULL;
    Node* head = binomial_heap_merge(heap1, heap2);
    if (head == NULL) {
        return NULL;
    }
    // printf("binomial_heap_merge:\n");
    // print_binomial_heap(head);
    Node* prev = NULL;
    Node* x = head;
    Node* next = x->sibling;
    Node* child;
    unsigned int jobs = 0;
    unsigned int max_priority = 0;

    while (next != NULL) {
        if ((x->degree != next->degree) || (next->sibling != NULL && next->sibling->degree == x->degree)) {
            jobs += x->nodes;
            if (x->priority > max_priority) {
                max_priority = x->priority;
                heapmax->current = x;
                heapmax->prev = prev;
            }
            prev = x;
            x = next;
        } else {
            if (x->priority >= next->priority) {
                x->sibling = next->sibling;
                if (x->child == NULL) {
                    x->child = next;
                    next->sibling = NULL;
                } else {
                    child = x->child;
                    x->child = next;
                    next->sibling = child;
                }
                x->degree++;
                x->nodes = x->nodes + next->nodes;
            } else {
                if (prev == NULL) {
                    head = next;
                } else {
                    prev->sibling = next;
                }
                if (next->child == NULL) {
                    next->child = x;
                    x->sibling = NULL;
                } else {
                    child = next->child;
                    next->child = x;
                    x->sibling = child;
                }
                next->degree++;
                next->nodes = next->nodes + x->nodes;
                x = next;
            }
        }
        next = x->sibling;
    }
    jobs += x->nodes;
    if (x->priority > max_priority) {
        max_priority = x->priority;
        heapmax->current = x;
        heapmax->prev = prev;
    }

    switch(op) { 
        case 1:
            // Add Operation
            printf("%u jobs waiting on printer %u\n", jobs, printer_id);
            break;
        case 2:
            // Print Operation
            break;
        case 3:
            // Move Operation
            printf("%u jobs waiting on printer %u after moving\n", jobs, printer_id);
            break;
        default: 
            break;
    }
    // printf("Max priority %u waiting on printer %u\n", heapmax->current->priority, printer_id);
    return head;
}

void binomial_heap_insert(Node** printer, MaxNode** printer_max, unsigned int job_id, unsigned int priority, unsigned int printer_id) {
    Node* new_heap = create_node(job_id, priority);
    printer[printer_id] = binomial_heap_union(printer[printer_id], new_heap, printer_max[printer_id], printer_id, 1);
}

Node* reverse_linked_list(Node* head) {
    Node* prev = NULL;
    Node* current = head;
    Node* next;
    while (current != NULL) {
        next = current->sibling;
        current->sibling = prev;
        prev = current;
        current = next;
    }
    return prev;
}

void binomial_heap_extract_max(Node** printer, MaxNode** printer_max, unsigned int printer_id) {
    Node* current = printer_max[printer_id]->current;
    Node* prev = printer_max[printer_id]->prev;
    Node* new_heap;
    if (current == NULL) {
        printf("no documents in queue\n");
    } else {
        printf("%u printed\n", current->job_id);
        if (prev == NULL) {
            printer[printer_id] = current->sibling;
        } else {
            prev->sibling = current->sibling;
        }
        current->sibling = NULL;
        new_heap = reverse_linked_list(current->child);
        current->child = NULL;
        free(current);
        printer[printer_id] = binomial_heap_union(printer[printer_id], new_heap, printer_max[printer_id], printer_id, 2);
    }
}

void binomial_heap_move(Node** printer, MaxNode** printer_max, unsigned int printer_id1, unsigned int printer_id2) {
    printer[printer_id2] = binomial_heap_union(printer[printer_id1], printer[printer_id2], printer_max[printer_id2], printer_id2, 3);
    printer[printer_id1] = NULL;
    printer_max[printer_id1]->current = NULL;
    printer_max[printer_id1]->prev = NULL;
}

int main() {
    unsigned int N, M, op, job_id, priority, printer_id, printer_id1, printer_id2;
    scanf("%u%u", &N, &M);
    // printf("%u %u\n", N, M);

    // Dynamically allocate memory for the array of pointers
    Node** printer = malloc((N + 1) * sizeof(Node*));
    MaxNode** printer_max = malloc((N + 1) * sizeof(MaxNode*));

    // Initialize all elements of the array to NULL
    for (unsigned int i=0; i<=N; i++) {
        printer[i] = NULL;
        printer_max[i] = create_max_node();
    }

    for (unsigned int i=1; i<=M; i++) {
        scanf("%u", &op);

        // printf("= = = = = = = = = = %u = = = = = = = = = =\n", i+1);
        switch(op) { 
            case 1:
                // Add Operation
                scanf("%u%u%u", &job_id, &priority, &printer_id);
                // printf("%u %u %u %u\n", op, job_id, priority, printer_id);
                binomial_heap_insert(printer, printer_max, job_id, priority, printer_id);
                // printf("Printer %u:\n", printer_id);
                // print_binomial_heap(printer[printer_id]);
                break;
            case 2:
                // Print Operation
                scanf("%u", &printer_id);
                // printf("%u %u\n", op, printer_id);
                if (printer_max[printer_id]->current != NULL) {
                    printf("Max priority %u waiting on printer %u\n", printer_max[printer_id]->current->priority, printer_id);
                }
                // print_binomial_heap(printer[printer_id]);
                binomial_heap_extract_max(printer, printer_max, printer_id);
                // printf("Printer %u:\n", printer_id);
                // print_binomial_heap(printer[printer_id]);
                break;
            case 3:
                // Move Operation
                scanf("%u%u", &printer_id1, &printer_id2);
                // printf("%u %u %u\n", op, printer_id1, printer_id2);
                binomial_heap_move(printer, printer_max, printer_id1, printer_id2);
                // printf("Printer %u:\n", printer_id1);
                // print_binomial_heap(printer[printer_id1]);
                // printf("\n");
                // printf("Printer %u:\n", printer_id2);
                // print_binomial_heap(printer[printer_id2]);
                break;
            default: 
                break;
        }
    }
    return 0;
}