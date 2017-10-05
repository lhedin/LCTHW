#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <lcthw/radixmap.h>
#include <lcthw/dbg.h>

enum opt opt = OPT_0;

void set_optimazation_code(enum opt o)
{
  opt = o;
}

RadixMap* RadixMap_create(size_t max)
{
  RadixMap* map = calloc(1, sizeof(RadixMap));
  check_mem(map);

  map->contents = calloc(max + 1, sizeof(RMElement));
  check_mem(map->contents);

  map->temp = calloc(max + 1, sizeof(RMElement));
  check_mem(map->temp);

  map->max = max;
  map->end = 0;

  return map;
 error:
  return NULL;
}

void RadixMap_destroy(RadixMap* map)
{
  if (map) {
    free(map->contents);
    free(map->temp);
    free(map);
  }
}

#define ByteOf(x,y) (((uint8_t*)x)[(y)])

static inline void radix_sort(short offset, uint64_t max,
			      uint64_t* source, uint64_t* dest)
{
  uint64_t count[256] = {0};
  uint64_t* cp = NULL;
  uint64_t* sp = NULL;
  uint64_t* end = NULL;
  uint64_t s = 0;
  uint64_t c = 0;

  // count occurances of every byte value
  //printf("[%s] max = %"PRIu64"\n", __func__, max);
  //RadixMap_print_element(source, "radix_sort source = ");
  for (sp = source, end = source + max; sp < end; sp++) {
    count[ByteOf(sp, offset)]++;
    //printf("ByteOf(sp=%p, offset) = %"PRIu8" ", sp, ByteOf(sp,offset));
  }
  /*printf("\n");
  printf("[%s] count = ", __func__);
  int i;
  for (i = 0; i < 256; i++) {
    printf(" %d", count[i]);
  }
  printf("\n");*/

  // transform count into index by summing
  // elements and storing into same array
  for (s = 0, cp = count, end = count + 256; cp < end; cp++) {
    c = *cp;
    *cp = s;
    s += c;
  }
  /*printf("[%s] count = ", __func__);
  for (i = 0; i < 256; i++) {
    printf(" %d", count[i]);
  }
  printf("\n");*/


  // fill dest with the right values in the right place
  for (sp = source, end = source + max; sp < end; sp++) {
    cp = count + ByteOf(sp, offset);
    dest[*cp] = *sp;
    ++(*cp);
  }
}

void RadixMap_sort(RadixMap* map, int from_pos)
{
  //printf("[%s] from_pos = %d\n", __func__, from_pos);
  uint64_t* source = &map->contents[from_pos].raw;
  uint64_t* temp = &map->temp[from_pos].raw;
  
  //RadixMap_print(map, "FIRST");
  radix_sort(0, map->end - from_pos, source, temp);
  //RadixMap_print(map, "SECOND");
  radix_sort(1, map->end - from_pos, temp, source);
  //RadixMap_print(map, "THIRD");
  radix_sort(2, map->end - from_pos, source, temp);
  //RadixMap_print(map, "FOURTH");
  radix_sort(3, map->end - from_pos, temp, source);
}

RMElement* RadixMap_find(RadixMap* map, uint32_t to_find)
{
  int low = 0;
  int high = map->end - 1;
  RMElement* data = map->contents;

  while (low <= high) {
    int middle = low + (high - low) / 2;
    uint32_t key = data[middle].data.key;
    
    if (to_find < key) {
      high = middle - 1;
    } else if (to_find > key) {
      low = middle + 1;
    } else {
      return &data[middle];
    }
  }
  
  return NULL;
}

int RadixMap_find_min_pos(RadixMap* map, uint32_t to_find)
{
  int low = 0;
  int pos = 0;
  int high = map->end -1;
  RMElement* data = map->contents;

  while (low <= high) {
    int middle = low + (high - low) / 2;
    pos = middle;
    uint32_t key = data[middle].data.key;

    if (to_find < key) {
      high = middle - 1;
      pos = high >= 0 ? high : 0;
    } else if (to_find > key) {
      low = middle + 1;
      pos = low;
    } else {
      return pos;
    }
  }
  return pos;
}

int RadixMap_add(RadixMap* map, uint32_t key, uint32_t value)
{
  check(key < UINT32_MAX, "Key can't be equal to UINT32_MAX.");

  RMElement element = {.data = {.key = key, .value = value}};
  check(map->end + 1 < map->max, "RadixMap is full.");

  map->contents[map->end++] = element;

  int pos;
  switch(opt) {
  case OPT_0:
    pos = 0;
    break;
  case OPT_1:
    pos = RadixMap_find_min_pos(map, element.data.key);
    break;
  case OPT_2:
    //todo
    break;
  default:
    break;
    // todo add error printout.
  }

  RadixMap_sort(map, pos);

  return 0;

 error:
  return -1;
}

int RadixMap_delete(RadixMap* map, RMElement* el)
{
  check(map->end > 0, "There is nothing to delete.");
  check(el != NULL, "Can't delete a NULL element.");

  el->data.key = UINT32_MAX;
  
  if (map->end > 1) {
    // don't bother resorting a map of 1 length
    RadixMap_sort(map, 0);
  }

  map->end--;

  return 0;

 error:
  return -1;
}

#define B2BIN_STR "%c%c%c%c%c%c%c%c "
#define B2BIN(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

void RadixMap_print_element(uint64_t* elem, char* msg)
{
  printf("%s\n", msg);
  printf("raw = %"PRIu64" %p \n", *elem, elem);

  printf("RAW = ");
  int j;
  for (j = 7; j >= 0; j--) {
    printf(B2BIN_STR, B2BIN(*elem >> (j * 8)));
  }
  printf("\n");
}

void RadixMap_print(RadixMap* map, char* msg)
{
  printf("[%s] %s\n", __func__, msg);

  int i;
  for (i = 0; i < (int)map->max; i++) {
    printf("cont [%d] raw = %"PRIu64" %p key = %"PRIu32" %p value = %"PRIu32" %p \n", i,
	   map->contents[i].raw, &map->contents[i].raw,
	   map->contents[i].data.key, &map->contents[i].data.key,
	   map->contents[i].data.value, &map->contents[i].data.value);
    int j;
    printf("RAW CONT = ");
    for (j = 7; j >= 0; j--) {
      printf(B2BIN_STR, B2BIN(map->contents[i].raw >> (j * 8)));
    }
    printf("\n");
    printf("temp [%d] raw = %"PRIu64" %p key = %"PRIu32" %p value = %"PRIu32" %p\n", i,
	   map->temp[i].raw, &map->temp[i].raw,
	   map->temp[i].data.key, &map->temp[i].data.key,
    	   map->temp[i].data.value, &map->temp[i].data.value);
    printf("RAW TEMP = ");
    for (j = 7; j >= 0; j--) {
      printf(B2BIN_STR, B2BIN(map->temp[i].raw >> (j * 8)));
    }
    printf("\n");
  }
  printf("\n\n");
}
