#include "myqsort.h"
#include <string.h>  // 为了 memcpy 和 memcmp（未直接使用，但可用于优化）

// 辅助函数：交换两个元素的内存内容
static void swap(char *a, char *b, size_t size) {
    char temp;
    for (size_t i = 0; i < size; ++i) {
        temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
}

// 辅助函数：三数取中法选择枢轴
static char *median_of_three(char *low, char *mid, char *high, 
                            int (*compar)(const void *, const void *)) {
    if (compar(low, mid) < 0) {
        if (compar(mid, high) < 0) return mid;
        else if (compar(low, high) < 0) return high;
        else return low;
    } else {
        if (compar(low, high) < 0) return low;
        else if (compar(mid, high) < 0) return high;
        else return mid;
    }
}

// 递归排序辅助函数
static void quicksort_helper(char *low, char *high, size_t size,
                            int (*compar)(const void *, const void *)) {
    if (low >= high) return;

    // 计算子数组的中位数枢轴
    size_t num_elements = (high - low) / size + 1;
    char *mid = low + ((num_elements - 1) / 2) * size;
    char *pivot = median_of_three(low, mid, high, compar);

    // 将枢轴交换到末尾（high位置）
    swap(pivot, high, size);
    pivot = high;

    // Lomuto分区逻辑
    char *i = low - size;
    for (char *j = low; j < high; j += size) {
        if (compar(j, pivot) < 0) {
            i += size;
            if (i != j) swap(i, j, size);
        }
    }

    // 将枢轴放到正确的位置（i+size）
    i += size;
    if (i != high) swap(i, high, size);

    // 递归排序左半部分和右半部分
    quicksort_helper(low, i - size, size, compar);
    quicksort_helper(i + size, high, size, compar);
}

// 对外暴露的快速排序函数
void quicksort(void *base, size_t nmemb, size_t size, 
               int (*compar)(const void *, const void *)) {
    if (nmemb <= 1) return;
    char *low = (char *)base;
    char *high = low + (nmemb - 1) * size;
    quicksort_helper(low, high, size, compar);
}
