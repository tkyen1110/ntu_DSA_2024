#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include<time.h>

int total_num = 0;
int total_num_permute = 0;

struct node{
	long long* result;
	struct node *next;
};
typedef struct node Node;

void swap(long long* a, long long* b) {
    long long tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void selection_sort(long long* a, long long N) {
    long long minval;
    int minidx;
    for (int i=N-1; i>=1; i--) {
        minval = a[i];
        for (int j=i-1; j>=0; j--) {
            if (a[j] < minval) {
                minval = a[j];
                minidx = j;
            }
        }
        if (a[i] != minval) {
            swap(a+i, a+minidx);
        }
    }
}

Node* create_node(void) {
    Node *new;
    new = (Node*)malloc(sizeof(Node));
    new -> result = NULL;
    new -> next = NULL;
    return new;
}

int compare(long long a, long long b){
    if (a>b){
        return 1;
    } else if (a<b){
        return -1;
    } else{
        return 0;
    }
}

void print_array(long long* arr, int N) {
    for (int i=0; i<N; i++) {
        printf("%lld ", arr[i]);
    }
    printf("\n");
}

void print_array_wo_endl(long long* arr, int N) {
    for (int i=0; i<N; i++) {
        printf("%lld ", arr[i]);
    }
}

void print_array_with_comma(long long* arr, int N) {
    for (int i=0; i<N; i++) {
        if (i==0){
            printf("{");
        }
        if (i==N-1){
            printf("%lld}", arr[i]);
        } else{
            printf("%lld, ", arr[i]);
        }
        
    }
    printf("\n");
}

void print_ans(Node* ans, int N) {
    for(Node* head=ans ; head!=NULL ; head=head->next) {
        print_array(head->result, N);
    }
}

void print_both_ans(Node* ans1, Node* ans2, int N) {
    Node* head1;
    Node* head2;
    for(head1=ans1, head2=ans2 ; (head1!=NULL) && (head2!=NULL); head1=head1->next, head2=head2->next) {
        print_array_wo_endl(head1->result, N);
        printf("   |   ");
        print_array(head2->result, N);
    }
}

void clear_ans(Node** ans) {
    Node* current;
    while ((*ans)!=NULL) {
        current = *ans;
        *ans = (*ans)->next;
        free(current);
    }
}

bool compare_ans(Node* ans1, Node* ans2, int N) {
    Node* head1 = ans1;
    Node* head2 = ans2;
    for(head1=ans1, head2=ans2 ; (head1!=NULL) && (head2!=NULL) ; head1=head1->next, head2=head2->next) {
        for (int i=0; i<N; i++) {
            if (head1->result[i] != head2->result[i]) {
                return false;
            }
        }
    }
    return true;
}

void backtrack(long long* a, bool* used, int n, int N, long long* result, Node** ans) {
    if (n==N) {
        Node *new = create_node();
        new->result = (long long*)malloc(sizeof(long long)*N);
        memcpy(new->result, result, sizeof(long long)*N);

        if (*ans==NULL) {
            *ans = new;
        } else {
            new->next = *ans;
            *ans = new;
        }
        total_num++;

        return;
    }

    for (int i=0; i<N; i++) {
        if (!used[i]) {
            if (i>0 && a[i]==a[i-1] && used[i-1]==false) {
                continue;
            }
            used[i] = true;
            result[n] = a[i];
            if ((n<2) || (n>=2 && compare(result[n-1], result[n-2])*compare(result[n], result[n-1]) < 0) || N<=2) {
                backtrack(a, used, n+1, N, result, ans);
                used[i] = false;
            } else {
                used[i] = false;
                continue;
            }
        }
    }
}

bool check_beautiful(long long* result, int N) {
    if (N<=2) {
        return true;
    } else{
        int prev_flag = compare(result[0], result[1]);
        for (int i=2; i<N; i++) {
            if ((prev_flag==1 && (result[i-1] < result[i])) || (prev_flag==-1 && (result[i-1] > result[i]))) {
                prev_flag = compare(result[i-1], result[i]);
            } else {
                return false;
            }
        }
        return true;
    }
}

bool check_duplicate(long long* result, int N, Node* ans) {
    bool duplicate;
    for(Node* head=ans ; head!=NULL ; head=head->next) {
        duplicate = true;
        for (int i=0; i<N; i++) {
            if (result[i] != head->result[i]) {
                duplicate = false;
                break;
            }
        }
        if (duplicate) {
            return true;
        }
    }
    return false;
}

void permutation(long long* a, bool* used, int n, int N, long long* result, Node** ans) {
    if (n==N) {
        if (check_beautiful(result, N) && !check_duplicate(result, N, *ans)){
            Node *new = create_node();
            new->result = (long long*)malloc(sizeof(long long)*N);
            memcpy(new->result, result, sizeof(long long)*N);

            if (*ans==NULL) {
                *ans = new;
            } else {
                new->next = *ans;
                *ans = new;
            }
            total_num_permute++;
        }
        return;
    }

    for (int i=0; i<N; i++) {
        if (!used[i]) {
            used[i] = true;
            result[n] = a[i];
            permutation(a, used, n+1, N, result, ans);
            used[i] = false;
        }
    }
}


void random_number(long long* arr, int N) {
	for(int i=0;i<N;i++)
	{
        arr[i] = rand()%30;
	}
}

int main() {
    int N;
    scanf("%d", &N);

    long long a[N];
    bool used_permute[N];
    bool used[N];
    for (int i=0; i<N; i++) {
        scanf("%lld", a+i);
        used_permute[i] = false;
        used[i] = false;
    }
    selection_sort(a, N);

    long long result_permute[N];
    long long result[N];

    Node* ans_permute = NULL;
    Node* ans = NULL;

    // printf("= = = = = = = = = = = = = = = = = = = = = = = = = = = \n");
    // total_num_permute = 0;
    // permutation(a, used_permute, 0, N, result_permute, &ans_permute);
    // printf("%d\n", total_num_permute);
    // print_ans(ans_permute, N);
    // printf("= = = = = = = = = = = = = = = = = = = = = = = = = = = \n");
    total_num = 0;
    backtrack(a, used, 0, N, result, &ans);
    printf("%d\n", total_num);
    print_ans(ans, N);
    // printf("= = = = = = = = = = = = = = = = = = = = = = = = = = = \n");
    // if ((total_num_permute == total_num) && compare_ans(ans_permute, ans, N)) {
    //     printf("[Pass]\n");
    // }
    clear_ans(&ans_permute);
    clear_ans(&ans);
}


// int main() {
//     time_t t;
//     srand((unsigned)time(&t));

//     int N=10;
//     // scanf("%d", &N);

//     long long a[N];
//     // long long A[13][5] = {{1,2,3,4,5}, {1,1,3,4,5}, {1,2,2,4,5}, {1,2,3,3,5}, {1,2,3,4,4},
//     //                      {1,1,3,3,5}, {1,1,2,3,3}, {1,2,2,3,3}, {1,1,1,4,4}, {1,1,3,3,3},
//     //                      {1,1,1,1,5}, {1,2,2,2,2}, {1,1,1,1,1}
//     //                      };
//     bool used_permute[N];
//     bool used[N];
//     // for (int i=0; i<N; i++) {
//     //     scanf("%lld", a+i);
//     //     used_permute[i] = false;
//     //     used[i] = false;
//     // }
//     // selection_sort(a, N);

//     long long result_permute[N];
//     long long result[N];

//     Node* ans_permute = NULL;
//     Node* ans = NULL;

//     // int imax = sizeof(A)/sizeof(long long)/N;
//     // for (int i=0; i<imax; i++) {
//     while (1){
//         for (int j=0; j<N; j++) {
//             used_permute[j] = false;
//             used[j] = false;
//         }
//         total_num = 0;
//         total_num_permute = 0;
//         // memcpy(a, A[i], sizeof(long long)*N);

//         ans_permute = NULL;
//         ans = NULL;

//         random_number(a, N);
//         selection_sort(a, N);

//         // printf("= = = = = = = = = = = = = = = = = = = = = = = = = = = \n");
//         total_num_permute = 0;
//         permutation(a, used_permute, 0, N, result_permute, &ans_permute);
//         // printf("%d\n", total_num_permute);
//         // print_ans(ans_permute, N);
//         // printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - \n");
//         total_num = 0;
//         backtrack(a, used, 0, N, result, &ans);
//         // printf("%d\n", total_num);
//         // print_ans(ans, N);
//         // printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - \n");
        
//         if (total_num_permute != total_num) {
//             printf("[Fail] ");
//             print_array(a, N);
//             printf("%d != %d\n", total_num_permute, total_num);
//             break;
//         } else{
//             if (compare_ans(ans_permute, ans, N)) {
//                 printf("[Pass] ");
//                 print_array(a, N);
//                 printf("%d == %d\n", total_num_permute, total_num);
//                 // print_ans(ans, N);
//                 print_both_ans(ans_permute, ans, N);
//                 printf("%d == %d\n", total_num_permute, total_num);
//             } else {
//                 printf("[Fail] ");
//                 print_array(a, N);
//                 printf("compare_ans == false\n");
//                 break;
//             }
            
//         }
//         printf("= = = = = = = = = = = = = = = = = = = = = = = = = = = \n");

//         clear_ans(&ans_permute);
//         clear_ans(&ans);
//     }

//     return 0;
// }