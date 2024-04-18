#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct guess_node{
    unsigned long long guess;
    unsigned int child_id;
    struct guess_node* prev;
    struct guess_node* next;
};
typedef struct guess_node GuessNode;

GuessNode* create_guess_node(unsigned long long guess, unsigned int child_id) {
    GuessNode *new;
    new = (GuessNode*)malloc(sizeof(GuessNode));
    new -> guess = guess;
    new -> child_id = child_id;
    new -> prev = NULL;
    new -> next = NULL;
    return new;
}

struct node{
    unsigned int num;
    unsigned int level;
    unsigned int child_id;
    unsigned int length;
    unsigned long long acc_length;
    GuessNode* guess_head;
    GuessNode* guess_tail;
    struct node* child;
    struct node* parent;
    struct node* sibling;
    struct node* siblingtail;
};
typedef struct node Node;

Node* create_node(unsigned int num) {
    Node *new;
    new = (Node*)malloc(sizeof(Node));
    new -> num = num;
    new -> level = 0;
    new -> child_id = 0;
    new -> length = 0;
    new -> acc_length = 0;
    new -> guess_head = NULL;
    new -> guess_tail = NULL;
    new -> child = NULL;
    new -> parent = NULL;
    new -> sibling = NULL;
    new -> siblingtail = NULL;
    return new;
}

void print_guess_node(Node* node) {
    GuessNode* head = node->guess_head;
    printf("[node = %3u]   ", node->num);
    while (head != NULL) {
        printf("(guess = %llu,  child_id = %u) -> ", head->guess, head->child_id);
        head = head->next;
    }
    printf("\n");
}

void print_node(Node* root) {
    Node* sibling = NULL;
    while (root != NULL) {
        printf("%u ", root->num);

        sibling = root->sibling;
        while (sibling != NULL) {
            printf("%u ", sibling->num);
            sibling = sibling->sibling;
        }
        printf("\n");
        root = root->child;
    }
}

struct discover_head_node{
    unsigned int level_start;
    unsigned int level_end;
    struct discover_node* head;
    struct discover_node* tail;
    struct discover_head_node* up;
    struct discover_head_node* down;
};
typedef struct discover_head_node DiscoverHeadNode;

struct discover_node{
    Node* dungeon;
    unsigned int first_negative_dungeon;
    unsigned long long escorted_treasure;
    bool escorted_negative;
    struct discover_node* prev;
    struct discover_node* next;
};
typedef struct discover_node DiscoverNode;

DiscoverHeadNode* create_discover_head_node(unsigned int level_start, unsigned int level_end) {
    DiscoverHeadNode* new;
    new = (DiscoverHeadNode*)malloc(sizeof(DiscoverHeadNode));
    new -> level_start = level_start;
    new -> level_end = level_end;
    new -> head = NULL;
    new -> tail = NULL;
    new -> up = NULL;
    new -> down = NULL;
    return new;
}

DiscoverNode* create_discover_node(Node* dungeon, unsigned int first_negative_dungeon, unsigned long long escorted_treasure, bool escorted_negative) {
    DiscoverNode *new;
    new = (DiscoverNode*)malloc(sizeof(DiscoverNode));
    new -> dungeon = dungeon;
    new -> first_negative_dungeon = first_negative_dungeon;
    new -> escorted_treasure = escorted_treasure;
    new -> escorted_negative = escorted_negative;
    new -> prev = NULL;
    new -> next = NULL;
    return new;
}

void print_discover_node(DiscoverHeadNode* discover_head) {
    while (discover_head != NULL) {
        printf("(%u, %u): ", discover_head->level_start, discover_head->level_end);
        DiscoverNode* head = discover_head->head;
        while (head != NULL) {
            printf("%u(first_negative_dungeon=%u; escorted_treasure=%llu; escorted_negative=%d) -> ", 
            head->dungeon->num, head->first_negative_dungeon, head->escorted_treasure, head->escorted_negative);
            head = head->next;
        }
        printf("\n");
        discover_head = discover_head->down;
        printf("%p\n", discover_head);
    }
}

