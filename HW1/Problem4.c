#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// struct node{
// 	unsigned int label;
//     unsigned int power;
//     int level;
//     struct node *up;
//     struct node *down;
//     struct node *prev;
// 	struct node *next;
// };
// typedef struct node Node;

// Node* create_node(unsigned int label, unsigned int power, int level) {
//     Node *new;
//     new = (Node*)malloc(sizeof(Node));
//     new -> label = label;
//     new -> power = power;
//     new -> level = level;
//     new -> up = NULL;
//     new -> down = NULL;
//     new -> prev = NULL;
//     new -> next = NULL;
//     return new;
// }

// struct attack_gain_node{
// 	unsigned int attack_gain;
//     struct attack_gain_node *prev;
//     struct attack_gain_node *next;
// };
// typedef struct attack_gain_node AttackGainNode;

// AttackGainNode* create_attack_gain_node(unsigned int attack_gain) {
//     AttackGainNode *new;
//     new = (AttackGainNode*)malloc(sizeof(AttackGainNode));
//     new -> attack_gain = attack_gain;
//     new -> prev = NULL;
//     new -> next = NULL;
//     return new;
// }

// void print_node(Node* tail) {
//     while (tail!=NULL) {
//         printf("level = %3d ", tail->level);
//         for(Node* head=tail->next; head!=NULL ; head=head->next) {
//             printf("%u(%u) ", head->label, head->power);
//         }
//         printf("\n");
//         tail = tail->down;
//     }
// }

// // void slowget(Node* head, unsigned long long data) {
// //     if (head==NULL) {
// //         printf("%d\n", -1);
// //         return;
// //     }
// //     while (head->next != NULL && data <= head->next->value) {
// //         head = head->next;
// //         printf("%llu ", head->value);
// //     }
// //     if (head->prev==NULL) {
// //         printf("%d", -1);
// //     }
// //     printf("\n");
// // }

// void fastget(Node* tail, unsigned int power, Node** prev_nodes, bool record_prev_nodes) {
//     bool used = false;
//     if (tail==NULL) {
//         printf("%d\n", -1);
//         return;
//     }
//     while (tail->down != NULL) {
//         while (tail->next != NULL && power <= tail->next->power) {
//             tail = tail->next;
//             if (!record_prev_nodes) {
//                 printf("%u ", tail->power);
//                 used = true;
//             }
//         }
//         if (record_prev_nodes) {
//             prev_nodes[tail->level] = tail;
//         }
//         tail = tail->down;
//         if (!record_prev_nodes && tail->prev!=NULL) {
//             printf("%u ", tail->power);
//             used = true;
//         }
//     }

//     while (tail->next != NULL && power <= tail->next->power) {
//         tail = tail->next;
//         if (!record_prev_nodes) {
//             printf("%u ", tail->power);
//             used = true;
//         }
//     }
//     if (!record_prev_nodes) {
//         if (used==false) {
//             printf("%d", -1);
//         }
//         printf("\n");
//     }
//     if (record_prev_nodes) {
//         prev_nodes[tail->level] = tail;
//     }
// }

// void insert(Node** head, Node** tail, unsigned int label, unsigned int power) {
//     Node* new;
//     int level;
//     bool odd=false;
//     unsigned long long num;

//     if (*head==NULL || *tail==NULL) {
//         new = create_node(0, 0, 0);
//         *head = new;
//         *tail = new;
//         (*head)->next = create_node(label, power, 0);
//         (*head)->next->prev = *head;

//         level = (*tail)->level;
//         odd=false;
//         for (Node* tmp=(*tail)->next; tmp!=NULL; tmp=tmp->next) {
//             num = (tmp->power)>>(level);
//             if (num%2 == 1) {
//                 if (odd==false) {
//                     odd=true;
//                     new = create_node(0, 0, level+1);
//                     (*tail)->up = new;
//                     new->down = *tail;
//                     *tail = new;
//                 }
//                 new->next = create_node(tmp->label, tmp->power, level+1);
//                 new->next->prev = new;
//                 new->next->down = tmp;
//                 tmp->up = new->next;
//                 new = new->next;
//             }
//         }

//     } else {
//         level = (*tail)->level;
//         Node* prev_nodes[level+1];
//         fastget(*tail, power, prev_nodes, true);

//         int top_level = 0;
//         for (int i=0; i<=level; i++) {
//             if (i > 0) {
//                 num = power>>(i-1);
//                 if (num%2 == 1) {
//                     top_level = i;
//                     new = create_node(label, power, i);
//                     new->next = prev_nodes[i]->next;
//                     if (new->next!=NULL) {
//                         new->next->prev = new;
//                     }
//                     prev_nodes[i]->next = new;
//                     new->prev = prev_nodes[i];
//                     prev_nodes[i] = new;
//                 } else{
//                     break;
//                 }
//             } else {
//                 new = create_node(label, power, i);
//                 new->next = prev_nodes[i]->next;
//                 if (new->next!=NULL) {
//                     new->next->prev = new;
//                 }
//                 prev_nodes[i]->next = new;
//                 new->prev = prev_nodes[i];
//                 prev_nodes[i] = new;
//             }
//         }

