#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct node{
	unsigned long long value;
    int level;
    struct node *up;
    struct node *down;
    struct node *prev;
	struct node *next;
};
typedef struct node Node;

Node* create_node(unsigned long long value, int level) {
    Node *new;
    new = (Node*)malloc(sizeof(Node));
    new -> value = value;
    new -> level = level;
    new -> up = NULL;
    new -> down = NULL;
    new -> prev = NULL;
    new -> next = NULL;
    return new;
}

void print_node(Node* tail) {
    while (tail!=NULL) {
        printf("level = %3d ", tail->level);
        for(Node* head=tail->next; head!=NULL ; head=head->next) {
            printf("%llu ", head->value);
        }
        printf("\n");
        tail = tail->down;
    }
}

void slowget(Node* head, unsigned long long data) {
    if (head==NULL) {
        printf("%d\n", -1);
        return;
    }
    while (head->next != NULL && data <= head->next->value) {
        head = head->next;
        printf("%llu ", head->value);
    }
    if (head->prev==NULL) {
        printf("%d", -1);
    }
    printf("\n");
}

void fastget(Node* tail, unsigned long long data, Node** prev_nodes, bool record_prev_nodes) {
    bool used = false;
    if (tail==NULL) {
        printf("%d\n", -1);
        return;
    }
    while (tail->down != NULL) {
        while (tail->next != NULL && data <= tail->next->value) {
            tail = tail->next;
            if (!record_prev_nodes) {
                printf("%llu ", tail->value);
                used = true;
            }
        }
        if (record_prev_nodes) {
            prev_nodes[tail->level] = tail;
        }
        tail = tail->down;
        if (!record_prev_nodes && tail->prev!=NULL) {
            printf("%llu ", tail->value);
            used = true;
        }
    }

    while (tail->next != NULL && data <= tail->next->value) {
        tail = tail->next;
        if (!record_prev_nodes) {
            printf("%llu ", tail->value);
            used = true;
        }
    }
    if (!record_prev_nodes) {
        if (used==false) {
            printf("%d", -1);
        }
        printf("\n");
    }
    if (record_prev_nodes) {
        prev_nodes[tail->level] = tail;
    }
}

void insert(Node** head, Node** tail, unsigned long long data) {
    Node* new;
    int level;
    bool odd=false;
    unsigned long long num;

    if (*head==NULL || *tail==NULL) {
        new = create_node(0, 0);
        *head = new;
        *tail = new;
        (*head)->next = create_node(data, 0);
        (*head)->next->prev = *head;
        do {
            level = (*tail)->level;
            odd=false;
            for (Node* tmp=(*tail)->next; tmp!=NULL; tmp=tmp->next) {
                num = (tmp->value)>>(level);
                if (num%2 == 1) {
                    if (odd==false) {
                        odd=true;
                        new = create_node(0, level+1);
                        (*tail)->up = new;
                        new->down = *tail;
                        *tail = new;
                    }
                    new->next = create_node(tmp->value, level+1);
                    new->next->prev = new;
                    new->next->down = tmp;
                    tmp->up = new->next;
                    new = new->next;
                }
            }
        } while(odd && num!=0 && num!=1);
    } else {
        level = (*tail)->level;
        Node* prev_nodes[level+1];
        fastget(*tail, data, prev_nodes, true);

        int top_level = 0;
        for (int i=0; i<=level; i++) {
            if (i > 0) {
                num = data>>(i-1);
                if (num%2 == 1) {
                    top_level = i;
                    new = create_node(data, i);
                    new->next = prev_nodes[i]->next;
                    if (new->next!=NULL) {
                        new->next->prev = new;
                    }
                    prev_nodes[i]->next = new;
                    new->prev = prev_nodes[i];
                    prev_nodes[i] = new;
                } else{
                    break;
                }
            } else {
                new = create_node(data, i);
                new->next = prev_nodes[i]->next;
                if (new->next!=NULL) {
                    new->next->prev = new;
                }
                prev_nodes[i]->next = new;
                new->prev = prev_nodes[i];
                prev_nodes[i] = new;
            }
        }

        if (top_level > 0) {
            for (int i=top_level; i>=0; i--) {
                if (i<top_level) {
                    prev_nodes[i]->up = prev_nodes[i+1];
                }
                if (i>0) {
                    prev_nodes[i]->down = prev_nodes[i-1];
                }
            }
        }

        if (top_level==level) {
            do {
                level = (*tail)->level;
                odd=false;
                for (Node* tmp=(*tail)->next; tmp!=NULL; tmp=tmp->next) {
                    num = (tmp->value)>>(level);
                    if (num%2 == 1) {
                        if (odd==false) {
                            odd=true;
                            new = create_node(0, level+1);
                            (*tail)->up = new;
                            new->down = *tail;
                            *tail = new;
                        }
                        new->next = create_node(tmp->value, level+1);
                        new->next->prev = new;
                        new->next->down = tmp;
                        tmp->up = new->next;
                        new = new->next;
                    }
                }
            } while(odd && num!=0 && num!=1);
        }
    }
    // printf("\n");
    // print_node(*tail);
    // printf("\n");
}

void delete(Node** head, Node** tail, unsigned long long data) {
    if (*tail==NULL) {
        return;
    }
    int level;
    level = (*tail)->level;
    Node* ptr = *tail;
    Node* down;
    Node* prev_nodes[level+1];
    fastget(*tail, data, prev_nodes, true);

    for (int i=level; i>=0; i--) {
        if (prev_nodes[i]->prev!=NULL && prev_nodes[i]->value==data) {
            prev_nodes[i]->prev->next = prev_nodes[i]->next;
            if (prev_nodes[i]->next!=NULL) {
                prev_nodes[i]->next->prev = prev_nodes[i]->prev;
            }
            free(prev_nodes[i]);
        }
        down = ptr->down;
        if (ptr->next==NULL) {
            free(ptr);
            *tail = down;
            if (*tail!=NULL) {
                (*tail)->up = NULL;
            }
        }
        ptr = down;
    }
    if (*tail==NULL) {
        *head=NULL;
    }
    // printf("\n");
    // print_node(*tail);
    // printf("\n");
}

int main() {
    unsigned int M, t;
    unsigned long long k;
    scanf("%u", &M);

    Node *head=NULL, *tail=NULL;
    for (unsigned int i=1; i<=M; i++) {
        scanf("%u%llu", &t, &k);

        switch(t) { 
            case 1: 
                slowget(head, k);
                break; 
            case 2: 
                fastget(tail, k, NULL, false);
                break; 
            case 3:
                insert(&head, &tail, k);
                break; 
            case 4: 
                delete(&head, &tail, k);
                break; 
            default: 
                break;
        }

    }

    return 0;
}