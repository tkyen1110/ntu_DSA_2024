#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define DEBUG true

struct node {
	unsigned int parent, current;
    unsigned int size;
    long long salary_raise;
    long long balance_raise;
    long long balance_times;
    unsigned long long salary;
    unsigned long long salary_raise_times;
    unsigned long long balance;
    unsigned long long balance_raise_times;
    // struct node* head;
    struct node* tail;
    struct node* prev;
    struct node* next;
};
typedef struct node Node;

struct salary_node {

};
typedef struct salary_node SalaryNode;

struct balance_node {
    struct balance_node* next;
    struct balance_node* prev;
};
typedef struct balance_node BalanceNode;

void Make_Set(Node* DS, unsigned int i, unsigned long long salary) {
    DS[i].parent = i;
    DS[i].current = i;
    DS[i].size = 1;
    DS[i].salary_raise = 0;
    DS[i].balance_raise = 0;
    DS[i].balance_times = 0;
    DS[i].salary = salary;
    DS[i].salary_raise_times = 0;
    DS[i].balance = 0;
    DS[i].balance_raise_times =  0;
    DS[i].tail = NULL;
    DS[i].prev = NULL;
    DS[i].next = NULL;
}

unsigned int Find_Set(Node* DS, unsigned int x) {
    if (DS[x].parent != x) {
        DS[x].parent = Find_Set(DS, DS[x].parent);
    }
    return DS[x].parent;
}

void Link(Node* DS, unsigned int x, unsigned int y) {
    if (DS[x].size > DS[y].size) {
        DS[y].parent = x;
        DS[x].size = DS[x].size + DS[y].size;
        DS[y].size = 0;
        DS[y].salary_raise_times = DS[x].salary_raise_times;
        DS[y].balance_raise_times = DS[x].balance_raise_times;

        if (DS[x].tail == NULL) {
            DS[x].next = &DS[y];
            DS[y].prev = &DS[x];
        } else {
            DS[x].tail->next = &DS[y];
            DS[y].prev = DS[x].tail;
        }
        if (DS[y].tail == NULL) {
            DS[x].tail = &DS[y];
        } else {
            DS[x].tail = DS[y].tail;
            DS[y].tail = NULL;
        }
    } else {
        DS[x].parent = y;
        DS[y].size = DS[y].size + DS[x].size;
        DS[x].size = 0;
        DS[x].salary_raise_times = DS[y].salary_raise_times;
        DS[x].balance_raise_times = DS[y].balance_raise_times;

        if (DS[y].tail == NULL) {
            DS[y].next = &DS[x];
            DS[x].prev = &DS[y];
        } else {
            DS[y].tail->next = &DS[x];
            DS[x].prev = DS[y].tail;
        }
        if (DS[x].tail == NULL) {
            DS[y].tail = &DS[x];
        } else {
            DS[y].tail = DS[x].tail;
            DS[x].tail = NULL;
        }
    }
}

void update_salary(Node* DS, unsigned int i) {
    long long salary_raise_j;
    if (DS[i].prev == NULL) {
        salary_raise_j = DS[i].salary_raise;
        for(Node* tmp=&DS[i]; tmp!=NULL; tmp=tmp->next) {
            if ((tmp != &DS[i]) && (tmp->salary_raise != 0)) {
                salary_raise_j = DS[i].salary_raise + tmp->salary_raise;
                tmp->salary_raise = 0;
            }
            tmp->salary = tmp->salary + salary_raise_j;
        }
        DS[i].salary_raise = 0;
    }
}

void update_balance(Node* DS, unsigned int i) {
    long long salary_raise_j;
    long long balance_raise_j;
    long long balance_times_j;
    unsigned long long tmp_balance;
    if (DS[i].prev == NULL) {
        // printf("DS[%u].prev == NULL\n", i);
        salary_raise_j = DS[i].salary_raise;
        balance_raise_j = DS[i].balance_raise;
        balance_times_j = DS[i].balance_times;
        for(Node* tmp=&DS[i]; tmp!=NULL; tmp=tmp->next) {
            if (DEBUG) {
                printf("tmp->current = %u\n", tmp->current);
            }
            if (tmp != &DS[i]) {
                balance_times_j = balance_times_j + tmp->balance_times;
            }
            if ((tmp != &DS[i]) && (tmp->salary_raise != 0 || tmp->balance_raise != 0 || tmp->balance_raise_times != 0 || tmp->balance_times != 0)) {
                balance_raise_j = DS[i].balance_raise + tmp->balance_raise + 
                                  (DS[i].balance_raise_times - tmp->balance_raise_times) * tmp->salary_raise;
                if (DEBUG) {
                    printf("%lld = %lld + %lld + (%llu - %llu) * %lld\n", balance_raise_j, DS[i].balance_raise, tmp->balance_raise, DS[i].balance_raise_times, tmp->balance_raise_times, tmp->salary_raise);
                }
                // balance_times_j = DS[i].balance_times + tmp->balance_times;
                tmp->salary_raise_times = 0;
                tmp->balance_raise = 0;
                tmp->balance_raise_times = 0;
                tmp->balance_times = 0;
            }
            tmp_balance = tmp->balance;
            tmp->balance = tmp->balance + balance_raise_j + tmp->salary * balance_times_j;
            if (DEBUG) {
                printf("%llu = %llu + %lld + %llu * %lld\n", tmp->balance, tmp_balance, balance_raise_j, tmp->salary, balance_times_j);
            }
        }
        DS[i].salary_raise_times = 0;
        DS[i].balance_raise = 0;
        DS[i].balance_raise_times = 0;
        DS[i].balance_times = 0;
    }
}

