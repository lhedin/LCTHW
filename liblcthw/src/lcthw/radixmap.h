#ifndef _radixmap_h
#define _radixmap_h

#include <stdint.h>
#include <inttypes.h>
/*
#define OPTS \
  X(OPT_0)   \
  X(OPT_1)   \
  X(OPT_2)

#define X(a) ##a,
enum opt { OPTS };
#undef X
*/

enum opt {
  OPT_0,
  OPT_1,
  OPT_2
};

void set_optimazation_code(enum opt opt);

typedef union RMElement {
  uint64_t raw;
  struct {
    uint32_t key;
    uint32_t value;
  } data;
} RMElement;

typedef struct RadixMap {
  size_t max;
  size_t end;
  uint32_t counter;
  RMElement* contents;
  RMElement* temp;
} RadixMap;

RadixMap* RadixMap_create(size_t max);

void RadixMap_destroy(RadixMap* map);

void RadixMap_sort(RadixMap* map, int from_pos);

RMElement* RadixMap_find(RadixMap* map, uint32_t key);

int RadixMap_add(RadixMap* map, uint32_t key, uint32_t value);

int RadixMap_delete(RadixMap* map, RMElement* el);

#endif
