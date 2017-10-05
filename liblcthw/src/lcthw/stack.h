#ifndef lcthw_stack_h
#define lcthw_stack_h

#include "lcthw/darray.h"
#include "lcthw/list.h"

/*-----------------------------------------------------------------*/
/* Stack from DArray                                               */

typedef DArray Stack;

#define Stack_create() DArray_create(0, 3)
#define Stack_destroy DArray_destroy
#define Stack_push DArray_unshift
#define Stack_pop DArray_shift
#define Stack_peek DArray_first
#define Stack_count DArray_count

#define STACK_FOREACH(S, V) ListNode* V = calloc(1, sizeof(ListNode));\
int _i = 0;\
for(_i = 0, V->value = S->contents[0];\
    _i < S->end;\
    (++_i < S->end) ? V->value = S->contents[_i] : free(V))


/*-----------------------------------------------------------------*/
/* Stack from List                                                 */
#if 0

typedef List Stack; 

#define Stack_create List_create
#define Stack_destroy List_destroy
#define Stack_push List_unshift
#define Stack_pop List_shift
#define Stack_peek List_first
#define Stack_count List_count

#define STACK_FOREACH(S, V) LIST_FOREACH(S, first, next,  V)

#endif
/*-----------------------------------------------------------------*/

#endif