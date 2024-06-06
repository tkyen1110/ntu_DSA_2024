#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define d 26

struct node{
    char S[64];
	unsigned long long key;
    struct node* next;
    struct node* prev;
};
typedef struct node Node;

struct hashtable{
	unsigned int num;
    Node* head;
    Node* tail;
};
typedef struct hashtable HashTable;

unsigned long long hash_func(unsigned long long key, unsigned long long n) {
    unsigned long long hash_val;
    hash_val = key & ((unsigned long long)(1<<n) - 1);
    return hash_val;
}

void print_hash_table(HashTable* ht, unsigned int depth) {
    Node* tmp;
    for (unsigned int i=0; i<depth; i++) {
        if (ht[i].head == NULL) {
            printf("-1\n");
        } else {
            tmp = ht[i].head;
            while (tmp != NULL) {
                if (tmp == ht[i].head) {
                    printf("%s", tmp->S);
                } else {
                    printf(" %s", tmp->S);
                }
                tmp = tmp->next;
            }
            printf("\n");
        }
    }
}

int main() {
    unsigned int N;
    unsigned int r=2, q=0;
    unsigned int depth = (1<<r) + q;
    unsigned int i, j;
    unsigned long long key, hash_val;
    Node* tmp1, *tmp2;
    scanf("%u", &N);

    HashTable* ht = (HashTable*)malloc(sizeof(HashTable)*4);
    for (i=0; i<4; i++) {
        ht[i].num = 0;
        ht[i].head = NULL;
        ht[i].tail = NULL;
    }
    char S[64];
    for (i=1; i<=N; i++) {
        scanf("%s", S);
        key = 0;
        for(j=0; j<(unsigned int)strlen(S); j++) {
            key = d * key + ((unsigned long long)S[j]-'a');
        }
        // printf("S = %s ; length = %ld ; key = %llu\n", S, strlen(S), key);
        Node* new = (Node*)malloc(sizeof(Node));
        strcpy(new->S, S);
        new->key = key;
        new->next = NULL;
        new->prev = NULL;
        hash_val = hash_func(key, r);
        if (hash_val < q) {
            hash_val = hash_func(key, r+1);
        }
        if (ht[hash_val].num == 0) {
            ht[hash_val].head = new;
            ht[hash_val].tail = new;
        } else {
            ht[hash_val].tail->next = new;
            new->prev = ht[hash_val].tail;
            ht[hash_val].tail = ht[hash_val].tail->next;
        }
        ht[hash_val].num = ht[hash_val].num + 1;

        if (ht[hash_val].num > 2) {
            q = q + 1;
            ht = realloc(ht, sizeof(HashTable) * (++depth));
            ht[depth-1].num = 0;
            ht[depth-1].head = NULL;
            ht[depth-1].tail = NULL;

            tmp1 = ht[q-1].head;
            while (tmp1 != NULL) {
                hash_val = hash_func(tmp1->key, r+1);
                if (hash_val == depth-1) {
                    if (tmp1->prev != NULL) {
                        tmp1->prev->next = tmp1->next;
                    } else {
                        ht[q-1].head = tmp1->next;
                    }
                    if (tmp1->next != NULL) {
                        tmp1->next->prev = tmp1->prev;
                    }
                    tmp1->prev = NULL;
                    if (ht[depth-1].num == 0) {
                        ht[depth-1].head = tmp1;
                        ht[depth-1].tail = tmp1;
                    } else {
                        ht[depth-1].tail->next = tmp1;
                        tmp1->prev = ht[depth-1].tail;
                        ht[depth-1].tail = ht[depth-1].tail->next;
                    }
                    ht[q-1].num = ht[q-1].num - 1;
                    ht[depth-1].num = ht[depth-1].num + 1;
                    tmp2 = tmp1;
                    tmp1 = tmp1->next;
                    tmp2->next = NULL;
                } else {
                    tmp1 = tmp1->next;
                }
            }
        }
        if (q == (1<<r)) {
            r = r + 1;
            q = 0;
        }
    }
    print_hash_table(ht, depth);

    return 0;
}