#pragma once

#define MAX_NAME_SIZE 50 
#define MAX_MISC_SIZE (MAX_NAME_SIZE / 2) 
#define TOTAL_RECORDS 300000
#include <stdbool.h>

//#define MANUAL_TYPING

typedef struct data
{
    char name[MAX_NAME_SIZE];
    char specialization[MAX_MISC_SIZE];
    int summa;
    int rating;
    bool document;
    int is_deleted;
} data;

int scanf_c(const char* format, ...);
