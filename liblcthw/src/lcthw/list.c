#include <lcthw/list.h>
#include <lcthw/dbg.h>
#include <assert.h>

void List_print(List* list)
{
  ListNode* N = NULL;
  printf("List (%d): ", list->count);
  for(N = list->first; N != NULL; N = N->next) {
    printf(": %s :", (char*)N->value);
  }
  printf("\n");
}

List* List_create()
{
  List* list = calloc(1, sizeof(List));
  assert(list != NULL);

  return list;
}

void List_destroy(List* list)
{
  LIST_FOREACH(list, first, next, cur) {
    if(cur->prev) {
      free(cur->prev);
    }
  }
 
  free(list->last);
  free(list);
}

void List_clear(List* list)
{
  assert(list != NULL);
  LIST_FOREACH(list, first, next, cur) {
    cur->value = NULL; // according to instruction free(cur->value);
  }
}

void List_clear_destroy(List* list)
{
  List_clear(list);
  List_destroy(list);
}

/* Add element to end of list. */
void List_push(List* list, void* value)
{
  assert(list != NULL);
  ListNode* node = calloc(1, sizeof(ListNode));
  check_mem(node);
  
  node->value = value;

  if(list->last == NULL) {
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    node->prev = list->last;
    list->last = node;
  }
  
  list->count++;

 error:
  return;
}

/* Remove last element in list. */
void* List_pop(List* list)
{
  ListNode* node = list->last;
  return node != NULL ? List_remove(list, node) : NULL;
}

/* Add an element to beginning of list. */
void List_unshift(List* list, void* value)
{
  assert(list != NULL);
  ListNode* node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;

  if (list->first == NULL) {
    list->first = node;
    list->last = node;
  } else {
    node->next = list->first;
    list->first->prev = node;
    list->first = node;
  }
  list->count++;

 error:
  return;
}

/* Remove first element in list. */
void* List_shift(List* list)
{
  ListNode* node = list->first;
  return node != NULL ? List_remove(list, node) : NULL;
}

void* List_remove(List* list, ListNode* node)
{
  void* result = NULL;

  check(list->first && list->last, "List is empty.");
  check(node, "node can't be NULL.");

  if (node == list->first && node == list->last) {
    list->first = NULL;
    list->last = NULL;
  } else if (node == list->first) {
    list->first = node->next;
    check(list->first != NULL,
	  "Invalid list, somehow got a first that is NULL.");
    list->first->prev = NULL;
  } else if (node == list->last) {
    list->last = node->prev;
    check(list->last != NULL,
	  "Invalid list, somehow got a next that is NULL.");
    list->last->next = NULL;
  } else {
    ListNode* after = node->next;
    ListNode* before = node->prev;
    after->prev = before;
    before->next = after;
  }

  list->count--;
  result = node->value;
  free(node);

 error:
  return result;
}

List* List_copy(List* list)
{
  check(list, "Incomming list is empty!");

  List* new_list = List_create(); 
  ListNode* N = NULL;

  for(N = list->first; N != NULL; N = N->next) {
    List_push(new_list, N->value);
  }

  return new_list;

 error:
  return NULL;
}

List* List_split(List* list, void* value)
{
  check(list, "No list received!");

  List*     new_list = NULL;
  ListNode* N = list->first;
  int       a = 1;

  while(N != NULL && N->value != value) {
    N = N->next;
    a++;
  }
  if (!N) goto error;
  if (!N->next) goto error;

  new_list = List_create();
  new_list->first = N->next;
  new_list->last = list->last;
  new_list->count = list->count - a;
  N->next = NULL;
  list->last = N;
  list->count = a;

  return new_list;

 error:
  return NULL;
}

void List_join(List* list1, List* list2)
{
  check(list1, "Incomming list is empty!");
  check(list2, "Incomming list is empty!");

  list1->last->next = list2->first;
  list2->first->prev = list1->last;
  list1->last = list2->last;
  list1->count = list1->count + list2->count;
  list2->first = NULL;
  list2->last = NULL;
  list2->count = 0;

 error:
  return;
}

void ListNode_swap(ListNode* a, ListNode* b)
{
  void* tmp = a->value;
  a->value = b->value;
  b->value = tmp;
}
