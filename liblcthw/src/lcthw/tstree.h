#ifndef _lcthw_TSTree_h
#define _lcthw_TSTree_h

#include <stdlib.h>
#include <lcthw/darray.h>

typedef struct TSTree {
    char splitchar;
    struct TSTree* low;
    struct TSTree* equal;
    struct TSTree* high;
    void* value;
} TSTree;

void* TSTree_search(TSTree* root, const char* key, size_t len);
void* TSTree_search_prefix(TSTree* root, const char* key, size_t len);
void  TSTree_search_suffix(TSTree* root, const char* key, size_t len, DArray* array);

typedef void (*TSTree_traverse_cb) (void* item, void* data);

TSTree* TSTree_insert(TSTree* node, const char* key, size_t len, void* value);
void TSTree_traverse(TSTree* node, TSTree_traverse_cb cb, void* data);
void TSTree_collect(TSTree* root, const char* key, size_t len, DArray* array);

void TSTree_destroy(TSTree* root);

#endif