#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct node{
	unsigned int value, size;
	int priority;
    unsigned long long sum;
	struct node *left, *right;
};
typedef struct node Node;

Node* create_node(unsigned int value) {
    Node *new;
    new = (Node*)malloc(sizeof(Node));
    new -> value = value;
    new -> size = 1;
    new -> priority = rand();
    new -> sum = value;
    new -> left = NULL;
    new -> right = NULL;
    return new;
}

void print_inorder(Node* root, unsigned int level, bool debug) {
    if (root == NULL)
        return;
    print_inorder(root->left, level+1, debug);
    if (debug) {
        printf("%u(sum = %llu, level = %d) ", root->value, root->sum, level);
    } else {
        printf("%u ", root->value);
    }
    print_inorder(root->right, level+1, debug);
}

void print_inorder_newline_debug(Node* root) {
    return;
    print_inorder(root, 1, true);
    printf("\n");
}

void print_inorder_newline(Node* root) {
    print_inorder(root, 1, false);
    printf("\n");
}

unsigned int size(Node *root) {
    if (root == NULL) {
        return 0;
    } else {
        return root->size;
    }
}

unsigned long long sum(Node *root) {
    if (root == NULL) {
        return 0;
    } else {
        return root->sum;
    }
}

void split(Node *root, Node** left, Node** right, unsigned int k) {
    Node *tmp_left=NULL, *tmp_right=NULL;
    if (root == NULL) {
        return;
    }
	if (size(root->left) < k) {
        *left = root;
        split(root->right, &tmp_right, right, k - size(root->left) - 1);
        root->right = tmp_right;
	} else {
        *right = root;
		split(root->left, left, &tmp_left, k);
        root->left = tmp_left;
	}
    root->size = 1 + size(root->left) + size(root->right);
    root->sum = (unsigned long long)root->value + sum(root->left) + sum(root->right);
}

Node* merge(Node* left, Node* right) {
	if (left == NULL) {
		return right;
	}
	if (right == NULL) {
		return left;
	}

    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        left->size = 1 + size(left->left) + size(left->right);
        left->sum = (unsigned long long)left->value + sum(left->left) + sum(left->right);
        return left;
    } else {
        right->left = merge(left, right->left);
        right->size = 1 + size(right->right) + size(right->left);
        right->sum = (unsigned long long)right->value + sum(right->left) + sum(right->right);
        return right;
    }
}

void compute_sum(Node *root, unsigned int L, unsigned int R) {
    Node *left = NULL;
    Node *mid = NULL;
    Node *right = NULL;
    if (L > 1) {
        split(root, &left, &mid, L-1);
    } else {
        left = NULL;
        mid = root;
    }

    if (R - size(left) == mid->size) {
        right = NULL;
    } else {
        split(mid, &mid, &right, R-L+1);
    }
    printf("%llu\n", mid->sum);
    print_inorder_newline_debug(left);
    print_inorder_newline_debug(mid);
    print_inorder_newline_debug(right);
    root = merge(left, mid);
    root = merge(root, right);
}

int main() {
    unsigned int N, T, op;
    scanf("%u%u", &N, &T);

    // srand(time(NULL));
    srand(0);
    unsigned int i, value;
    Node* treap = NULL;
    Node* tmp = NULL;
    for (i=1; i<=N; i++) {
        scanf("%u", &value);
        tmp = create_node(value);
        treap = merge(treap, tmp);
    }
    print_inorder_newline_debug(treap);

    unsigned int k, a, L, R;
    Node *left, *mid, *right;
    for (unsigned int i=1; i<=T; i++) {
        scanf("%u", &op);

        switch(op) { 
            case 1:
                // Delete the k-th element from the sequence
                scanf("%u", &k);
                left = NULL;
                mid = NULL;
                right = NULL;
                split(treap, &left, &right, k);
                if (k > 1) {
                    split(left, &left, &mid, k-1);
                    free(mid);
                    treap = merge(left, right);
                } else {
                    free(left);
                    treap = right;
                }
                print_inorder_newline_debug(treap);
                break;
            case 2:
                // Insert integer a between the k-th and the (k + 1)-th elements
                scanf("%u%u", &k, &a);
                tmp = create_node(a);
                left = NULL;
                right = NULL;
                if (k == 0) {
                    treap = merge(tmp, treap);
                } else if (k == treap->size) {
                    treap = merge(treap, tmp);
                } else {
                    split(treap, &left, &right, k);
                    left = merge(left, tmp);
                    treap = merge(left, right);
                }
                print_inorder_newline_debug(treap);
                break;
            case 3:
                // The sum of the elements between the l-th and the r-th elements in the sequence should be calculated
                scanf("%u%u", &L, &R);
                compute_sum(treap, L, R);
                print_inorder_newline_debug(treap);
                break;
            default:
                break;
        }
    }

    if (treap == NULL) {
        printf("0\n");
    } else {
        printf("%u\n", treap->size);
    }
    print_inorder_newline(treap);
    return 0;
}