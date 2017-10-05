#include <lcthw/darray.h>
#include <assert.h>

DArray* DArray_create(size_t element_size, size_t initial_max)
{
  DArray* array = malloc(sizeof(DArray));
  check_mem(array);
  array->max = initial_max;
  check(array->max > 0, "you must set an initial_max > 0.");

  array->contents = calloc(initial_max, sizeof(void*));
  check_mem(array->contents);

  array->end = 0;
  array->element_size = element_size;
  array->expand_rate = DEFAULT_EXPAND_RATE;

  return array;

 error:
  DArray_destroy(array);
  return NULL;
}

void DArray_clear(DArray* array)
{
  int i = 0;
  if (array->element_size > 0) {
    for (i = 0; i < array->max; i++) {
      if (array->contents[i] != NULL) {
	free(array->contents[i]);
      }
    }
  }
}

static inline int DArray_resize(DArray* array, size_t newsize)
{
  array->max = newsize;
  check(array->max > 0, "the newsize must be > 0.");

  void* contents = realloc(array->contents, array->max * sizeof(void *));
  // check contents and assume realloc doesn't harm the original on error
  check_mem(contents);
  array->contents = contents;

  return 0;

 error:
  return -1;
}

int DArray_expand(DArray* array)
{
  size_t old_max = array->max;
  check(DArray_resize(array, array->max + array->expand_rate) == 0,
	"failed to expand array to new size: %d",
	array->max + (int)array->expand_rate);

  memset(array->contents + old_max, 0, array->expand_rate + 1);
  return 0;

 error:
  return -1;
}

int DArray_contract(DArray* array)
{
  int new_size = array->end < (int)array->expand_rate ?
    (int)array->expand_rate : array->end;

  return DArray_resize(array, new_size + 1);
}

void DArray_destroy(DArray* array)
{
  if (array) {
    if (array->contents)
      free(array->contents);
    free(array);
  }
}

void DArray_clear_destroy(DArray* array)
{
  DArray_clear(array);
  DArray_destroy(array);
}

/* add to end */
int DArray_push(DArray* array, void* el)
{
  array->contents[array->end] = el;
  array->end++;

  if (DArray_end(array) >= DArray_max(array)) {
    return DArray_expand(array);
  } else {
    return 0;
  }
}

/* remove from end */
void* DArray_pop(DArray* array)
{
  check(array->end - 1 >= 0, "Attempt to pop from empty array.");

  void* el = DArray_remove(array, array->end - 1);
  array->end--;

  if (DArray_end(array) > (int)array->expand_rate &&
      DArray_end(array) % array->expand_rate) {
    DArray_contract(array);
  }

  return el;

 error:
  return NULL;
}

/* Remove from beginning of array. */
void* DArray_shift(DArray* array)
{
  check(array->end - 1 >= 0, "Attempt to shift from empty array.");

  int i = 0;
  void* el = array->contents[0];
  for (i = 0; i < array->end - 1; i++) {
    array->contents[i] = array->contents[i + 1];
  }
  array->contents[array->end - 1] = NULL;
  array->end--;

  return el;

error:
  return NULL;
}

/* Add an element to beginning of array. */
int DArray_unshift(DArray* array, void* el)
{
  int i = 0;

  for (i = array->end; i > 0; i--) {
    array->contents[i] = array->contents[i - 1];
  }
  array->contents[0] = el;
  array->end++;

  if (DArray_end(array) >= DArray_max(array)) {
    return DArray_expand(array);
  } else {
    return 0;
  }
}

void DArray_swap(DArray* array, int i, int j)
{
  check((i < array->end) && (j < array->end), "Trying to swap element > array size!");

  void* tmp = array->contents[i];
  array->contents[i] = array->contents[j];
  array->contents[j] = tmp;

 error:
  return;
}

void DArray_copy(DArray* a, DArray* b)
{
  b->end = a->end;
  b->max = a->max;
  b->element_size = a->element_size;
  b->expand_rate = a->expand_rate;

  int i;
  for (i = 0; i < a->end; i++) {
    b->contents[i] = a->contents[i];
  }
}

void DArray_print(DArray* array, char* msg)
{
  printf("%s\n", msg);
  int i;
  for (i = 0; i < array->end; i++) {
    printf("[%d] %s, %p\n", i, (char*)DArray_get(array, i), DArray_get(array, i));
  }
  printf("\n\n");
}
