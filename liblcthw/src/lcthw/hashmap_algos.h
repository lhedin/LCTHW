#ifndef hashmap_algos_h
#define hashmap_algos_h

#include <stdint.h>

uint32_t Hashmap_fnv1a_hash(void* data);

uint32_t Hashmap_adler32_hash(void* data);

uint32_t Hashmap_djb_hash(void* data);

uint32_t Hashmap_bad_hash(void* data);

uint32_t Hashmap_default_hash(void* a);

#endif