void stack_push(Node** stack, unsigned int* idx, Node* current) {
    if (*idx >= 1) {
        current->acc_length = (unsigned long long)stack[(*idx)-1]->acc_length + current->length;
    }

    stack[*idx] = current;
    (*idx)++;
}

void stack_pop(Node** stack, unsigned int* idx) {
    if (*idx >= 1) {
        (*idx)--;
        stack[*idx] = NULL;
    }
}

void stack_print(Node** stack, unsigned int idx) {
    if (idx==0) {
        printf("Stack is empty\n");
        return;
    }
    printf("\nStack size = %u / Stack = ", idx);
    for (unsigned int i=0; i<idx; i++) {
        printf("%llu ", stack[i]->acc_length);
    }
    printf("\n");
}

void dequeue_push_back(GuessNode** head, GuessNode** tail, unsigned long long guess, unsigned int child_id) {
    if ((*tail)==NULL) {
        *head = create_guess_node(guess, child_id);
        *tail = *head;
    } else {
        (*tail)->next = create_guess_node(guess, child_id);
        (*tail)->next->prev = *tail;
        *tail = (*tail)->next;
    }
}

void dequeue_pop_back(GuessNode** head, GuessNode** tail) {
    GuessNode* tmp;
    if ((*tail)!=NULL) {
        tmp = *tail;
        if ((*tail)->prev == NULL) {
            *head = NULL;
            *tail = NULL;
        } else {
            (*tail)->prev->next = NULL;
            *tail = (*tail)->prev;
        }
        free(tmp);
    }
}

void dequeue_push_front(GuessNode** head, GuessNode** tail, unsigned long long guess, unsigned int child_id) {
    if ((*head)==NULL) {
        *head = create_guess_node(guess, child_id);
        *tail = *head;
    } else {
        (*head)->prev = create_guess_node(guess, child_id);
        (*head)->prev->next = *head;
        *head = (*head)->prev;
    }
}

void dequeue_pop_front(GuessNode** head, GuessNode** tail) {
    GuessNode* tmp;
    if ((*head)!=NULL) {
        tmp = *head;
        if ((*head)->next == NULL) {
            *head = NULL;
            *tail = NULL;
        } else {
            (*head)->next->prev = NULL;
            *head = (*head)->next;
        }
        free(tmp);
    }
}

void dequeue_delete_middle(GuessNode** head, GuessNode** middle, GuessNode** tail) {
    GuessNode* tmp;
    if (*middle == *head) {
        dequeue_pop_front(head, tail);
        *middle = *head;
    } else {
        (*middle)->prev->next = (*middle)->next;
        if ((*middle)->next != NULL) {
            (*middle)->next->prev = (*middle)->prev;
        } else {
            *tail = (*middle)->prev;
        }
        tmp = *middle;
        *middle = (*middle)->next;
        free(tmp);
    }
}

void monotonic_queue_insert(Node** dungeon, unsigned long long guess, unsigned int child_id, bool check_parent) {
    Node* current = *dungeon;
    GuessNode *guess_head, *guess_tail, *guess_midd, *guess_tmp;
    unsigned long long parent_guess;

    while (current->guess_tail != NULL && current->guess_tail->guess <= guess) {
        dequeue_pop_back(&(current->guess_head), &(current->guess_tail));
    }
    dequeue_push_back(&(current->guess_head), &(current->guess_tail), guess, child_id);
    // print_guess_node(current);

    while (check_parent && current->parent != NULL) {
        guess_head = current->parent->guess_head;
        guess_tail = current->parent->guess_tail;
        guess_midd = guess_head;
        parent_guess = current->guess_head->guess + current->length;

        while (guess_midd != NULL) {
            if (guess_midd->child_id < current->child_id) {
                if (guess_midd->guess <= parent_guess) {
                    dequeue_delete_middle(&guess_head, &guess_midd, &guess_tail);
                } else {
                    guess_midd = guess_midd->next;
                }
            } else if (guess_midd->child_id == current->child_id) {
                if (guess_midd->guess < parent_guess) {
                    guess_midd->guess = parent_guess;
                }
                if (guess_midd->next != NULL) {
                    if (guess_midd->next->guess < parent_guess) {
                        break;
                    } else {
                        dequeue_delete_middle(&guess_head, &guess_midd, &guess_tail);
                    }
                } else {
                    guess_midd = guess_midd->next;
                }
            } else {
                if (guess_midd->guess >= parent_guess) {
                    break;
                } else {
                    guess_tmp = create_guess_node(parent_guess, current->child_id);
                    guess_tmp->next = guess_midd;
                    if (guess_midd->prev == NULL) {
                        guess_head = guess_tmp;
                        guess_midd->prev = guess_tmp;
                    } else {
                        guess_midd->prev->next = guess_tmp;
                        guess_tmp->prev = guess_midd->prev;
                        guess_tmp->next = guess_midd;
                        guess_midd->prev = guess_tmp;
                    }
                    break;
                }
            }
        }

        current->parent->guess_head = guess_head;
        current->parent->guess_tail = guess_tail;

        // print_guess_node(current->parent);
        current = current->parent;
    }
}

