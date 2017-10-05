#ifndef lcthw_List_Algos_h
#define lcthw_List_Algos_h

#include <lcthw/list.h>

typedef int (*List_compare)(const void *a, const void *b);

int* merge_sort(int* A, int* B, int n);


int   List_bubble_sort(List* list, List_compare compare);
List* List_merge_sort(List* list, List_compare compare);

#endif
