#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void print_array(long long* arr, int N, unsigned int swap_count) {
    printf("The array is ");
    for (int i=0; i<N; i++) {
        printf("%lld ", arr[i]);
    }
    printf("after %u back-moves.", swap_count);
}


void insert(long long* a, int m, unsigned int B, unsigned int* swap_count_ptr) {
    long long data = a[m];
    int i = m - 1;
    while (i>=0 && a[i] > data) {
        a[i+1] = a[i];
        i = i -1;
        (*swap_count_ptr)++;
        if (*swap_count_ptr >= B) {
            break;
        }
    }
    a[i+1] = data;
}

void insertion_sort(long long* a, int N, unsigned int B) {
    unsigned int swap_count = 0;
    for (int i=1; i<N; i++) {
        insert(a, i, B, &swap_count);
        if (swap_count >= B) {
            break;
        }
    }
    print_array(a, N, swap_count);
    return;
}

int main() {
    int N;
    unsigned int B;
    scanf("%d%u", &N, &B);

    long long a[N];
    for (int i=0; i<N; i++) {
        scanf("%lld", a+i);
    }
    insertion_sort(a, N, B);
}