#ifndef SORT_H
#define SORT_H

#include "triangle.h"

void merge_sort(triangle_t* a, int len);
void merge_sort_recursion(triangle_t* a, int l, int r);
void merge_sorted_arrays(triangle_t* a, int l, int m, int r);

void int_swap(int* a, int* b);
void float_swap(float* a, float* b);
#endif
