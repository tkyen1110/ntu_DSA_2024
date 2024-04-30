#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE 5000002
#define min(a,b) (((a) < (b)) ? (a) : (b))

// https://www.youtube.com/watch?v=af1oqpnH1vA
// https://www.naukri.com/code360/library/prefix-function-knuth-morris-pratt-algorithm
void compute_prefix_function(char* P, unsigned int* pi) {
    // P : pattern
    // m : pattern length
    // pi: prefix function
    unsigned int m = strlen(P); // pattern length
    unsigned int i, j;

    pi[0] = 0;

    for (i=1; i<m; i++) {
        j = pi[i-1];
        while (j>0 && P[i]!=P[j]) {
            j = pi[j-1];
        }

        if (P[i]==P[j]) {
            j = j + 1;
        }
        pi[i] = j;
    }

    // printf("prefix function: ");
    // for (i=0; i<m; i++) {
    //     printf("%u ", pi[i]);
    // }
    // printf("\n");
}

unsigned int kmp_matcher(char* T, char* P, unsigned int* pi, unsigned int* hit_count, unsigned int* plagiarism_likelihood) {
    unsigned int n = strlen(T); // text length
    unsigned int m = strlen(P); // pattern length
    unsigned int i, j = 0;

    for (i=0; i<n; i++) {
        while (j>0 && T[i]!=P[j]) {
            j = pi[j-1];
        }
        if (T[i]==P[j]) {
            hit_count[j] = hit_count[j] + 1;
            j = j + 1;
        }
        if (j==m) {
            // printf("Pattern occurs with shift %u\n", i+1-m);
            j = pi[j-1];
        }
    }

    for (i=0; i<m; i++) {
        j = m-1-i;
        if (pi[j]>0) {
            hit_count[pi[j]-1] += hit_count[j];
        }

        if (pi[i]>0) {
            plagiarism_likelihood[i] = plagiarism_likelihood[pi[i]-1] + 1;
        } else {
            plagiarism_likelihood[i] = 1;
        }
    }

    // for (i=0; i<m; i++) {
    //     printf("hit_count = %u / plagiarism_likelihood = %u\n", hit_count[i], plagiarism_likelihood[i]);
    // }
}

int main() {
    char* T = (char*)malloc(sizeof(char)*MAX_SIZE); // text (S)
    char* P = (char*)malloc(sizeof(char)*MAX_SIZE); // pattern (C)
    fgets(T, sizeof(char)*MAX_SIZE, stdin);
    fgets(P, sizeof(char)*MAX_SIZE, stdin);
    if (T[strlen(T)-1]=='\n') {
        T[strlen(T)-1] = '\0';
    }
    if (P[strlen(P)-1]=='\n') {
        P[strlen(P)-1] = '\0';
    }
    // printf("Text    : %s %lu\n", T, strlen(T));
    // printf("Pattern : %s %lu\n", P, strlen(P));
    unsigned int m = strlen(P);
    unsigned int* pi = (unsigned int*)malloc(sizeof(unsigned int)*m);
    unsigned int* hit_count = (unsigned int*)malloc(sizeof(unsigned int)*m);
    unsigned int* plagiarism_likelihood = (unsigned int*)malloc(sizeof(unsigned int)*m);
    memset(pi, 0, sizeof(unsigned int)*m);
    memset(hit_count, 0, sizeof(unsigned int)*m);
    memset(plagiarism_likelihood, 0, sizeof(unsigned int)*m);

    compute_prefix_function(P, pi);
    kmp_matcher(T, P, pi, hit_count, plagiarism_likelihood);

    for (unsigned int i = 0; i < m; i++) {
        printf("%llu\n", (unsigned long long)hit_count[i]*plagiarism_likelihood[i]);
    }
    return 0;
}