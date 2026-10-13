#ifndef UTILS_H
#define UTILS_H

static inline void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

static inline int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

#endif
