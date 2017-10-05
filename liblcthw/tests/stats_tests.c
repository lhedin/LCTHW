#include "minunit.h"
#include <lcthw/stats.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>

const int NUM_SAMPLES = 10;
double samples[] = {
    6.1061334, 9.6783204, 1.2747090, 8.2395131, 0.3333483,
    6.9755066, 1.0626275, 7.6587523, 4.9382973, 9.5788115
};

Stats expect = {
    .sumsq = 425.1641,
    .sum = 55.84602,
    .min = 0.333,
    .max = 9.678,
    .n = 10,
};

#define MAX_STATS_TABLES 10

// TODO would be nicer with a list or darray here.
typedef struct Stats_table {
    int     count;
    char*   headers[MAX_STATS_TABLES];
    Stats*  stats[MAX_STATS_TABLES];
} Stats_table;

double expect_mean = 5.584602;
double expect_stddev = 3.547868;

#define EQ(X,Y,N) (round((X) * pow(10,N)) == round((Y) * pow(10,N)))

void read_stats_from_file(Stats* st)
{
    FILE* fp = NULL;
    char line[1000] = {0};
    char* token;

    fp = fopen("/home/linda/Programming/LearnCTheHardWay/liblcthw/tests/stats_samples.txt", "r");
    if (fp == NULL) {
        printf("Error %d \n", errno);
    }

    while (fgets(line, sizeof(line), fp))
    {
        token = strtok(line, " \t");
        while(token != NULL) {
            Stats_sample(st, strtof(token, NULL));
            token = strtok(NULL, " \t");
        }
    }

    fclose(fp);
}

void read_stats_table_from_file(Stats_table* table)
{
    FILE* fp = NULL;
    char line[1000] = {0};
    char* token;

    fp = fopen("/home/linda/Programming/LearnCTheHardWay/liblcthw/tests/stats_table_samples.txt", "r");
    if (fp == NULL) {
        printf("Error. Can't open file: %d \n", errno);
    }

    // Get the headings
    fgets(line, sizeof(line), fp);
    token = strtok(line, " \t");
    int i = 0;
    while (token != NULL) {
        strcpy(table->headers[i], token);
        table->count++;
        i++;
        token = strtok(NULL, " \t");
    }

    // TODO check if table->count > MAX_STATS_TABLES
    
    // Get the data
    while (fgets(line, sizeof(line), fp))
    {
        token = strtok(line, " \t");
        for (i = 0; (i < table->count) && (token != NULL); i++) {
            Stats_sample(table->stats[i], strtof(token, NULL));
            token = strtok(NULL, " \t");
        }
    }

    fclose(fp);
}

char* test_operations_from_table_of_data()
{
    Stats_table table;
    int i = 0;

    for (i = 0; i < MAX_STATS_TABLES; i++) {
        table.stats[i] = Stats_create();
        table.headers[i] = malloc(sizeof(char) * 10);
    }
    table.count = 0;
    
    read_stats_table_from_file(&table);

    for (i = 0; i < table.count; i++) {
        printf("%s\n", table.headers[i]);
        Stats_dump(table.stats[i]);
    }

    return NULL;
}

char* test_operations_from_file()
{
    Stats* st = Stats_create();
    mu_assert(st != NULL, "Failed to create stats.");

    read_stats_from_file(st);

    Stats_dump(st);

    mu_assert(EQ(st->sumsq, expect.sumsq, 3), "sumsq not valid.");
    mu_assert(EQ(st->sum, expect.sum, 3), "sum not valid.");
    mu_assert(EQ(st->min, expect.min, 3), "min not valid.");
    mu_assert(EQ(st->max, expect.max, 3), "max not valid.");
    mu_assert(EQ(st->n, expect.n, 3), "n not valid.");
    mu_assert(EQ(expect_mean, Stats_mean(st), 3), "mean not valid.");
    mu_assert(EQ(expect_stddev, Stats_stddev(st), 3), "stddev not valid.");

    return NULL;
}

char* test_operations()
{
    int i = 0;
    Stats* st = Stats_create();
    mu_assert(st != NULL, "Failed to create stats.");

    for (i = 0; i < NUM_SAMPLES; i++) {
        Stats_sample(st, samples[i]);
    }

    Stats_dump(st);

    mu_assert(EQ(st->sumsq, expect.sumsq, 3), "sumsq not valid.");
    mu_assert(EQ(st->sum, expect.sum, 3), "sum not valid.");
    mu_assert(EQ(st->min, expect.min, 3), "min not valid.");
    mu_assert(EQ(st->max, expect.max, 3), "max not valid.");
    mu_assert(EQ(st->n, expect.n, 3), "n not valid.");
    mu_assert(EQ(expect_mean, Stats_mean(st), 3), "mean not valid.");
    mu_assert(EQ(expect_stddev, Stats_stddev(st), 3), "stddev not valid.");

    return NULL;
}

char* test_recreate()
{
    Stats* st = Stats_recreate(expect.sum, expect.sumsq, expect.n,
                               expect.min, expect.max);
    
    mu_assert(st->sum == expect.sum, "sum not equal.");
    mu_assert(st->sumsq == expect.sumsq, "sumsq not equal.");
    mu_assert(st->n == expect.n, "n not equal.");
    mu_assert(st->min == expect.min, "min not equal.");
    mu_assert(st->max == expect.max, "max not equal.");
    mu_assert(EQ(expect_mean, Stats_mean(st), 3), "mean not valid.");
    mu_assert(EQ(expect_stddev, Stats_stddev(st), 3), "stddev not valid.");

    return NULL;
}

char* all_tests()
{
    mu_suite_start();

    mu_run_test(test_operations_from_table_of_data);
    mu_run_test(test_operations_from_file);
    mu_run_test(test_operations);
    mu_run_test(test_recreate);

    return NULL;
}

RUN_TESTS(all_tests);