void dfs_monotonic_queue_insert(Node* root) {
    if (root == NULL) {
        return;
    }

    dfs_monotonic_queue_insert(root->child);

    if (root->parent != NULL) {
        if (root->guess_head == NULL) {
            monotonic_queue_insert(&(root->parent), (unsigned long long)root->length, root->child_id, false);
            // printf("num = %u / length = %u / acc_length = %llu / guess = 0\n", root->num, root->length, root->acc_length);
        } else {
            monotonic_queue_insert(&(root->parent), (unsigned long long)root->guess_head->guess + root->length, root->child_id, false);
            // printf("num = %u / length = %u / acc_length = %llu / guess = %llu\n", root->num, root->length, root->acc_length, root->guess_head->guess);
        }
    } else {
        ;
        // printf("num = %u / length = %u / acc_length = %llu / guess = %llu\n", root->num, root->length, root->acc_length, root->guess_head->guess);
    }
    dfs_monotonic_queue_insert(root->sibling);
}

unsigned int find_first_negative_dungeon(unsigned int plan_idx, Node** plan_stack, unsigned long long treasure) {
    unsigned int low = 0;
    unsigned int high = plan_idx-1;
    unsigned int middle;

    while (low < high) {
        middle = (low + high) >> 1;
        if (treasure >= plan_stack[plan_idx-1]->acc_length - plan_stack[middle]->acc_length) {
            high = middle;
        } else {
            low = middle;
        }
        if (high == low + 1) {
            return plan_stack[low]->num;
        }
    }
}

