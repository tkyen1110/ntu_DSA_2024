#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct node{
	unsigned int parent;
    unsigned int rank;
};
typedef struct node Node;

void Make_Set(Node* DS, unsigned int N) {
    for (unsigned int i=1; i<=N; i++) {
        DS[i].parent = i;
        DS[i].rank = 0;
    }
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
    } else {
        DS[x].parent = y;
        if (DS[x].rank == DS[y].rank) {
            DS[y].rank = DS[y].rank + 1;
        }
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
    unsigned int N, M;
    unsigned int i, x, y;
    char op;
    scanf("%u", &N);
    scanf("%u", &M);

    Node* DS = (Node*)malloc(sizeof(Node)*(N+1));
    Make_Set(DS, N);

    for (i=1; i<=M; i++) {
        scanf("%c", &op);
        scanf("%c", &op);
        switch(op) {
            case 'F': // Find-Set(x)
                scanf("%u", &x);
                Find_Set(DS, x);
                break; 
            case 'U': // Union(x, y)
                scanf("%u%u", &x, &y);
                Union(DS, x, y);
                break; 
            case 'P': // Print-Path(x)
                scanf("%u", &x);
                Print_Path(DS, x);
                break;
            default:
                break;
        }
    }

    return 0;
}