#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

int get_min_index(int* a, int left, int right){
    int minidx = left;
    for (int i=left+1; i<right; i++) {
        if (a[i] < a[minidx]) {
            minidx = i;
        }
    }
    return minidx;
} 

void print_array(int* arr, int N, int swap_count) {
    printf("The array is ");
    for (int i=0; i<N; i++) {
        printf("%d ", arr[i]);
    }
    printf("after %d swaps.", swap_count);
}

void selection_sort(int* a, int N, int B) {
    int minidx;
    int swap_count = 0;
    for (int i=0; i<N-1; i++) {
        minidx = get_min_index(a, i, N);
        if (minidx != i) {
            if (swap_count==B) {
                print_array(a, N, B);
                return;
            }
            swap(a+i, a+minidx);
            swap_count++;
        }
    }
    print_array(a, N, swap_count);
    return;
}

int main() {
    int N, B;
    scanf("%d%d", &N, &B);

    int a[N];
    for (int i=0; i<N; i++) {
        scanf("%d", a+i);
    }
    selection_sort(a, N, B);
}