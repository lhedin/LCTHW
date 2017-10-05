#ifndef lcthw_queue_h_
#define lcthw_queue_h_

#include <lcthw/darray.h>
#include <lcthw/list.h>

/*-----------------------------------------------------------------*/
/* Queue from DArray                                               */
typedef DArray Queue;

#define Queue_create() DArray_create(0, 3)
#define Queue_destroy DArray_destroy
#define Queue_send DArray_push
#define Queue_recv DArray_shift
#define Queue_peek DArray_first
#define Queue_count DArray_count

#define QUEUE_FOREACH(Q, V) ListNode* V = calloc(1, sizeof(ListNode));\
int _i = 0;\
for(_i = 0, V->value = Q->contents[0];\
    _i < Q->end;\
    (++_i < Q->end) ? V->value = Q->contents[_i] : free(V))

/*-----------------------------------------------------------------*/
/* Queue from List                                                 */
#if 0

typedef List Queue;

#define Queue_create List_create
#define Queue_destroy List_destroy
#define Queue_send List_push
#define Queue_recv List_shift
#define Queue_peek List_first
#define Queue_count List_count

#define QUEUE_FOREACH(Q, V) LIST_FOREACH(Q, first, next, V)

#endif
/*-----------------------------------------------------------------*/

#endif