//         if (top_level > 0) {
//             for (int i=top_level; i>=0; i--) {
//                 if (i<top_level) {
//                     prev_nodes[i]->up = prev_nodes[i+1];
//                 }
//                 if (i>0) {
//                     prev_nodes[i]->down = prev_nodes[i-1];
//                 }
//             }
//         }

//         if (top_level==level) {
//             level = (*tail)->level;
//             odd=false;
//             for (Node* tmp=(*tail)->next; tmp!=NULL; tmp=tmp->next) {
//                 num = (tmp->power)>>(level);
//                 if (num%2 == 1) {
//                     if (odd==false) {
//                         odd=true;
//                         new = create_node(0, 0, level+1);
//                         (*tail)->up = new;
//                         new->down = *tail;
//                         *tail = new;
//                     }
//                     new->next = create_node(tmp->label, tmp->power, level+1);
//                     new->next->prev = new;
//                     new->next->down = tmp;
//                     tmp->up = new->next;
//                     new = new->next;
//                 }
//             }
//         }
//     }
//     printf("\n");
//     print_node(*tail);
//     printf("\n");
// }

// void rank_attack(Node** head, Node** tail, unsigned int label, unsigned int* Power,
//                  unsigned int *Attack, AttackGainNode **AttackGain) {
//     unsigned int power = Power[label];
//     int level;
//     level = (*tail)->level;
//     Node* prev_nodes[level+1];
//     fastget(*tail, power, prev_nodes, true);

//     Node* src = prev_nodes[0];
//     while(src->label != label) {
//         src = src->prev;
//     }
//     Node* dst = src->prev;
//     if (dst->prev==NULL) {
//         return;
//     } else {
//         Attack[label]++;
//         if (AttackGain[label]==NULL){
//             AttackGain[label] = create_attack_gain_node(dst->power-src->power); 
//         } else {
//             AttackGain[label]->next = create_attack_gain_node(dst->power-src->power);
//             AttackGain[label]->next->prev = AttackGain[label];
//         }
//         src->power = dst->power;
//         dst->prev->next = src;
//         src->prev = dst->prev;
//         dst->next = src->next;
//         src->next = dst;
//         dst->prev = src;
//     }

//     // printf("%u %u\n", prev_nodes[1]->label, prev_nodes[1]->power);
//     // printf("%u %u\n", target->label, target->power);
//     printf("\n");
//     print_node(*tail);
//     printf("\n");
// }

// void reward_each(Node** head, unsigned int N, unsigned int* Power) {
//     Node* tmp = *head;
//     for (unsigned int i=1; i<=N; i++) {
//         tmp = tmp->next; 
//         tmp->power += (N-i);
//         Power[tmp->label] = tmp->power;
//     }
//     printf("\n");
//     print_node(*head);
//     printf("\n");
// }

// void query_power(Node* head, unsigned int power) {
//     unsigned int rank = 0;
//     while(head->next!=NULL) {
//         if (head->next->power>=power) {
//             head = head->next;
//             rank++;
//         } else {
//             if (head->prev==NULL) {
//                 printf("%d %d", 0, 0);
//             } else {
//                 printf("%u %u", rank, head->label);
//             }
//             break;
//         }
//     }
// }

// void query_power_increase(Node* head, unsigned int power) {

// }

// int main() {
//     unsigned int N, T, M, Pn, incident;
//     scanf("%u%u%u", &N, &T, &M);

//     unsigned int Power[N+1]; // label -> power
//     unsigned int Attack[N+1]; // label -> num of attacks
//     AttackGainNode *AttackGain[N+1]; // label -> gain of attacks
//     Node *head=NULL, *tail=NULL;
//     for (unsigned int i=1; i<=N; i++) {
//         scanf("%u", &Pn);
//         Power[i] = Pn;
//         Attack[i] = 0;
//         AttackGain[i] = NULL;
//         insert(&head, &tail, i, Pn);
//     }

//     unsigned int a, q, b, m;
//     for (unsigned int i=1; i<=T; i++) {
//         scanf("%u", &incident);
//         switch(incident) { 
//             case 1:
//                 scanf("%u", &a);
//                 rank_attack(&head, &tail, a, Power, Attack, AttackGain);
//                 break; 
//             case 2:
//                 reward_each(&head, N, Power);
//                 break; 
//             case 3:
//                 scanf("%u", &q);
//                 query_power(head, q);
//                 break; 
//             case 4:
//                 scanf("%u%u", &b, &m);
//                 break; 
//             default: 
//                 break;
//         }
//     }

//     return 0;
// }

struct array_node{
	unsigned int label;
    unsigned long long power;
};
typedef struct array_node ArrayNode;

struct power_gain_node{
	unsigned long long power_gain;
    struct power_gain_node* prev;
    struct power_gain_node* next;
};
typedef struct power_gain_node PowerGainNode;