void Union(Node* DS, unsigned int x, unsigned int y) {
    unsigned int v, w;
    v = Find_Set(DS, x);
    w = Find_Set(DS, y);
    Node* tmp;

    if (v != w) {
        if (DS[v].size > DS[w].size) {
            DS[w].salary_raise  = DS[w].salary_raise  - DS[v].salary_raise;
            DS[w].balance_raise = DS[w].balance_raise - DS[v].balance_raise;
            DS[w].balance_times = DS[w].balance_times - DS[v].balance_times;
            // for (tmp=&DS[w]; tmp!=NULL; tmp=tmp->next) {
            //     tmp->salary_raise  = tmp->salary_raise  - DS[v].salary_raise;
            //     tmp->balance_raise = tmp->balance_raise - DS[v].balance_raise;
            //     tmp->balance_times = tmp->balance_times - DS[v].balance_times;
            // }
        } else {
            DS[v].salary_raise  = DS[v].salary_raise  - DS[w].salary_raise;
            DS[v].balance_raise = DS[v].balance_raise - DS[w].balance_raise;
            DS[v].balance_times = DS[v].balance_times - DS[w].balance_times;
            // for (tmp=&DS[v]; tmp!=NULL; tmp=tmp->next) {
            //     tmp->salary_raise  = tmp->salary_raise  - DS[w].salary_raise;
            //     tmp->balance_raise = tmp->balance_raise - DS[w].balance_raise;
            //     tmp->balance_times = tmp->balance_times - DS[w].balance_times;
            // }
        }
        // update_balance(DS, v);
        // update_balance(DS, w);
        // update_salary(DS, v);
        // update_salary(DS, w);
        Link(DS, v, w);
    }
}

// void Print_Path(Node* DS, unsigned int x) {
//     unsigned int k = x;
//     while (DS[k].parent != k) {
//         printf("%u ", k);
//         k = DS[k].parent;
//     }
//     printf("%u\n", k);
// }

void print_debug(Node* DS, unsigned int N) {
    Node* tmp;
    for (unsigned int i=1; i<=N; i++) {
        printf("%u<-%u: salary_raise = %lld ; salary_raise_times = %llu ;  balance_raise = %lld ; balance_raise_times = %llu ; balance_times = %lld ; salary = %llu ; balance = %llu ; ", 
            DS[i].parent, DS[i].current, DS[i].salary_raise, DS[i].salary_raise_times, DS[i].balance_raise, DS[i].balance_raise_times, DS[i].balance_times, DS[i].salary, DS[i].balance);
        if (DS[i].prev == NULL) {
            for (tmp=&DS[i]; tmp!=NULL; tmp=tmp->next) {
                if (tmp==&DS[i]) {
                    printf("%u", tmp->current);
                } else {
                    printf(" -> %u", tmp->current);
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    unsigned int N, Q, op;
    unsigned long long salary, c;
    unsigned int i, j, x, y, k ,r;
    scanf("%u%u", &N, &Q);

    Node* DS = (Node*)malloc(sizeof(Node)*(N+1));
    for (i=1; i<=N; i++) {
        scanf("%llu", &salary);
        Make_Set(DS, i, salary);
    }
    if (DEBUG) {
        print_debug(DS, N);
    }

    for (i=1; i<=Q; i++) {
        scanf("%u", &op);
        switch(op) {
            case 1: // merge
                scanf("%u%u", &x, &y);
                if (DEBUG) {
                    printf("%u: merge %u %u\n", i, x, y);
                }
                Union(DS, x, y);
                break;
            case 2: // raise
                scanf("%u%u", &k, &r);
                if (DEBUG) {
                    printf("%u: raise %u %u\n", i, k, r);
                }
                j = Find_Set(DS, k);
                DS[j].salary_raise += r;
                DS[j].salary_raise_times += 1;
                break;
            case 3: // transfer
                scanf("%u", &k);
                if (DEBUG) {
                    printf("%u: transfer %u\n", i, k);
                }
                j = Find_Set(DS, k);
                DS[j].balance_raise += DS[j].salary_raise;
                DS[j].balance_raise_times += 1;
                DS[j].balance_times += 1;
                break;
            case 4: // quits
                scanf("%u%llu", &k, &c);
                // j = Find_Set(DS, k);
                // if (k == j) {
                //     if (DS[k].next != NULL) {
                //         j = DS[k].next->current;
                //         for(struct node* tmp=DS[k].next; tmp!=NULL; tmp=tmp->next) {
                //             tmp->parent = j;
                //         }
                //         DS[j].head = DS[k].next;
                //         DS[j].tail = DS[k].tail;
                //         DS[k].head = &DS[k];
                //         DS[k].tail = &DS[k];
                //         DS[k].prev = NULL;
                //         DS[k].next = NULL;
                //     }
                // } else {
                //     if (DS[k].next != NULL) {
                //         DS[k].next->prev = DS[k].prev;
                //         DS[k].prev->next = DS[k].next;
                //     } else {
                //         DS[k].prev->next = DS[k].next;
                //         DS[j].tail = DS[k].prev;
                //     }
                //     DS[k].head = &DS[k];
                //     DS[k].tail = &DS[k];
                //     DS[k].prev = NULL;
                //     DS[k].next = NULL;
                // }
                // DS[k].salary = c;
                break;
            case 5: // update
                if (DEBUG) {
                    printf("%u: update\n", i);
                }
                for (j=1; j<=N; j++) {
                    update_balance(DS, j);
                    update_salary(DS, j);
                }
                break;
            default:
                break;
        }
        if (DEBUG) {
            print_debug(DS, N);
        }
    }

    for (j=1; j<=N; j++) {
        update_balance(DS, j);
        update_salary(DS, j);
    }
    if (DEBUG) {
        print_debug(DS, N);
    }
    for (i=1; i<=N; i++) {
        printf("%llu ", DS[i].balance);
    }
    return 0;
}