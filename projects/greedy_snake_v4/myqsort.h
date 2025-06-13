#ifndef MY_QSORT_H
#define MY_QSORT_H

#include <stddef.h>  // 为了 size_t

// 函数声明：与标准库 qsort 完全一致
void quicksort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

#endif
