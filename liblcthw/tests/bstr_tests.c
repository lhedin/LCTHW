#include "minunit.h"
#include <lcthw/bstrlib.h>

/* bfromcstr - Create a bstring from a C style constant. */
char* test_bfromcstr()
{
  char* cstr = "hello";
  int cstrlen = 5;

  bstring bstr = bfromcstr(cstr);
  
  mu_assert(bstr->mlen >= bstr->slen, "mlen incorrect!");
  mu_assert(bstr->slen == cstrlen, "slen incorrect!");
  mu_assert(strcmp((char*)bstr->data, cstr) == 0, "data incorrect!");
  
  bdestroy(bstr);

  return NULL;
}

/* blk2bstr - Do the same thing as bfromcstr, but give the length of the buffer. */
char* test_blk2bstr()
{
  void* blk = "hello";
  int len = 5;

  bstring bstr = blk2bstr(blk, len);

  mu_assert(bstr->mlen >= bstr->slen, "mlen incorrect!");
  mu_assert(bstr->slen == len, "slen incorrect!");
  mu_assert(strcmp((char*)bstr->data, (char*) blk) == 0, "data incorrect!");

  bdestroy(bstr);

  return NULL;
}

/* bstrcpy - Copy a bstring. */
char* test_bstrcpy()
{
  bstring bstr1 = bfromcstr("hello");
  bstring bstr2 = bstrcpy(bstr1);

  mu_assert(bstrcmp(bstr1, bstr2) == 0, "bstrings not equal!");

  bdestroy(bstr1);
  bdestroy(bstr2);

  return NULL;
}

/* bassign - Set one bstring to another. */
char* test_bassign()
{
  char* cstr = "hello";

  bstring bstr1 = bfromcstr(cstr);
  bstring bstr2 = NULL;

  mu_assert(bassign(bstr1, bstr2) == BSTR_ERR, "Should not accept a NULL string!");

  bstr2 = bfromcstr("hello world");

  mu_assert(bassign(bstr1, bstr2) == BSTR_OK, "bassign failed!");
  mu_assert(bstr1->slen == bstr2->slen, "slen not equal!");
  mu_assert(bstr1->mlen == bstr2->mlen, "mlen not equal!");
  mu_assert(strcmp((char*)bstr1->data, (char*)bstr2->data) == 0, "data not equal!");

  bdestroy(bstr1);
  bdestroy(bstr2);

  return NULL;
}

/* bassigncstr - Set a bstring to a C string’s contents. */
char* test_bassigncstr()
{
  bstring bstr = bfromcstr("hello");

  mu_assert(bassigncstr(bstr, "world") == BSTR_OK, "bassigncstr failed!");
  mu_assert(bstr->slen == 5, "slen not correct!");
  mu_assert(bstr->mlen >= bstr->slen, "mlen not correct!");

  bdestroy(bstr);

  return NULL;
}

/* bassignblk - Set a bstring to a C string but give the length. */
char* test_bassignblk()
{
  bstring bstr = bfromcstr("o");
  
  mu_assert(bassignblk(bstr, "hello", 5) == BSTR_OK, "bassignblk failed!");
  mu_assert(bstr->slen == 5, "slen not correct!");
  mu_assert(bstr->mlen >= bstr->slen, "mlen not correct!");
  
  mu_assert(bassignblk(bstr, "hi", 2) == BSTR_OK, "bassignblk failed!");
  mu_assert(bstr->slen == 2, "slen not correct!");
  mu_assert(bstr->mlen >= bstr->slen, "mlen not correct!");
  
  bdestroy(bstr);

  return NULL;
}

/* bdestroy - Destroy a bstring. */
char* test_bdestroy()
{
  bstring bstr = NULL;

  mu_assert(bdestroy(bstr) == BSTR_ERR, "bstring free'ed but shouldn't be!");

  return NULL;
}

/* bconcat - Concatenate one bstring onto another. */
char* test_bconcat()
{
  bstring bstr1 = bfromcstr("Hello, ");
  bstring bstr2 = bfromcstr("World!");

  mu_assert(bconcat(bstr1, bstr2) == BSTR_OK, "bconcat failed!");
  mu_assert(strcmp((char*)bstr1->data, "Hello, World!") == 0, "String not correct!");

  bdestroy(bstr1);
  bdestroy(bstr2);

  return NULL;
}

