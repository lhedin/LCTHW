#include "minunit.h"
#include <lcthw/ringbuffer.h>
#include <assert.h>

static RingBuffer* buffer = NULL;
char* test1 = "hello";
char* test2 = "hi";
char* test3 = "this is a long string";
char* test4 = "max";

#define BUFFER_LEN 10
#define TEST1_LEN 5
#define TEST2_LEN 2
#define TEST3_LEN 21
#define TEST4_LEN 3

char* test_ringbuffer_create()
{
    buffer = RingBuffer_create(BUFFER_LEN);
    mu_assert(buffer != NULL, "Failed to create buffer.");

    return NULL;
}

char* test_ringbuffer_destroy()
{
    mu_assert(buffer != NULL, "No buffer for destroy.");
    RingBuffer_destroy(buffer);

    return NULL;
}

char* test_ringbuffer_write_ok()
{
    int result = 0;

    result = RingBuffer_write(buffer, test1, TEST1_LEN);
    mu_assert(result == TEST1_LEN, "Test1 not written to buffer.");
    mu_assert(RingBuffer_available_data(buffer) == TEST1_LEN,
              "Available data for test1 is not correct.");
    mu_assert(RingBuffer_available_space(buffer) == (BUFFER_LEN - TEST1_LEN),
              "Available space for test1 is not correct.");

    //RingBuffer_print(buffer);
    return NULL;
}

char* test_ringbuffer_write_nok()
{
    int result = 0;

    result = RingBuffer_write(buffer, test3, TEST3_LEN);
    mu_assert(result == -1, "Test3 was written to buffer when it shouldn't.");
    mu_assert(RingBuffer_available_data(buffer) == TEST1_LEN,
              "Available data for test3 is not correct.");
    mu_assert(RingBuffer_available_space(buffer) == (BUFFER_LEN - TEST1_LEN),
              "Available space for test3 is not correct.");
    
    //RingBuffer_print(buffer);
    return NULL;
}

char* test_ringbuffer_write_full_ok()
{
    int result = 0;

    result = RingBuffer_write(buffer, test2, TEST2_LEN);
    mu_assert(result == TEST2_LEN, "Test2 not written to buffer.");
    mu_assert(RingBuffer_available_data(buffer) == (TEST1_LEN + TEST2_LEN),
              "Available data after test2 is not correct.");
    printf("avail space %d\n", RingBuffer_available_space(buffer));
    mu_assert(RingBuffer_available_space(buffer) == (BUFFER_LEN - TEST1_LEN - TEST2_LEN),
              "Available space after test2 is not correct.");

    //RingBuffer_print(buffer);

    result = RingBuffer_write(buffer, test4, TEST4_LEN);
    mu_assert(result == TEST4_LEN, "Test4 not written to buffer.");
    mu_assert(RingBuffer_available_data(buffer) == (TEST1_LEN + TEST2_LEN + TEST4_LEN),
              "Available data after test4 is not correct.");
    mu_assert(RingBuffer_available_space(buffer) == 0,
              "Available space after test4 is not correct.");

    //RingBuffer_print(buffer);

    return NULL;
}

char* test_ringbuffer_read()
{
    char target[BUFFER_LEN] = {0};
    char str[BUFFER_LEN] = {0};

    strcpy(str, test1);
    strcat(str, test2);
    strcat(str, test4);

    //result = RingBuffer_gets(buffer, 2);
    int result = RingBuffer_read(buffer, target, BUFFER_LEN);
    mu_assert(result == BUFFER_LEN, "Result not correct for RingBuffer read");
    mu_assert(strncmp(target, str, BUFFER_LEN), "String is incorrect after RingBuffer_gets");
    mu_assert(RingBuffer_available_data(buffer) == 0, "Available data not as expected");
    mu_assert(RingBuffer_available_space(buffer) == BUFFER_LEN,
              "Available space not as expected");
    mu_assert(RingBuffer_starts_at(buffer) == RingBuffer_ends_at(buffer),
              "RingBuffer start and end should be at same position");

    return NULL;
}



char* all_tests()
{
    mu_suite_start();

    mu_run_test(test_ringbuffer_create);
    mu_run_test(test_ringbuffer_write_ok);
    mu_run_test(test_ringbuffer_write_nok);
    mu_run_test(test_ringbuffer_write_full_ok);
    mu_run_test(test_ringbuffer_destroy);
    mu_run_test(test_ringbuffer_read);

    return NULL;
}

RUN_TESTS(all_tests);