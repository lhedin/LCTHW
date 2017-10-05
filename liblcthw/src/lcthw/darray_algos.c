#include <lcthw/darray_algos.h>
#include <stdlib.h>
#include <math.h>


void swap(DArray* array, int a, int b)
{
  void* tmp = DArray_get(array, a);
  DArray_set(array, a, DArray_get(array, b));
  DArray_set(array, b, tmp);
}

int partition(DArray* array, int lo, int hi, DArray_compare cmp)
{
  void* pivot = DArray_get(array, hi);
  int j, i = lo - 1;
  for(j = lo; j < hi; j++) {
    if (cmp(&array->contents[j], &pivot) <= 0) {
      i++;
      if (i != j) {
	swap(array, i, j);
      }
    }
  }

  swap(array, i + 1, hi);
  return i + 1;
}

void quicksort(DArray* array, int lo, int hi, DArray_compare cmp)
{
  if (lo < hi) {
    int p = partition(array, lo, hi, cmp);
    quicksort(array, lo, p - 1, cmp);
    quicksort(array, p + 1, hi, cmp);
  }
}

int DArray_qsort(DArray* array, DArray_compare cmp)
{
  //qsort(array->contents, DArray_count(array), sizeof(void *), cmp);
  quicksort(array, 0, DArray_end(array) -1, cmp);
  return 0;
}

/********************************************************************************/

int i_parent(int i)
{
  return (floor((i - 1) / 2));
}

int i_left_child(int i)
{
  return ((2 * i) + 1);
}

int i_right_child(int i)
{
  return ((2 * i) + 2);
}

/* 
 * Repair the heap whose root element is at index start, assuming the
 * heaps rooted at its children are valid.
 *
 * base: the array to be sorted.
 * start: the root.
 * end: the last element in array.
 */
void sift_down(DArray* array, int start, int end, DArray_compare cmp)
{
  int root = start;

  while (i_left_child(root)  <= end) {
    int child = i_left_child(root);
    int swap = root;

    if (cmp(&array->contents[swap], &array->contents[child]) < 0) {
      swap = child;
    }
    /* If there is a right child and that child is greater */
    if ((child + 1 <= end) &&
        (cmp(&array->contents[swap], &array->contents[child + 1]) < 0)) {
      swap = child + 1;
    }
    /* If root holds the largest element we are done! */
    if (swap == root) {
      return;
    } else {
      DArray_swap(array, root, swap);
      root = swap;
    }
  }
}

void heapify(DArray* array, DArray_compare cmp)
{
  int start = i_parent(DArray_count(array) - 1);

  while (start >= 0) {
    /* sift down the node at index start to the proper place such that all nodes */
    /* below the start index are in the heap order                               */
    sift_down(array, start, DArray_count(array) - 1, cmp);
    /* go to the next parent node */
    start -= 1;
  }
}

int DArray_heapsort(DArray* array, DArray_compare cmp)
{
  heapify(array, cmp);

  int end = DArray_count(array) - 1;
  while (end > 0) {
    DArray_swap(array, end, 0);
    end--;
    sift_down(array, 0, end, cmp);
  }
 
  return 0;
}

/********************************************************************************/

void top_down_merge(DArray* a, int begin, int middle, int end, DArray* b, DArray_compare cmp)
{
  int i = begin, j = middle, k;

  for(k = begin; k < end; k++) {
    if (i < middle && ((j >= end) || (cmp(&a->contents[i], &a->contents[j]) <= 0))) {
      b->contents[k] = a->contents[i];
      i++;
    } else {
      b->contents[k] = a->contents[j];
      j++;
    }
  }
}

void top_down_split_merge(DArray* b, int begin, int end, DArray* a, DArray_compare cmp)
{
  if ((end - begin) < 2) {
    return;
  }
  int middle = (begin + end) / 2;
  top_down_split_merge(a, begin, middle, b, cmp);
  top_down_split_merge(a, middle, end, b, cmp);
  top_down_merge(b, begin, middle, end, a, cmp);
}

void top_down_merge_sort(DArray* a, DArray* b, DArray_compare cmp)
{
  DArray_copy(a, b);
  top_down_split_merge(b, 0, DArray_end(b), a, cmp);
}

int DArray_mergesort(DArray* array, DArray_compare cmp)
{
  DArray* brray = DArray_create(array->element_size, array->max);
  top_down_merge_sort(array, brray, cmp);
  DArray_clear_destroy(brray);
  return 0;
}

void DArray_sort_add(DArray* array, void* el, DArray_compare cmp)
{
  DArray_push(array, el);
  DArray_qsort(array, cmp);
}

char* DArray_find(DArray* array, char* el, DArray_compare cmp)
{
  int low = 0;
  int high = array->end - 1;

  while (low <= high) {
    int mid = low + (high - low) / 2;
    char* key = array->contents[mid];

    if (cmp(&el, &key) < 0) {
      high = mid - 1;
    } else if (cmp(&el, &key) > 0) {
      low = mid + 1;
    } else {
      return array->contents[mid];
    }
  }

  return NULL;
}

/* no heapsort or mergesort in stdlib?
int DArray_heapsort(DArray* array, DArray_compare cmp)
{
  heapsort(array->contents, DArray_count(array), sizeof(void *), cmp);
  return 0;
}

int DArray_mergesort(DArray* array, DArray_compare cmp)
{
  mergesort(array->contents, DArray_count(array), sizeof(void *), cmp);
  return 0;
}
*/
