#include <stdio.h>
#include <stdlib.h>

struct node{
    long long data;
    struct node *next;
};
typedef struct node Node;

void floyd_cycle_detection(Node* head) {
    Node* hare = head;
    Node* tortoise = head;
    printf("%lld", head->data);
    while (hare != NULL && hare->next != NULL) {
        hare = hare->next->next;
        tortoise = tortoise->next;
        if (hare != NULL) {
            printf(" %lld", hare->data);
        }
        if (hare == tortoise) {
            return;
        }
    }

    return;
}

int main() {
    unsigned int N, next;
    long long data;
    scanf("%u", &N);

    Node a[N+1];
    for (unsigned int i=1; i<=N; i++) {
        scanf("%lld%u", &data, &next);
        a[i].data = data;
        if (next == 0) {
            a[i].next = NULL;
        } else {
            a[i].next = a+next;
        }
    }
    floyd_cycle_detection(a+1);
}