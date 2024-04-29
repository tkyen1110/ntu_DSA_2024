#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE 5000002
#define min(a,b) (((a) < (b)) ? (a) : (b))

// https://www.youtube.com/watch?v=af1oqpnH1vA
// https://www.naukri.com/code360/library/prefix-function-knuth-morris-pratt-algorithm
void compute_prefix_function(char* P, unsigned int** pi) {
    // P : pattern
    // pi: prefix function
    unsigned int m = strlen(P); // pattern length
    unsigned int i, j;
    *pi = (unsigned int*)malloc(sizeof(unsigned int)*m);
    (*pi)[0] = 0;

    for (i=1; i<m; i++) {
        j = (*pi)[i-1];
        while (j>0 && P[i]!=P[j]) {
            j = (*pi)[j-1];
        }

        if (P[i]==P[j]) {
            j = j + 1;
        }
        (*pi)[i] = j;
    }

    // printf("prefix function: ");
    // for (i=0; i<m; i++) {
    //     printf("%u ", (*pi)[i]);
    // }
    // printf("\n");
}

unsigned int kmp_matcher(char* T, char* P, unsigned int* pi, unsigned int m) {
    unsigned int n = strlen(T); // text length
    // unsigned int m = strlen(P); // pattern length
    unsigned int i, j = 0;
    unsigned int hit_count = 0;
    if (n < m) {
        return hit_count;
    }
    for (i=0; i<n; i++) {
        while (j>0 && T[i]!=P[j]) {
            j = pi[j-1];
        }
        if (T[i]==P[j]) {
            j = j + 1;
        }
        if (j==m) {
            // printf("Pattern occurs with shift %u\n", i+1-m);
            hit_count = hit_count + 1;
            j = pi[j-1];
        }
    }
    return hit_count;
}

// https://www.naukri.com/code360/library/count-all-substrings-of-all-lengths-that-match-exactly-with-the-prefix-and-suffix-of-a-string
// https://www.naukri.com/code360/guided-paths/interview-guide-for-product-based-companies/content/111307/offering/1376457
// https://www.naukri.com/code360/library/length-of-all-prefixes-that-are-also-the-suffixes-of-the-given-string
// https://codeforces.com/blog/entry/3107?#comment-692814


// https://hackmd.io/@Cutiemango/B1A8muQIL
unsigned int compute_z_function(char* P, unsigned int** z, unsigned int m) {
    // unsigned int m = strlen(P);
    unsigned int i, L=0, R=0;
    unsigned int plagiarism_likelihood = 1;

    *z = (unsigned int*)malloc(sizeof(unsigned int)*m);
    (*z)[0] = 0;

    for (i = 1; i < m; i++) {
        if (i + (*z)[i-L] < R) {
            (*z)[i] = min((*z)[i-L], R-i);
        } else {
            while (i + (*z)[i] < m && P[(*z)[i]] == P[i + (*z)[i]]) {
                (*z)[i]++;
            }
        }

        if (m-i == (*z)[i]) {
            plagiarism_likelihood++;
        }
    
        if (i + (*z)[i] > R) {
            R = i + (*z)[i];
            L = i;
        }
    }

    // printf("z: ");
    // for (i = 0; i < m; i++) {
    //     printf("%u ", (*z)[i]);
    // }
    // printf("\n");
    return plagiarism_likelihood;
}

// https://codeforces.com/blog/entry/9612?locale=ru#comment-217621
unsigned int prefix_function_to_z_function(char* P, unsigned int* pi, unsigned int** z, unsigned int m) {
    // unsigned int m = strlen(P);
    unsigned int i, j, v;
    unsigned int plagiarism_likelihood = 1;

    *z = (unsigned int*)malloc(sizeof(unsigned int)*m);
    (*z)[0] = 0;

    for (i = 1; i < m; i++) {
        (*z)[i] = 0;
        if (pi[i]) {
            (*z)[i - pi[i] + 1] = pi[i];
        }
    }

    for (i = 1; i < m; ) {
        for (j = 1; j < (*z)[i] && (v = min((*z)[j], (*z)[i] - j)) >= (*z)[i + j] ; ++j) {
            (*z)[i + j] = v;
        }
        i += j;
    }

    // printf("z: ");
    for (i = 0; i < m; i++) {
        if (m-i == (*z)[i]) {
            plagiarism_likelihood++;
        }
        // printf("%u ", (*z)[i]);
    }
    // printf("\n");
    return plagiarism_likelihood;
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

    unsigned int* pi;
    unsigned int *z1, *z2;
    compute_prefix_function(P, &pi);
    for (unsigned int m = 1; m <= strlen(P); m++) {
        printf("%llu\n", (unsigned long long)kmp_matcher(T, P, pi, m)*compute_z_function(P, &z1, m));
        // printf("%u\n", kmp_matcher(T, P, pi, m));
        // printf("%u\n", compute_z_function(P, &z1, m));
        // printf("%u\n", prefix_function_to_z_function(P, pi, &z2, m));
        // printf("\n");
    }
    return 0;
}