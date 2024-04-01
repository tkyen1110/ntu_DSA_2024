#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define max(a, b) ((a > b) ? a : b)

struct node{
    unsigned int num;
    unsigned int length;
    unsigned int guess;
    bool treasure_exist;
	long long treasure;
    struct node *child;
    struct node *parent;
    struct node *sibling;
    struct node *siblingtail;
};
typedef struct node Node;

Node* create_node(unsigned int num) {
    Node *new;
    new = (Node*)malloc(sizeof(Node));
    new -> num = num;
    new -> length = 0;
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


int main() {
    unsigned int N, M, Q, op, num;
    unsigned int ui, vi, li;
    long long ti, pi, treasure, treasure2;
    bool first_negative;
    scanf("%u%u%u", &N, &M, &Q);

    Node* tmp;
    // Dynamically allocate memory for the array of pointers
    Node** dungeon = malloc((N) * sizeof(Node*));

    // Initialize all elements of the array to NULL
    for (unsigned int i=0; i<N; i++) {
        dungeon[i] = create_node(i);
    }

    for (unsigned int i=1; i<=M; i++) {
        scanf("%u%u%u", &ui, &vi, &li);
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

        tmp = dungeon[vi];
        while (tmp->parent != NULL) {
            tmp->parent->guess = max(tmp->parent->guess, tmp->guess + tmp->length);
            tmp = tmp->parent;
        }
    }

    Node* current = dungeon[0];

    for (unsigned int i=1; i<=Q; i++) {
        scanf("%u", &op);

        switch(op) { 
            case 1:
                // Downstream
                if (current->child != NULL) {
                    current = current->child;
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
                    tmp = current->sibling;
                    if (current->parent->guess == current->guess + current->length) {
                        current->parent->guess = 0;
                        while (tmp != NULL) {
                            current->parent->guess = max(current->parent->guess, tmp->guess + tmp->length);
                            tmp = tmp->sibling;
                        }
                    }
                    current->parent->child = current->sibling;
                    current = current->parent;
                    printf("%u\n", current->num);
                }
                break;
            case 3:
                // Plan
                scanf("%lld", &ti);
                tmp = current;
                while (tmp->parent != NULL && ti >= tmp->length) {
                    ti = ti - tmp->length;
                    tmp = tmp->parent;
                }
                printf("%u\n", tmp->num);
                break;
            case 4:
                // Guess
                printf("%u\n", current->guess);
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