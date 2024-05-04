#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

struct eval_node{
    long long num;
    struct eval_node *next;
};
typedef struct eval_node EvalNode;

EvalNode* create_eval_node(char token) {
    EvalNode *new;
    new = (EvalNode*)malloc(sizeof(EvalNode));
    new -> num = token - '0';
    new -> next = NULL;
    return new;
}

void print_eval_node(EvalNode* evalhead) {
    while (evalhead!=NULL) {
        printf("%lld ", evalhead->num);
        evalhead = evalhead->next;
    }
    printf("\n");
}

void push_eval_node(EvalNode** evalhead, char token) {
    EvalNode* new = create_eval_node(token);
    if (*evalhead == NULL) {
        *evalhead = new;
    } else {
        new->next = *evalhead;
        *evalhead = new;
    }
}

void revise_eval_node(EvalNode** evalhead, char token) {
    (*evalhead)->num = (*evalhead)->num * 10 + (token - '0');

}

long long pop_eval_node(EvalNode** evalhead) {
    long long num = (*evalhead)->num;
    EvalNode* tmp = *evalhead;
    *evalhead = (*evalhead)->next;
    free(tmp);
    return num;
}

void calculate(EvalNode** evalhead, char operator) {
    long long num2 = pop_eval_node(evalhead);
    long long num1 = (*evalhead)->num;

    switch(operator) {
        case '+':
            (*evalhead)->num = num1 + num2;
            break;
        case '-':
            (*evalhead)->num = num1 - num2;
            break;
        case '*':
            (*evalhead)->num = num1 * num2;
            break;
        case '/':
            (*evalhead)->num = num1 / num2;
            break;
        case '%':
            (*evalhead)->num = num1 % num2;
            break;
        default:
            break;
    }
    // printf("\n%lld %c %lld = %lld\n", num1, operator, num2, (*evalhead)->num);
}

struct node{
    char token;
    struct node *next;
};
typedef struct node Node;

Node* create_node(char token) {
    Node *new;
    new = (Node*)malloc(sizeof(Node));
    new -> token = token;
    new -> next = NULL;
    return new;
}

void print_node(Node* head) {
    while (head!=NULL) {
        printf("%c ", head->token);
        head = head->next;
    }
    printf("\n");
}

void push(Node** head, char token) {
    Node* new = create_node(token);
    if (*head == NULL) {
        *head = new;
    } else {
        new->next = *head;
        *head = new;
    }
}

char peep(Node* head) {
    return head->token;
}

char pop(Node** head) {
    char token = (*head)->token;
    Node* tmp = *head;
    *head = (*head)->next;
    free(tmp);
    return token;
}

bool precedence_comparison(char token1, char token2) {
    if (token1=='(') {
        return false;
    }
    int precedence1, precedence2;
    if (token1 == '*' || token1 == '/' || token1 == '%') {
        precedence1 = 1;
    } else {
        precedence1 = 0;
    }

    if (token2 == '*' || token2 == '/' || token2 == '%') {
        precedence2 = 1;
    } else {
        precedence2 = 0;
    }
    return precedence1>=precedence2;
}

void infix_to_postfix(char token, Node** head, EvalNode** evalhead, bool reset) {
    static bool last_is_digit = false;
    if (reset) {
        last_is_digit = false;
    }
    char operator;
    if isdigit(token) {
        printf("%c", token);
        if (last_is_digit==false) {
            push_eval_node(evalhead, token);
            last_is_digit = true;
        } else {
            revise_eval_node(evalhead, token);
        }
    } else {
        switch(token) {
            case '(':
                push(head, token);
                last_is_digit = false;
                break;
            case ')':
                while (*head != NULL && peep(*head)!='(') {
                    operator = pop(head);
                    printf("%c", operator);
                    calculate(evalhead, operator);
                }
                pop(head);
                last_is_digit = false;
                break;
            default:
                while (*head != NULL && precedence_comparison(peep(*head), token)) {
                    operator = pop(head);
                    printf("%c", operator);
                    calculate(evalhead, operator);
                }
                push(head, token);
                last_is_digit = false;
                break;
        }
    }
}

int main() {
    char token, operator;
    bool reset;
    Node* head = NULL;
    EvalNode* evalhead = NULL;
    for (int i=1; i<=3; i++) {
        reset = true;
        scanf("%c", &token);
        while (token!='\n') {
            infix_to_postfix(token, &head, &evalhead, reset);
            reset = false;
            scanf("%c", &token);
        }
        while (head != NULL) {
            operator = pop(&head);
            printf("%c", operator);
            calculate(&evalhead, operator);
        }
        printf("=");
        print_eval_node(evalhead);
        pop_eval_node(&evalhead);
    }
}