PowerGainNode* create_power_gain_node(unsigned long long power_gain) {
    PowerGainNode *new;
    new = (PowerGainNode*)malloc(sizeof(PowerGainNode));
    new -> power_gain = power_gain;
    new -> prev = NULL;
    new -> next = NULL;
    return new;
}

struct label_node{
	unsigned int rank;
    unsigned int attacks;
    PowerGainNode* power_gain_head;
    PowerGainNode* power_gain_tail;
};
typedef struct label_node LabelNode;

void print_array(ArrayNode* arr, unsigned int N) {
    for (unsigned int i=1; i<=N; i++) {
        printf("%u(%llu) ", arr[i].label, arr[i].power);
    }
    printf("\n");
}

void rank_attack(ArrayNode* arr, unsigned int label, LabelNode* labelnode){
    unsigned int rank = labelnode[label].rank;
    unsigned int tmp;
    if (rank==1) {
        return;
    } else {
        tmp = arr[rank].label;
        arr[rank].label = arr[rank-1].label;
        arr[rank-1].label = tmp;

        if (labelnode[arr[rank-1].label].power_gain_head==NULL || labelnode[arr[rank-1].label].power_gain_tail==NULL) {
            labelnode[arr[rank-1].label].power_gain_head = create_power_gain_node(arr[rank-1].power - arr[rank].power);
            labelnode[arr[rank-1].label].power_gain_tail = labelnode[arr[rank-1].label].power_gain_head;
        } else {
            labelnode[arr[rank-1].label].power_gain_tail->next = create_power_gain_node(arr[rank-1].power - arr[rank].power);
            labelnode[arr[rank-1].label].power_gain_tail->next->prev = labelnode[arr[rank-1].label].power_gain_tail;
            labelnode[arr[rank-1].label].power_gain_tail = labelnode[arr[rank-1].label].power_gain_tail->next;
        }

        arr[rank].power = arr[rank-1].power;
        labelnode[arr[rank].label].rank= rank;
        labelnode[arr[rank-1].label].rank= rank-1;
        labelnode[arr[rank-1].label].attacks++;
    }
}

void reward_each(ArrayNode* arr, unsigned int N){
    for (unsigned int i=1; i<=N; i++) {
        arr[i].power += (N-i);
    }
}

void query_power(ArrayNode* arr, unsigned int N, unsigned long long q){
    unsigned int low = 1;
    unsigned int high = N;
    unsigned int middle = (low + high) >> 1;

    if (arr[1].power < q) {
        printf("0 0\n");
        return;
    }
    while (low < high) {
        middle = (low + high) >> 1;
        if (arr[middle].power>=q) {
            if (middle+1<=N && arr[middle+1].power>=q) {
                low = middle+1;
                if (low==high) {
                    middle = low;
                }
            } else {
                break;
            }
        } else {
            high = middle - 1;
            if (low==high) {
                if (arr[low].power>=q) {
                    middle = low;
                }
            }
        }
    }
    printf("%u %u\n", middle, arr[middle].label);
}

void query_power_gain(LabelNode* labelnode, unsigned int b, unsigned int m) {
    PowerGainNode* tail = labelnode[b].power_gain_tail;
    unsigned long long power_gain = 0;
    while (tail!=NULL) {
        power_gain += tail->power_gain;
        tail = tail->prev;
        m = m - 1;
        if (m==0) {
            break;
        }
    }
    printf("%llu\n", power_gain);
}

int main() {
    unsigned int N, T, M, incident;
    scanf("%u%u%u", &N, &T, &M);

    // ArrayNode arr[N+1];
    // LabelNode labelnode[N+1];
    ArrayNode* arr = malloc(sizeof(ArrayNode)*(N+1)); // index is rank
    LabelNode* labelnode = malloc(sizeof(LabelNode)*(N+1)); // index is label
    for (unsigned int i=1; i<=N; i++) {
        arr[i].label = i;
        labelnode[i].rank = i;
        labelnode[i].attacks = 0;
        labelnode[i].power_gain_head = NULL;
        labelnode[i].power_gain_tail = NULL;
        scanf("%llu", &arr[i].power);
    }
    // print_array(arr, N);


    unsigned int label, b, m;
    unsigned long long q;
    for (unsigned int i=1; i<=T; i++) {
        scanf("%u", &incident);
        switch(incident) { 
            case 1:
                scanf("%u", &label);
                rank_attack(arr, label, labelnode);
                // print_array(arr, N);
                break; 
            case 2:
                reward_each(arr, N);
                // print_array(arr, N);
                break; 
            case 3:
                scanf("%llu", &q);
                query_power(arr, N, q);
                break; 
            case 4:
                scanf("%u%u", &b, &m);
                query_power_gain(labelnode, b, m);
                break; 
            default: 
                break;
        }
    }

    printf("\n");
    for (unsigned int i=1; i<=N; i++) {
        printf("%u ", labelnode[i].attacks);
        for (PowerGainNode* head=labelnode[i].power_gain_head; head!=NULL; head=head->next) {
            printf("%llu ", head->power_gain);
        }
        printf("\n");
    }
}
