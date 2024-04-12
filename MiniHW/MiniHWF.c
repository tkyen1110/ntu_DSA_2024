#include <stdio.h>
#include <stdlib.h>

void swap(unsigned int* a, unsigned int* b) {
    unsigned int tmp = *a;
    *a = *b;
    *b = tmp;
}

int main() {
    unsigned int N;
    unsigned int idx = 0;
    unsigned int j, value;
    scanf("%u", &N);

    unsigned int* A = malloc((N + 1) * sizeof(unsigned int));
    for (int i=1; i<=N; i++) {
        scanf("%u", &value);
        A[++idx] = value;
        j = idx;
        while (j > 1 && A[j/2] < A[j]) {
            swap(&A[j/2], &A[j]);
            j = j/2;
        }
    }
    for (int i=1; i<=N; i++) {
        printf("%u ", A[i]);
    }
    // printf("\n");
}