/* bstricmp - Compare two bstrings returning the same result as strcmp. */
char* test_bstricmp()
{
  bstring bstr1 = bfromcstr("hello");
  bstring bstr2 = bfromcstr("hello");

  mu_assert(bstricmp(bstr1, bstr2) == 0, "bstricmp failed!");

  bdestroy(bstr1);
  bdestroy(bstr2);

  return NULL;
}

/* biseq - Tests if two bstrings are equal. */
char* test_biseq()
{
  bstring bstr1 = bfromcstr("hello");
  bstring bstr2 = bfromcstr("hello");

  mu_assert(biseq(bstr1, bstr2) == 1, "biseq failed!");

  bdestroy(bstr1);
  bdestroy(bstr2);

  return NULL;
}

/* binstr - Tells if one bstring is in another. */
char* test_binstr()
{
  bstring bstr1 = bfromcstr("hello world!");
  bstring bstr2 = bfromcstr("llo");

  mu_assert(binstr(bstr1, 0, bstr2) == 2, "binstr failed!");
  mu_assert(binstr(bstr1, 5, bstr2) == BSTR_ERR, "binstr failed!");

  bdestroy(bstr1);
  bdestroy(bstr2);

  return NULL;
}

/* bfindreplace - Find one bstring in another, then replace it with a third. */
char* test_bfindreplace()
{
  bstring bstr1 = bfromcstr("hello world!");
  bstring bstr2 = bfromcstr("llo");
  bstring bstr3 = bfromcstr("ro");

  mu_assert(bfindreplace(bstr1, bstr2, bstr3, 0) == BSTR_OK, "bfindreplace failed!");
  mu_assert(strcmp((char*)bstr1->data, "hero world!") == 0, "New string not as expected!");

  bdestroy(bstr1);
  bdestroy(bstr2);
  bdestroy(bstr3);

  return NULL;
}

/* bsplit - Split a bstring into a bstrList. */
char* test_bsplit()
{
  bstring bstr = bfromcstr("should be splitting this line of text");
  unsigned char splitchar = ' ';
  
  struct bstrList* sl = bsplit(bstr, splitchar);
  mu_assert(sl->qty == 7, "No of entries is not correct in bstrList!");
  mu_assert(strcmp((char*)sl->entry[0]->data, "should") == 0, "First entry not correct in bstrList!");

  bdestroy(bstr);
  bstrListDestroy(sl);

  return NULL;
}


/* bformat - Do a format string, which is super handy. */
char* test_bformat()
{
  bstring bstr = bformat("Fiat %d", 500);

  mu_assert(strcmp((char*)bstr->data, "Fiat 500") == 0, "bformat failed!");
  mu_assert(bstr->slen == 8, "slen not correct!");
  mu_assert(bstr->mlen >= bstr->slen, "mlen not correct!");

  bdestroy(bstr);

  return NULL;
}

/* blength - Get the length of a bstring. */
char* test_blength()
{
  bstring bstr = bfromcstr("hello");

  mu_assert(blength(bstr) == bstr->slen, "blength failed!");

  bdestroy(bstr);

  return NULL;
}

/* bdata - Get the data from a bstring. */
char* test_bdata()
{
  bstring bstr = bfromcstr("hello");
  char* data = bdata(bstr);

  mu_assert(strcmp(data, (char*)bstr->data) == 0, "bdata failed!");

  bdestroy(bstr);
  return NULL;
}

/* bchar - Get a char from a bstring. */
char* test_bchar()
{
  bstring bstr = bfromcstr("hello");
  char c = (char)bchar(bstr, 4);

  mu_assert(c == 'o', "bchar failed!");

  bdestroy(bstr);
  return NULL;
}

char* all_tests()
{
  mu_suite_start();

  mu_run_test(test_bfromcstr);
  mu_run_test(test_blk2bstr);
  mu_run_test(test_bstrcpy);
  mu_run_test(test_bassign);
  mu_run_test(test_bassigncstr);
  mu_run_test(test_bassignblk);
  mu_run_test(test_bdestroy);
  mu_run_test(test_bconcat);
  mu_run_test(test_bstricmp);
  mu_run_test(test_biseq);
  mu_run_test(test_binstr);
  mu_run_test(test_bfindreplace);
  mu_run_test(test_bsplit);
  mu_run_test(test_bformat);
  mu_run_test(test_blength);
  mu_run_test(test_bdata);
  mu_run_test(test_bchar);

  return NULL;
}

RUN_TESTS(all_tests);
