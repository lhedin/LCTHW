#ifndef darray_algos_h
#define darray_algos_h

#include <lcthw/darray.h>

typedef int (*DArray_compare) (const void* a, const void* b);

int DArray_qsort(DArray* array, DArray_compare cmp);
int DArray_heapsort(DArray* array, DArray_compare cmp);
int DArray_mergesort(DArray* array, DArray_compare cmp);

void DArray_sort_add(DArray* array, void* el, DArray_compare cmp);

char* DArray_find(DArray* array, char* el, DArray_compare cmp);

#endif
