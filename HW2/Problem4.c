#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define max(a, b) ((a > b) ? a : b)

struct node{
    unsigned int num;
    unsigned int length;
    unsigned long long acc_length;
    unsigned long long guess;
    bool treasure_exist;
	long long treasure;
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
    new -> length = 0;
    new -> acc_length = 0;
    new -> guess = 0;
    new -> treasure_exist = false;
    new -> child = NULL;
    new -> parent = NULL;
    new -> sibling = NULL;
    new -> siblingtail = NULL;
    return new;
}

void print_node(Node* head) {
    Node* sibling = NULL;
    while (head != NULL) {
        printf("%u ", head->num);

        sibling = head->sibling;
        while (sibling != NULL) {
            printf("%u ", sibling->num);
            sibling = sibling->sibling;
        }
        printf("\n");
        head = head->child;
    }
}

// void BFS(Node* root) {
//     if (root == NULL) {
//         return;
//     }
//     root->level = root->parent->level + 1;
//     root->acc_length = (unsigned long long)root->parent->acc_length + root->length;

//     root->path = malloc((root->level+1) * sizeof(Node*));
//     root->path[0] = root;
//     if (root->level == 1) {
//         root->path[1] = root->parent;
//     } else if (root->level > 1) {
//         memcpy(root->path+1, root->parent->path, sizeof(root->parent->path)*(root->parent->level+1));
//     }
//     // printf("num = %u / level = %u / length = %u / acc_length = %llu\n", root->num, root->level, root->length, root->acc_length);
//     // for (unsigned int i=0; i<=root->level; i++) {
//     //     printf("%llu ", root->path[i]->acc_length);
//     // }
//     // printf("\n");
//     BFS(root->sibling);
//     BFS(root->child);
// }

// void BFS_print(Node* root) {
//     if (root == NULL) {
//         return;
//     }
//     printf("num = %u / level = %u / length = %u / acc_length = %llu\n", root->num, root->level, root->length, root->acc_length);
//     for (unsigned int i=0; i<=root->level; i++) {
//         printf("%llu ", root->path[i]->acc_length);
//     }
//     printf("\n");
//     BFS_print(root->sibling);
//     BFS_print(root->child);
// }

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

int main() {
    unsigned int N, M, Q, op, num;
    unsigned int ui, vi, li;

    unsigned int low, high, median;
    long long ti, pi, path, treasure, treasure2;
    unsigned long long current_guess, parent_guess;

    bool first_negative;
    scanf("%u%u%u", &N, &M, &Q);

    // Dynamically allocate memory for the array of pointers
    Node** dungeon = malloc((N) * sizeof(Node*));

    // Initialize all elements of the array to NULL
    // for (unsigned int i=0; i<N; i++) {
    //     dungeon[i] = create_node(i);
    // }

    Node* tmp;
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
        } else if (dungeon[ui]->child->siblingtail == NULL) {
            dungeon[ui]->child->sibling = dungeon[vi];
            dungeon[ui]->child->siblingtail = dungeon[vi];
        } else {
            dungeon[ui]->child->siblingtail->sibling = dungeon[vi];
            dungeon[ui]->child->siblingtail = dungeon[ui]->child->siblingtail->sibling;
        }
        dungeon[vi]->parent = dungeon[ui];
        dungeon[vi]->length = li;

        // For op = 4
        tmp = dungeon[vi];
        while (tmp->parent != NULL) {
            tmp->parent->guess = max(tmp->parent->guess, (unsigned long long)tmp->guess + tmp->length);
            tmp = tmp->parent;
        }
    }

    Node* current = dungeon[0];
    // For op = 3
    Node** plan_stack = malloc((N) * sizeof(Node*));
    unsigned int plan_idx = 0;
    stack_push(plan_stack, &plan_idx, current);

    for (unsigned int i=1; i<=Q; i++) {
        scanf("%u", &op);

        switch(op) { 
            case 1:
                // Downstream
                if (current->child != NULL) {
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
                    // For op = 4
                    tmp = current;
                    current_guess = tmp->guess;
                    parent_guess = tmp->parent->guess;
                    while (parent_guess == current_guess + tmp->length) {
                        tmp->parent->guess = 0;
                        while (tmp->sibling != NULL) {
                            tmp = tmp->sibling;
                            tmp->parent->guess = max(tmp->parent->guess, (unsigned long long)tmp->guess + tmp->length);
                        }
                        if (parent_guess != tmp->parent->guess) {
                            tmp = tmp->parent;
                            if (tmp->parent == NULL) {
                                break;
                            }
                            current_guess = parent_guess;
                            parent_guess = tmp->parent->guess;
                        } else {
                            break;
                        }
                    }

                    // For op = 2
                    current->parent->child = current->sibling;
                    if (current->parent->child != NULL) {
                        if (current->parent->child->sibling != NULL) {
                            current->parent->child->siblingtail = current->siblingtail;
                        } else {
                            current->parent->child->siblingtail = NULL;
                        }
                    }
                    stack_pop(plan_stack, &plan_idx);
                    // stack_print(plan_stack, plan_idx);
                    current = current->parent;
                    printf("%u\n", current->num);
                }
                break;
            case 3:
                // Plan
                scanf("%lld", &ti);

                low = 0;
                high = plan_idx-1;
                if (ti == 0) {
                    printf("%u\n", plan_stack[high]->num);
                } else if (ti >= plan_stack[plan_idx-1]->acc_length - plan_stack[low]->acc_length) {
                    printf("%u\n", plan_stack[low]->num);
                } else {
                    while (low < high) {
                        median = (low + high) >> 1;
                        path = plan_stack[plan_idx-1]->acc_length - plan_stack[median]->acc_length;
                        if (ti >= path) {
                            high = median;
                        } else {
                            low = median;
                        }
                        if (high == low + 1) {
                            if (ti >= plan_stack[plan_idx-1]->acc_length - plan_stack[high]->acc_length) {
                                printf("%u\n", plan_stack[high]->num);
                            } else {
                                printf("%u\n", plan_stack[low]->num);
                            }
                            break;
                        }
                    }
                }
                break;
            case 4:
                // Guess
                printf("%llu\n", current->guess);
                break;
            case 5:
                // Discover
                scanf("%lld", &pi);
                first_negative = false;
                if (current->treasure_exist) {
                    tmp = current;
                    treasure = tmp->treasure;
                    tmp->treasure = pi;
                    while (tmp->parent != NULL) {
                        if (tmp->parent->treasure_exist) {
                            treasure2 = tmp->parent->treasure;
                            tmp->parent->treasure = treasure - tmp->length;
                            if (tmp->parent->treasure < 0 && first_negative == false) {
                                first_negative = true;
                                num = tmp->parent->num;
                            }
                            treasure = treasure2;
                            tmp = tmp->parent;
                            if (tmp->num == 0) {
                                if (tmp->treasure >= 0) {
                                    printf("value remaining is %lld\n", tmp->treasure);
                                } else {
                                    printf("value lost at %u\n", num);
                                }
                            }
                        } else {
                            tmp->parent->treasure_exist = true;
                            tmp->parent->treasure = treasure - tmp->length;
                            if (tmp->parent->treasure < 0 && first_negative == false) {
                                first_negative = true;
                                num = tmp->parent->num;
                            }
                            if (tmp->parent->num == 0) {
                                if (tmp->parent->treasure >= 0) {
                                    printf("value remaining is %lld\n", tmp->parent->treasure);
                                } else {
                                    printf("value lost at %u\n", num);
                                }
                                
                            }
                            break;
                        }
                    }
                } else {
                    current->treasure_exist = true;
                    current->treasure = pi;
                }
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