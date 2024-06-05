#include <stdio.h>
#include <stdlib.h>

struct node{
    char color;
	unsigned int key;
    struct node* left;
    struct node* right;
    struct node* parent;
};
typedef struct node Node;

Node* create_node(char color, unsigned int key) {
    Node *new;
    new = (Node*)malloc(sizeof(Node));
    new -> color = color;
    new -> key = key;
    new -> left = NULL;
    new -> right = NULL;
    new -> parent = NULL;
    return new;
}

Node* traverse_parent(Node* root, unsigned int key) {
    Node *parent;
    Node *current = root;
    while (current != NULL) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return parent;
}

Node* traverse_current(Node* root, unsigned int key) {
    Node *current = root;
    while (current != NULL) {
        if (key == current->key) {
            return current;
        }
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
}

void insert_node(Node** root, char color, unsigned int key) {
    Node *new = create_node(color, key);
    Node *parent;
    if (*root == NULL) {
        *root = new;
    } else {
        parent = traverse_parent(*root, key);
        if (key < parent->key) {
            parent->left = new;
        } else {
            parent->right = new;
        }
        new->parent = parent;
    }
}

void print_preorder(Node* root) {
    if (root == NULL) {
        return;
    }
    printf("%c %u\n", root->color, root->key);
    print_preorder(root->left);
    print_preorder(root->right);
}

void left_rotate(Node** root, unsigned int key) {
    Node* x = traverse_current(*root, key);
    Node* y;
    y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void right_rotate(Node** root, unsigned int key) {
    Node* y = traverse_current(*root, key);
    Node* x;
    x = y->left;
    y->left = x->right;
    if (x->right != NULL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NULL) {
        *root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

void print_information(Node* root, unsigned int key) {
    Node *current = root;
    unsigned int BD = 0;
    unsigned int D = 0;
    while (current != NULL) {
        // parent = current;
        if (key == current->key) {
            break;
        } else {
            if (current->color == 'B') {
                BD = BD + 1;
            }
            D = D + 1;
        }

        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (current->left == NULL) {
        printf("%d ", -1);
    } else {
        printf("%u ", current->left->key);
    }
    if (current->right == NULL) {
        printf("%d ", -1);
    } else {
        printf("%u ", current->right->key);
    }
    printf("%u %u\n", BD, D);
}

int main() {
    unsigned int N, M, key;
    unsigned int i;
    char color, op;
    Node* root = NULL;

    scanf("%u", &N);
    for (i=1; i<=N; i++) {
        scanf(" %c%u", &color, &key);
        insert_node(&root, color, key);
    }
    // print_preorder(root);

    scanf("%u", &M);
    for (i=1; i<=M; i++) {
        scanf(" %c%u", &op, &key);
        switch(op) {
            case 'L':
                left_rotate(&root, key);
                break; 
            case 'R':
                right_rotate(&root, key);
                break; 
            case 'P':
                print_information(root, key);
                break;
            default:
                break;
        }
    }
    return 0;
}