int main() {
    unsigned int N, M, Q, op;
    unsigned int ui, vi, li;

    scanf("%u%u%u", &N, &M, &Q);

    // Dynamically allocate memory for the array of pointers
    Node** dungeon = malloc((N) * sizeof(Node*));

    // Initialize all elements of the array to NULL
    for (unsigned int i=0; i<N; i++) {
        dungeon[i] = NULL;
    }

    for (unsigned int i=1; i<=M; i++) {
        scanf("%u%u%u", &ui, &vi, &li);
        if (dungeon[ui]==NULL) {
            dungeon[ui] = create_node(ui);
        }
        if (dungeon[vi]==NULL) {
            dungeon[vi] = create_node(vi);
        }

        if (dungeon[ui]->child == NULL) {
            dungeon[ui]->child = dungeon[vi];
            dungeon[vi]->child_id = 1;
        } else if (dungeon[ui]->child->siblingtail == NULL) {
            dungeon[ui]->child->sibling = dungeon[vi];
            dungeon[ui]->child->siblingtail = dungeon[vi];
            dungeon[vi]->child_id = dungeon[ui]->child->child_id + 1;
        } else {
            dungeon[ui]->child->siblingtail->sibling = dungeon[vi];
            dungeon[vi]->child_id = dungeon[ui]->child->siblingtail->child_id + 1;
            dungeon[ui]->child->siblingtail = dungeon[ui]->child->siblingtail->sibling;
        }
        dungeon[vi]->parent = dungeon[ui];
        dungeon[vi]->length = li;

        // For op = 4
        // Monotonic Queue (monotonic_queue_insert when building the tree)
        // if (dungeon[vi]->guess_head == NULL) {
        //     monotonic_queue_insert(&dungeon[ui], (unsigned long long)dungeon[vi]->length, dungeon[vi]->child_id, true);
        // } else {
        //     monotonic_queue_insert(&dungeon[ui], (unsigned long long)dungeon[vi]->guess_head->guess + dungeon[vi]->length, dungeon[vi]->child_id, true);
        // }
    }

    // For op = 4
    // Monotonic Queue (dfs_monotonic_queue_insert after the tree is built)
    dfs_monotonic_queue_insert(dungeon[0]);

    Node* current = dungeon[0];
    Node* parent;

    // For op = 3
    unsigned long long ti;
    unsigned int low, high, middle;
    Node** plan_stack = malloc((N) * sizeof(Node*));
    unsigned int plan_idx = 0;
    stack_push(plan_stack, &plan_idx, current);

    // For op = 4
    GuessNode *guess_tmp;

    // For op = 5
    unsigned long long pi;
    unsigned int first_negative_dungeon;
    unsigned long long escorted_treasure;
    bool escorted_negative;
    DiscoverHeadNode* discover_head = NULL;
    DiscoverHeadNode* discover_head_tmp;
    DiscoverNode* discover_tmp;

    for (unsigned int i=1; i<=Q; i++) {
        scanf("%u", &op);

        switch(op) { 
            case 1:
                // Downstream
                if (current->child != NULL) {
                    current->child->level = current->level + 1;
                    current = current->child;
                    stack_push(plan_stack, &plan_idx, current);
                    // stack_print(plan_stack, plan_idx);
                    printf("%u\n", current->num);
                } else {
                    printf("-1\n");
                }
                break;
            case 2:
                // Upstream
                if (current->parent == NULL) {
                    printf("-1\n");
                } else {
                    // For op = 3
                    stack_pop(plan_stack, &plan_idx);
                    // stack_print(plan_stack, plan_idx);

                    // For op = 4
                    parent = current->parent;
                    if (parent->guess_head->child_id == current->child_id) {
                        guess_tmp = parent->guess_head;
                        parent->guess_head = parent->guess_head->next;
                        if (parent->guess_head==NULL) {
                            parent->guess_tail==NULL;
                        } else {
                            parent->guess_head->prev = NULL;
                        }
                        free(guess_tmp);
                    }

                    // For op = 5
                    if (discover_head != NULL && discover_head->level_end == current->level) {
                        if (discover_head->level_start == discover_head->level_end) {
                            free(discover_head->head);
                            discover_head_tmp = discover_head;
                            discover_head = discover_head->up;
                            if (discover_head != NULL) {
                                discover_head->down = NULL;
                            }
                            free(discover_head_tmp);
                        } else {
                            discover_head->level_end = discover_head->level_end - 1;
                            discover_tmp = discover_head->tail;
                            discover_head->tail = discover_head->tail->prev;
                            discover_head->tail->next = NULL;
                            free(discover_tmp);
                        }
                    }
                    // print_discover_node(discover_head);

                    // For op = 2
                    current->parent->child = current->sibling;
                    if (current->parent->child != NULL) {
                        if (current->parent->child->sibling != NULL) {
                            current->parent->child->siblingtail = current->siblingtail;
                        } else {
                            current->parent->child->siblingtail = NULL;
                        }
                    }

                    current = current->parent;
                    printf("%u\n", current->num);
                }
                break;
            case 3:
                // Plan
                scanf("%llu", &ti);

                low = 0;
                high = plan_idx-1;
                if (ti == 0) {
                    printf("%u\n", plan_stack[high]->num);
                } else if (ti >= plan_stack[plan_idx-1]->acc_length - plan_stack[low]->acc_length) {
                    printf("%u\n", plan_stack[low]->num);
                } else {
                    while (low < high) {
                        middle = (low + high) >> 1;
                        if (ti >= plan_stack[plan_idx-1]->acc_length - plan_stack[middle]->acc_length) {
                            high = middle;
                        } else {
                            low = middle;
                        }
                        if (high == low + 1) {
                            printf("%u\n", plan_stack[high]->num);
                            break;
                        }
                    }
                }
                break;
            case 4:
                // Guess
                if (current->guess_head == NULL) {
                    printf("0\n");
                } else {
                    printf("%llu\n", current->guess_head->guess);
                }
                break;
            case 5:
                // Discover
                scanf("%llu", &pi);

                if (plan_stack[plan_idx-1]->level == 0) {
                    if (pi < 0) {
                        printf("value lost at 0\n");
                    } else {
                        printf("value remaining is %llu\n", pi);
                    }
                    break;
                }

                if (pi < plan_stack[plan_idx-1]->acc_length) {
                    first_negative_dungeon = find_first_negative_dungeon(plan_idx, plan_stack, pi);
                    escorted_treasure = 0;
                    escorted_negative = true;
                } else {
                    first_negative_dungeon = 0;
                    escorted_treasure = pi - plan_stack[plan_idx-1]->acc_length;
                    escorted_negative = false;
                }

                if (discover_head==NULL) {
                    discover_head = create_discover_head_node(plan_stack[plan_idx-1]->level, plan_stack[plan_idx-1]->level);
                    discover_head->head = create_discover_node(plan_stack[plan_idx-1], first_negative_dungeon, escorted_treasure, escorted_negative);
                    discover_head->tail = discover_head->head;
                } else {
                    if (plan_stack[plan_idx-1]->level > discover_head->level_end + 1) {
                        discover_head->down = create_discover_head_node(plan_stack[plan_idx-1]->level, plan_stack[plan_idx-1]->level);
                        discover_head->down->up = discover_head;
                        discover_head = discover_head->down;
                        discover_head->head = create_discover_node(plan_stack[plan_idx-1], first_negative_dungeon, escorted_treasure, escorted_negative);
                        discover_head->tail = discover_head->head;
                    } else if (plan_stack[plan_idx-1]->level == discover_head->level_end + 1) {
                        discover_head->level_end = discover_head->level_end + 1;
                        discover_head->tail->next = create_discover_node(plan_stack[plan_idx-1], first_negative_dungeon, escorted_treasure, escorted_negative);
                        discover_head->tail->next->prev = discover_head->tail;
                        discover_head->tail = discover_head->tail->next;
                    } else if (plan_stack[plan_idx-1]->level == discover_head->level_end) {
                        if (discover_head->up != NULL) {
                            discover_head->level_start = discover_head->level_start - 1;
                            discover_head->tail->next = create_discover_node(plan_stack[plan_idx-1], first_negative_dungeon, escorted_treasure, escorted_negative);
                            discover_head->tail->next->prev = discover_head->tail;
                            discover_head->tail = discover_head->tail->next;

                            if (discover_head->level_start == discover_head->up->level_end + 1) {
                                discover_head->up->level_end = discover_head->level_end; 
                                discover_head->up->tail->next = discover_head->head;
                                discover_head->head->prev = discover_head->up->tail;
                                discover_head->up->tail = discover_head->tail;
                                discover_head_tmp = discover_head;
                                discover_head = discover_head->up;
                                discover_head->down = NULL;
                                free(discover_head_tmp);
                            }
                        } else if (discover_head->level_start > 0) {
                            discover_head->level_start = discover_head->level_start - 1;
                            discover_head->tail->next = create_discover_node(plan_stack[plan_idx-1], first_negative_dungeon, escorted_treasure, escorted_negative);
                            discover_head->tail->next->prev = discover_head->tail;
                            discover_head->tail = discover_head->tail->next;
                            // print_discover_node(discover_head);
                            if (discover_head->level_start == 0) {
                                if (discover_head->head->escorted_negative) {
                                    printf("value lost at %u\n", discover_head->head->first_negative_dungeon);
                                } else {
                                    printf("value remaining is %llu\n", discover_head->head->escorted_treasure);
                                }
                                discover_head->level_start = discover_head->level_start + 1;
                                discover_tmp = discover_head->head;
                                discover_head->head = discover_head->head->next;
                                free(discover_tmp);

                                if (discover_head->head == NULL) {
                                    discover_head->tail = NULL;
                                    free(discover_head);
                                    discover_head = NULL;
                                } else {
                                    discover_head->head->prev = NULL;
                                }
                            }
                        } else {
                            ;
                        }
                    }
                }
                // print_discover_node(discover_head);
                break;
            case 6:
                // Construct
                break;
            default: 
                break;
        }
    }
    return 0;
}