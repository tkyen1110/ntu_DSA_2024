#include <stdio.h>
#include <stdlib.h>

void binary_search(unsigned int* arr, unsigned int N, unsigned int key) {
    unsigned int left = 0;
    unsigned int right = N-1;
    unsigned int middle;
    while (left <= right) {
        printf("Searching %u in range [%u, %u].\n", key, left+1, right+1);
        middle = (left + right) >> 1;
        if (arr[middle]==key) {
            printf("Found at index %u.\n", middle+1);
            return;
        } else if (arr[middle]>key) {
            if (middle==0) {
                printf("Searching %u in range [%u, %u].\n", key, left+1, middle);
                printf("Not found.\n");
                return;
            } else {
                right = middle - 1;
            }
        } else {
            left = middle + 1;
        }
    }
    if (left > right) {
        printf("Searching %u in range [%u, %u].\n", key, left+1, right+1);
    }
    printf("Not found.\n");
    return;
}

int main() {
    unsigned int N, key;
    scanf("%u%u", &N, &key);

    unsigned int a[N];
    for (int i=0; i<N; i++) {
        scanf("%u", a+i);
    }
    binary_search(a, N, key);
}