#include <stdbool.h>
#include <lcthw/dbg.h>
#include <lcthw/list_algos.h>

int List_bubble_sort(List* list, List_compare cmp)
{
  int       result;
  ListNode* N = NULL;
  bool      swapped = true;

  if (list->count <= 1) {
    return 0;
  }

  //printf("Unsorted list: ");
  //List_print(list);

  while(swapped) {
    swapped = false;
    for (N = list->first; N->next != NULL; N = N->next) {
      result = cmp(N->value, N->next->value);
      if (result > 0) {
	      ListNode_swap(N, N->next);
	      swapped = true;
      }

      if (list->last == N) {
	      break;
      }
    }
  }

  //printf("Sorted list:   ");
  //List_print(list);

  return 0;
}

List* List_merge(List* left, List* right, List_compare cmp)
{
  List* result = List_create();

  void* val = NULL;

  while (List_count(left) > 0 || List_count(right) > 0) {
    if (List_count(left) > 0 && List_count(right) > 0) {
      if (cmp(List_first(left), List_first(right)) <= 0) {
	      val = List_shift(left);
      } else {
	      val = List_shift(right);
      }

      List_push(result, val);
    } else if (List_count(left) > 0) {
      val = List_shift(left);
      List_push(result, val);
    } else if (List_count(right) > 0) {
      val = List_shift(right);
      List_push(result, val);
    }
  }

  return result;
}

List* List_merge_sort(List* list, List_compare cmp)
{
  List* result = NULL;

  if (List_count(list) <= 1) {
    return list;
  }

  List* left = List_create();
  List* right = List_create();
  int middle = List_count(list) / 2;

  ListNode* N = NULL;
  for(N = list->first; N != NULL; N = N->next) {
    if (middle > 0) {
      List_push(left, N->value);
    } else {
      List_push(right, N->value);
    }
    middle--;
  }

  List* sort_left = List_merge_sort(left, cmp);
  List* sort_right = List_merge_sort(right, cmp);

  if (sort_left != left) {
    List_destroy(left);
  }
  if (sort_right != right) {
    List_destroy(right);
  }

  result = List_merge(sort_left, sort_right, cmp);

  List_destroy(sort_left);
  List_destroy(sort_right);

  //printf("list = ");
  //List_print(list);
  //printf("result = ");
  //List_print(result);

  return result;
}
