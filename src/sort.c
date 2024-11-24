#include "sort.h"

void int_swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void float_swap(float* a, float* b) {
    float tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
void merge_sorted_arrays(triangle_t* a, int l, int m, int r) {
        int i, j, k;

        int LA_len = m - l + 1;
        int RA_len = r - m;

        triangle_t LA[LA_len], RA[RA_len];

        for (int i = 0; i < LA_len; i++)
            LA[i] = a[l + i];
        for (int i = 0; i < RA_len; i++)
            RA[i] = a[m + 1 + i];

        i = 0, j = 0, k = l;

        while (i < LA_len && j < RA_len)
        {
            if (LA[i].avg_depth <= RA[j].avg_depth)
                a[k++] = LA[i++];
            else
                a[k++] = RA[j++];
        }

        while (i < LA_len)
            a[k++] = LA[i++];
        while (j < RA_len)
            a[k++] = RA[j++];
}

void merge_sort_recursion(triangle_t* a, int l, int r) {
        if (l < r) {
            int m = (l + r) / 2;

            merge_sort_recursion(a, l, m);
            merge_sort_recursion(a, m + 1, r);

            merge_sorted_arrays(a, l, m, r);
        }
}

void merge_sort(triangle_t* a, int len) {
        merge_sort_recursion(a, 0, len - 1);
}
*/
