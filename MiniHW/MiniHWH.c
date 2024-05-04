#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct node{
	unsigned int valid_shift;
    struct node *next;
};
typedef struct node Node;

unsigned int ascii_to_value(char c) {
    if (c>=65 && c<=90) { // [A-Z]
        return c - 'A';
    } else if (c>=97 && c<=122) { // [a-z]
        return c - 'a' + 26;
    }
}

void compute_prefix_function(unsigned int* P, unsigned int* pi, unsigned int m) {
    // P : pattern
    // m : pattern length
    // pi: prefix function

    unsigned int i, j;

    pi[0] = 0;

    for (i=1; i<m; i++) {
        j = pi[i-1];
        while (j>0 && P[i]!=P[j]) {
            j = pi[j-1];
        }

        if (P[i]==P[j]) {
            j = j + 1;
        }
        pi[i] = j;
    }
}

void kmp_matcher(unsigned int* T, unsigned int* P, unsigned int* pi, unsigned int n, unsigned int m, 
                 Node** vshift_head, Node** vshift_tail) {
    // n : text length
    // m : pattern length
    unsigned int i, j = 0;
    bool valid_shift_flag = false;
    for (i=0; i<n; i++) {
        while (j>0 && T[i]!=P[j]) {
            j = pi[j-1];
        }
        if (T[i]==P[j]) {
            j = j + 1;
        }
        if (j==m) {
            valid_shift_flag = true;
            if (*vshift_head == NULL) {
                *vshift_head = (Node*)malloc(sizeof(Node));
                (*vshift_head)->valid_shift = i+1-m;
                *vshift_tail = *vshift_head;
            } else {
                (*vshift_tail)->next = (Node*)malloc(sizeof(Node));
                *vshift_tail = (*vshift_tail)->next;
                (*vshift_tail)->valid_shift = i+1-m;
            }
            printf("%u ", (*vshift_tail)->valid_shift);
            j = pi[j-1];
        }
    }
    if (valid_shift_flag == false) {
        printf("-1\n");
    } else {
        printf("\n");
    }
}

void check_spurious_hit(char** T, char** P, unsigned int k, unsigned int m, Node* vshift_head) {
    bool spurious_hit_flag = false;
    unsigned int i;
    Node* tmp = vshift_head;
    while (tmp!=NULL) {
        for (i=0; i<k; i++) {
            if (strncmp(T[i] + tmp->valid_shift, P[i], m) != 0) {
                spurious_hit_flag = true;
                printf("%u ", tmp->valid_shift);
                break;
            }
        }  
        tmp = tmp->next;
    }

    if (spurious_hit_flag == false) {
        printf("-1\n");
    } else {
        printf("\n");
    }
}

int main() {
    unsigned int k, n, m, q, i, j;
    const unsigned int d = 52;
    char c;
    scanf("%u%u%u%u", &k, &n, &m, &q);
    do {
        c = getchar();
    } while (c != '\n');

    char** T = (char**)malloc(sizeof(char*)*k);
    unsigned int* Trb = (unsigned int*)malloc(sizeof(unsigned int)*n);
    memset(Trb, 0, sizeof(unsigned int)*n);
    for (i = 0; i < k; i++) {
        T[i] = (char*)malloc(sizeof(char)*(n+2));
        fgets(T[i], sizeof(char)*(n+2), stdin);
        if (T[i][strlen(T[i])-1]=='\n') {
            T[i][strlen(T[i])-1] = '\0';
        }
        for (j = 0; j < n; j++) {
            Trb[j] = (d * Trb[j] + ascii_to_value(T[i][j])) % q;
        }
    }

    char** P = (char**)malloc(sizeof(char*)*k);
    unsigned int* Prb = (unsigned int*)malloc(sizeof(unsigned int)*m);
    memset(Prb, 0, sizeof(unsigned int)*m);
    for (i = 0; i < k; i++) {
        P[i] = (char*)malloc(sizeof(char)*(m+2));
        fgets(P[i], sizeof(char)*(m+2), stdin);
        if (P[i][strlen(P[i])-1]=='\n') {
            P[i][strlen(P[i])-1] = '\0';
        }
        for (j = 0; j < m; j++) {
            Prb[j] = (d * Prb[j] + ascii_to_value(P[i][j])) % q;
        }
    }

    for (i = 0; i < n; i++) {
        printf("%u ", Trb[i]);
    }
    printf("\n");

    for (i = 0; i < m; i++) {
        printf("%u ", Prb[i]);
    }
    printf("\n");

    unsigned int* pi = (unsigned int*)malloc(sizeof(unsigned int)*m);
    Node* vshift_head = NULL;
    Node* vshift_tail = NULL;
    compute_prefix_function(Prb, pi, m);
    kmp_matcher(Trb, Prb, pi, n, m, &vshift_head, &vshift_tail);
    check_spurious_hit(T, P, k, m, vshift_head);
}