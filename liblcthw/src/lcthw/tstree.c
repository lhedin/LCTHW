#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <lcthw/dbg.h>
#include <lcthw/tstree.h>
#include <lcthw/darray.h>

static inline TSTree* TSTree_insert_base(TSTree* root, TSTree* node,
    const char* key, size_t len, void* value)
{
    if (node == NULL) {
        node = (TSTree*) calloc(1, sizeof(TSTree));

        if (root == NULL) {
            root = node;
        }

        node->splitchar = *key;
    }

    if (*key < node->splitchar) {
        node->low = TSTree_insert_base(root, node->low, key, len, value);
    } else if (*key == node->splitchar) {
        if (len > 1) {
            node->equal = TSTree_insert_base(root, node->equal,
                key + 1, len - 1, value);
        } else {
            assert(node->value == NULL && "Duplicate insert into TSTree.");
            node->value = value;
        }
    } else {
        node->high = TSTree_insert_base(root, node->high, key, len, value);
    }

    return node;
}

TSTree* TSTree_insert(TSTree* node, const char* key, size_t len, void* value)
{
    return TSTree_insert_base(node, node, key, len, value); 
}

void* TSTree_search(TSTree* root, const char* key, size_t len)
{
    TSTree* node = root;
    size_t i = 0;

    while (i < len && node) {
        if (key[i] < node->splitchar) {
            node = node->low;
        } else if (key[i] == node->splitchar) {
            i++;
            if (i < len)
                node = node->equal;
        } else {
            node = node->high;
        }
    }

    if (node) {
        return node->value;
    } else {
        return NULL;
    }
}

void* TSTree_search_prefix(TSTree* root, const char* key, size_t len)
{
    if (len == 0)
        return NULL;

    TSTree* node = root;
    TSTree* last = NULL;
    size_t i = 0;

    while (i < len && node) {
        if (key[i] < node->splitchar) {
            node = node->low;
        } else if (key[i] == node->splitchar) {
            i++;
            if (i < len) {
                if (node->value)
                    last = node;
                node = node->equal;
            }
        } else {
            node = node->high;
        }
    }

    node = node ? node : last;

    // traverse until we find the first value in the equal chain
    // this is then the first node with this prefix
    while (node && !node->value) {
        node = node->equal;
    }

    return node ? node->value : NULL;
}

/* Extra Credit: Implement a TSTree_collect that returns a DArray
 * containing all of the keys that match the given prefix. */
void TSTree_collect_cb(void* value, void* data)
{
    assert(data != NULL);

    DArray* array = (DArray*)data;
    if (value) {
        DArray_push(array, value);
    }
}

void TSTree_collect(TSTree* root, const char* key, size_t len, DArray* array)
{
    if (len == 0 || !root)
        return;

    TSTree* node = root;
    TSTree* start = NULL;
    size_t i = 0;

    while (i < len && node) {
        if (key[i] < node->splitchar) {
            node = node->low;
        } else if (key[i] == node->splitchar) {
            i++;
            if (i == len)
                start = node;
            node = node->equal;
        } else {
            node = node->high;
        }
    }

    TSTree_traverse(start, TSTree_collect_cb, array);
}

void TSTree_traverse(TSTree* node, TSTree_traverse_cb cb, void* data)
{
    if (!node)
        return;

    if (node->low)
        TSTree_traverse(node->low, cb, data);
    
    if (node->equal)
        TSTree_traverse(node->equal, cb, data);
    
    if (node->high)
        TSTree_traverse(node->high, cb, data);
    
    if (node->value)
        cb(node->value, data);
}

/* Implement TSTree_search_suffix and a TSTree_insert_suffix so you can do suffix
 * searches and inserts. */
void TSTree_traverse_suffix(TSTree* node, const char* key, DArray* array)
{
    if (!node)
        return;

    if (node->low)
        TSTree_traverse_suffix(node->low, key, array);
    
    if (node->equal)
        TSTree_traverse_suffix(node->equal, key, array);
    
    if (node->high)
        TSTree_traverse_suffix(node->high, key, array);
    
    if (node->splitchar == key[0]) {
        DArray_push(array, node);
    }
}

void TSTree_search_suffix(TSTree* root, const char* key, size_t len, DArray* array)
{
    assert(array != NULL);

    if (root == NULL)
        return;

    DArray* node_array = DArray_create(sizeof(TSTree), 10);
    TSTree_traverse_suffix(root, key, node_array);

    TSTree* node;
    void*   value;
    while (DArray_count(node_array) > 0) {
        node = (TSTree*)DArray_pop(node_array);
        value = TSTree_search(node, key, len);
        if (value != NULL)
            DArray_push(array, value);
    }
    DArray_destroy(node_array);
}

void TSTree_destroy(TSTree* node)
{
    if (node == NULL)
        return;
    
    if (node->low)
        TSTree_destroy(node->low);
    
    if (node->equal)
        TSTree_destroy(node->equal);
    
    if (node->high)
        TSTree_destroy(node->high);
    
    free(node);
}