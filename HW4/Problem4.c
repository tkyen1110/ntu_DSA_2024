#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct node{
	unsigned int parent, current;
    unsigned int rank;
    unsigned long long salary;
    unsigned long long balance;
    struct node* head;
    struct node* tail;
    struct node* prev;
    struct node* next;
};
typedef struct node Node;

void Make_Set(Node* DS, unsigned int i, unsigned long long salary) {
    DS[i].parent = i;
    DS[i].current = i;
    DS[i].rank = 0;
    DS[i].salary = salary;
    DS[i].balance = 0;
    DS[i].head = &DS[i];
    DS[i].tail = &DS[i];
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
    if (DS[x].rank > DS[y].rank) {
        DS[y].parent = x;

        DS[x].tail->next = DS[y].head;
        DS[y].head->prev = DS[x].tail;
        DS[x].tail = DS[y].tail;
        DS[y].head = NULL;
        DS[y].tail = NULL;
    } else {
        DS[x].parent = y;
        if (DS[x].rank == DS[y].rank) {
            DS[y].rank = DS[y].rank + 1;
        }

        DS[y].tail->next = DS[x].head;
        DS[x].head->prev = DS[y].tail;
        DS[y].tail = DS[x].tail;
        DS[x].head = NULL;
        DS[x].tail = NULL;
    }
}

void Union(Node* DS, unsigned int x, unsigned int y) {
    unsigned int v, w;
    v = Find_Set(DS, x);
    w = Find_Set(DS, y);
    if (v != w) {
        Link(DS, v, w);
    }
}

void Print_Path(Node* DS, unsigned int x) {
    unsigned int k = x;
    while (DS[k].parent != k) {
        printf("%u ", k);
        k = DS[k].parent;
    }
    printf("%u\n", k);
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

    for (i=1; i<=Q; i++) {
        scanf("%u", &op);
        switch(op) {
            case 1: // merge
                scanf("%u%u", &x, &y);
                Union(DS, x, y);
                break; 
            case 2: // raise
                scanf("%u%u", &k, &r);
                j = Find_Set(DS, k);
                for(struct node* tmp=DS[j].head; tmp!=NULL; tmp=tmp->next) {
                    tmp->salary = tmp->salary + r;
                }
                break; 
            case 3: // transfer
                scanf("%u", &k);
                j = Find_Set(DS, k);
                for(struct node* tmp=DS[j].head; tmp!=NULL; tmp=tmp->next) {
                    tmp->balance = tmp->balance + tmp->salary;
                }
                break;
            case 4: // quits
                scanf("%u%llu", &k, &c);
                j = Find_Set(DS, k);
                if (k == j) {
                    if (DS[k].next != NULL) {
                        j = DS[k].next->current;
                        for(struct node* tmp=DS[k].next; tmp!=NULL; tmp=tmp->next) {
                            tmp->parent = j;
                        }
                        DS[j].head = DS[k].next;
                        DS[j].tail = DS[k].tail;
                        DS[k].head = &DS[k];
                        DS[k].tail = &DS[k];
                        DS[k].prev = NULL;
                        DS[k].next = NULL;
                    }
                } else {
                    if (DS[k].next != NULL) {
                        DS[k].next->prev = DS[k].prev;
                        DS[k].prev->next = DS[k].next;
                    } else {
                        DS[k].prev->next = DS[k].next;
                        DS[j].tail = DS[k].prev;
                    }
                    DS[k].head = &DS[k];
                    DS[k].tail = &DS[k];
                    DS[k].prev = NULL;
                    DS[k].next = NULL;
                }
                DS[k].salary = c;
                break;
            default:
                break;
        }
    }

    for (i=1; i<=N; i++) {
        printf("%llu ", DS[i].balance);
    }
    return 0;
}