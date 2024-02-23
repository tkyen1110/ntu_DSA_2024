#include <stdio.h>
#include <stdlib.h>

struct node{
	int value;
	struct node *next;
};
typedef struct node Node;

Node* create_node(int value) {
    Node *new;
    new = (Node*)malloc(sizeof(Node));
    new -> value = value;
    new -> next = NULL;
    return new;
}

void print_node(Node* node) {
    for(Node* head=node ; head!=NULL ; head=head->next) {
        printf("%d ", head->value);
    }
}

void addi(int i, int u, int* nums, Node** decks_head, Node** decks_tail){
    Node* new = create_node(u);
    if (decks_head[i] == NULL) {
        decks_head[i] = new;
        decks_tail[i] = new;
    } else{
        new -> next = decks_head[i];
        decks_head[i] = new;
    }
    nums[i] += 1;
}

void removei(int i, int* nums, Node** decks_head, Node** decks_tail){
    if (decks_head[i] != NULL) {
        Node* head = decks_head[i];
        decks_head[i] = decks_head[i] -> next;
        if (decks_head[i] == NULL) {
            decks_tail[i] = NULL;
        }
        free(head);
        nums[i] -= 1;
    }
}

void movei(int i, int j, int* nums, Node** decks_head, Node** decks_tail){
    if (decks_head[i] != NULL) {
        decks_tail[i] -> next = decks_head[j];
        decks_head[j] = decks_head[i];
        if (decks_tail[j] == NULL) {
            decks_tail[j] = decks_tail[i];
        }
        decks_head[i] = NULL;
        decks_tail[i] = NULL;

        nums[j] = nums[i] + nums[j];
        nums[i] = 0;
    }
}

void mergei(int i, int j, int* nums, Node** decks_head, Node** decks_tail){
    Node *head, *tail;
    int count = 0;
    if (decks_head[i] != NULL) {
        head = decks_head[i];
        tail = decks_head[i];
        decks_head[i] = decks_head[i] -> next;
        while (decks_head[i]!=NULL & decks_head[j]!=NULL) {
            if (count%2 == 0) {
                tail -> next = decks_head[j];
                decks_head[j] = decks_head[j] -> next;
            } else{
                tail -> next = decks_head[i];
                decks_head[i] = decks_head[i] -> next;
            }
            tail = tail -> next;
            count++;
        }
        if (decks_head[i]==NULL) {
            tail -> next = decks_head[j];
        }
        if (decks_head[j]==NULL) {
            tail -> next = decks_head[i];
            decks_tail[j] = decks_tail[i];
        }
        decks_head[j] = head;
        decks_head[i] = NULL;
        decks_tail[i] = NULL;

        nums[j] = nums[i] + nums[j];
        nums[i] = 0;
    }
}

int main() {
    int N, M;
    scanf("%d%d", &N, &M);

    int param[3];
    int nums[N+1];
    Node* decks_head[N+1];
    Node* decks_tail[N+1];
    for (int i=1; i<=N; i++) {
        nums[i] = 0;
        decks_head[i] = NULL;
        decks_tail[i] = NULL;
    }

    for (int i=1; i<=M; i++) {
        scanf("%d", &param[0]);
        if (param[0]==2) {
            scanf("%d", &param[1]);
        } else{
            scanf("%d%d", &param[1], &param[2]);
        }

        switch(param[0]) { 
            case 1: 
                addi(param[1], param[2], nums, decks_head, decks_tail);
                break; 
            case 2: 
                removei(param[1], nums, decks_head, decks_tail);
                break; 
            case 3: 
                movei(param[1], param[2], nums, decks_head, decks_tail);
                break; 
            case 4: 
                mergei(param[1], param[2], nums, decks_head, decks_tail);
                break; 
            default: 
                break;
        }

    }

    for (int i=1; i<=N; i++) {
        printf("%d ", nums[i]);
        print_node(decks_head[i]);
        printf("\n");
    }
    return 0;
}