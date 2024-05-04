#include <stdio.h>
#include <stdlib.h>

void swap(long long* a, long long* b, unsigned int* count) {
    long long tmp = *a;
    *a = *b;
    *b = tmp;
    (*count)++;
}

unsigned int partition(long long* A , unsigned int L, unsigned int R, unsigned int B, unsigned int* count) {
    long long pivot = A[L];
    unsigned int i = L+1;
    unsigned int j = R;

    while (i<j) {
        while (i<=R && A[i]<=pivot) {
            i++;
        }
        while (j>=L && A[j]>pivot) {
            j--;
        }
        if (i>=j) {
            break;
        }
        swap(A+i, A+j, count);
        if (*count==B) {
            return 0;
        }
    }
    if (L<j && A[L]>A[j]) {
        swap(A+L, A+j, count);
        if (*count==B) {
            return 0;
        }
    }
    return j;
}

void quicksort(long long* A, unsigned int L, unsigned int R, unsigned int B, unsigned int* count) {
    unsigned int M;
    if (*count==B) {
        return;
    }
    if (L<R) {
        M = partition(A, L, R, B, count);
        if (*count==B) {
            return;
        }
        quicksort(A, L, M-1, B, count);
        quicksort(A, M+1, R, B, count);
    }
}

int main() {
    unsigned int N, B;
    scanf("%u%u", &N, &B);

    long long* A = malloc((N + 1) * sizeof(long long));
    for (unsigned int i=1; i<=N; i++) {
        scanf("%lld", A+i);

    }

    unsigned int count = 0;
    quicksort(A, 1, N, B, &count);

    printf("The array is ");
    for (unsigned int i=1; i<=N; i++) {
        printf("%lld ", A[i]);
    }
    printf("after %u swaps.\n", count);
}