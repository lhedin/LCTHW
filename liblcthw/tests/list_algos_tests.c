#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>
#include <sys/times.h>
#include <stdio.h>
#include <inttypes.h>
#include <time.h>

static clock_t st_time;
static clock_t en_time;
static struct tms st_cpu;
static struct tms en_cpu;

char* values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS"};
int T[6] = {3, 2, 5, 1, 6, 4};

#define NUM_VALUES 5

void start_clock()
{
  st_time = times(&st_cpu);
}

void end_clock_to_file(FILE* fp)
{
  en_time = times(&en_cpu);

  fprintf(fp, "%jd ", (intmax_t)(en_time - st_time));
}

void end_clock(char* msg)
{
  en_time = times(&en_cpu);

  fputs(msg, stdout);
  printf("Real Time: %jd, User Time: %jd, System Time: %jd\n",
	 (intmax_t)(en_time - st_time),
	 (intmax_t)(en_cpu.tms_utime - st_cpu.tms_utime),
	 (intmax_t)(en_cpu.tms_stime - st_cpu.tms_stime));
}

char* create_rnd_word(char* ch, int size)
{
  int wlen = 4, i = 0;
  char* word = malloc(wlen);
  int r = 0;

  for(i = 0; i < wlen; i++) {
    r = rand() % size;
    word[i] = ch[r];
  }

  return word;
}

List* create_rnd_words(int size)
{
  char  lowercase[] = {'a','b','c','d','e','f','g','h','i','j','k',\
                      'l','m','n','o','p','q','r','s','t','u','v',\
                      'w','x','y','z'};
  char  uppercase[] = {'A','B','C','D','E','F','G','H','I','J','K',\
                      'L','M','N','O','P','Q','R','S','T','U','V',\
                      'W','X','Y','Z'};
  char  numbers[]   = {'1','2','3','4','5','6','7','8','9'};
  enum {LOWERCASE, UPPERCASE, NUMBERS} chartype;
  int   i = 0;
  List* words = List_create();
  char* word = NULL;

  for(i = 0; i < size; i++) {
    chartype = rand() % 3;
    switch(chartype) {
    case LOWERCASE:
      word = create_rnd_word(lowercase, 26);
      break;
    case UPPERCASE:
      word = create_rnd_word(uppercase, 26);
      break;
    case NUMBERS:
      word = create_rnd_word(numbers, 9);
      break;
    default:
      break;
    }
    List_push(words, word);
  }
  return words;
}

List* create_words()
{
  int i = 0;
  List* words = List_create();

  for (i = 0; i < NUM_VALUES; i++) {
    List_push(words, values[i]);
  }

  return words;
}

void reset_words(List* words)
{
  int i = 0;
  ListNode* N = NULL;
  for (N = words->first, i = 0; N != NULL; N = N->next, i++) {
    N->value = values[i];
  }
}

int is_sorted(List* words)
{
  LIST_FOREACH(words, first, next, cur) {
    if (cur->next && strcmp(cur->value, cur->next->value) > 0) {
      debug("%s %s", (char*)cur->value, (char*)cur->next->value);
      return 0;
    }
  }

  return 1;
}

char* test_bubble_sort()
{
  List* words = create_words();
 
  // should work on a list that need sorting
  int rc = List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Bubble sort failed.");
  mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");

  // should work on an already sorted list
  rc = List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Bubble sort of already sorted failed.");
  mu_assert(is_sorted(words), "Words should be sort if already bubble sorted.");

  List_destroy(words);

  // should work on an empty list
  words = List_create(words);
  rc = List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Bubble sort failed on empty list.");
  mu_assert(is_sorted(words), "Words should be sorted if empty.");

  List_destroy(words);
  
  return NULL;
}

char* test_merge_sort()
{
  List* words = create_words();

  // should work on a list that need sorting
  List* res = List_merge_sort(words, (List_compare)strcmp);
  mu_assert(is_sorted(res), "Words are not sorted after merge sort.");

  List* res2 = List_merge_sort(res, (List_compare)strcmp);
  mu_assert(is_sorted(res), "Should still be sorted after merge sort.");
  List_destroy(res2);
  List_destroy(res);

  List_destroy(words);
  return NULL;
}

char* test_cmp()
{
  List* words = create_rnd_words(5);
  int i = 0;
  FILE* fp_bubble = fopen("tests/bubble_result.txt", "w");
  FILE* fp_merge = fopen("tests/merge_result.txt", "w");

  srand(time(NULL));

  printf("starting test_cmp\n");
  for(i = 5; i <= 10000; i += 5) {
    printf("i = %d\n", i);
    words = create_rnd_words(i);

    start_clock();
    List_bubble_sort(words, (List_compare)strcmp);
    end_clock_to_file(fp_bubble);
    List_destroy(words);

    words = create_rnd_words(i);
    start_clock();
    List_merge_sort(words, (List_compare)strcmp);
    end_clock_to_file(fp_merge);
    List_destroy(words);
  }

  fclose(fp_bubble);
  fclose(fp_merge);

  return NULL;
}

char* test_cmp_bubble_merge_sort()
{
  int max = 100000;
  int i = 0;
  List* words[max];

  for(i = 0; i < max; i++) {
    words[i] = create_words();
  }

  start_clock();
  for(i = 0; i < max; i++) {
    List_bubble_sort(words[i], (List_compare)strcmp);
  }
  end_clock("Bubble sort: ");
  start_clock();
  for(i = 0; i <max; i++) {
    List_merge_sort(words[i], (List_compare)strcmp);
  }
  end_clock("Merge sort: ");

  for(i = 0; i < max; i++) {
    List_destroy(words[i]);
  }

  return NULL;
}

char* all_tests()
{
  mu_suite_start();

  mu_run_test(test_bubble_sort);
  mu_run_test(test_merge_sort);
  //mu_run_test(test_cmp);
  //mu_run_test(test_cmp_bubble_merge_sort);

  return NULL;
}

RUN_TESTS(all_tests);
