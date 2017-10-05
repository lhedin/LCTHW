#include "minunit.h"
#include <lcthw/list.h>
#include <assert.h>

static List* list = NULL;
char* test1 = "test1 data";
char* test2 = "test2 data";
char* test3 = "test3 data";
char* test4 = "test4 data";
char* test5 = "test5 data";
char* test6 = "test6 data";

char* test_create()
{
  list = List_create();
  mu_assert(list != NULL, "Failed to create list.");

  return NULL;
}

char* test_destroy()
{
  List_clear_destroy(list);

  return NULL;
}

char* test_push_pop()
{
  List_push(list, test1);
  mu_assert(List_last(list) == test1, "Wrong last value.");

  List_push(list, test2);
  mu_assert(List_last(list) == test2, "Wrong last value.");

  List_push(list, test3);
  mu_assert(List_last(list) == test3, "Wrong last value.");
  mu_assert(List_count(list) == 3, "Wrong count on push.");

  char* val = List_pop(list);
  mu_assert(val == test3, "Wrong value on pop.");

  val = List_pop(list);
  mu_assert(val == test2, "Wrong value on pop.");

  val = List_pop(list);
  mu_assert(val == test1, "Wrong value on pop.");
  mu_assert(List_count(list) == 0, "Wrong count after pop.");

  return NULL;
}

char* test_unshift()
{
  List_unshift(list, test1);
  mu_assert(List_first(list) == test1, "Wrong first value.");

  List_unshift(list, test2);
  mu_assert(List_first(list) == test2, "Wrong first value.");

  List_unshift(list, test3);
  mu_assert(List_first(list) == test3, "Wrong first value.");
  mu_assert(List_count(list) == 3, "Wrong count on unshift.");

  return NULL;
}

char* test_remove()
{
  // We only need to test the middle remove case since push/shift
  // already tests the other cases.
  char* val = List_remove(list, list->first->next);
  mu_assert(val == test2, "Wrong removed element.");
  mu_assert(List_count(list) == 2, "Wrong count after remove.");
  mu_assert(List_first(list) == test3, "Wrong first after remove.");
  mu_assert(List_last(list) == test1, "Wrong last after remove.");

  return NULL;
}

char* test_shift()
{
  mu_assert(List_count(list) != 0, "Wrong count before shift.");

  char* val = List_shift(list);
  mu_assert(val == test3, "Wrong value on shift.");
  
  val = List_shift(list);
  mu_assert(val == test1, "Wrong value on shift.");
  mu_assert(List_count(list) == 0, "Wrong count after shift.");

  return NULL;
}

char* test_copy()
{
  mu_assert(List_count(list) == 0, "Wrong count before copy.");

  List_push(list, test1);
  List_push(list, test2);

  List* list_cp = List_copy(list);
  mu_assert(List_count(list_cp) == 2, "Wrong list count after copy");
 
  char* val = List_pop(list_cp);
  mu_assert(val == test2, "Wrong value on pop on copy");

  val = (char*)List_pop(list_cp);
  mu_assert(val == test1, "Wrong value on pop on copy");

  List_clear_destroy(list_cp);

  return NULL;
}

char* test_split()
{
  mu_assert(List_count(list) == 2, "Wrong count before split.");

  List_push(list, test3);
  List_push(list, test4);
  List_push(list, test5);

  List* list1 = List_split(list, "should not be found");
  mu_assert(!list1, "Wrong list after split, should return NULL!");
  
  List* list2 = List_split(list, "test3 data");

  mu_assert(List_count(list) == 3, "Wrong list count after split. A");
  mu_assert(List_count(list2) == 2, "Wrong list count after split. B");

  List_clear_destroy(list2);
  
  return NULL;
}

char* test_join()
{
  mu_assert(List_count(list) == 3, "Wrong count before split.");

  List* lista = List_create();
  List_push(lista, test6);

  List_join(list, lista);
  mu_assert(List_count(list) == 4, "Wrong list count after join.");

  List_clear_destroy(lista);

  return NULL;
}

char* all_tests()
{
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_push_pop);
  mu_run_test(test_unshift);
  mu_run_test(test_remove);
  mu_run_test(test_shift);
  mu_run_test(test_copy);
  mu_run_test(test_split);
  mu_run_test(test_join);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
