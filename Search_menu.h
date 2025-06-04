#pragma once
#include "Data.h"



typedef enum
{
    name = 1,
    specialization,
    summa,
    rating,
    document,
    exit_search
} data_cord;

typedef int (*search_func)(const data*, const void*);

static int is_correct_name(const data* person, const void* value);
static int is_correct_specialization(const data* person, const void* value);
static int is_correct_summa(const data* person, const void* value);
static int is_correct_rating(const data* person, const void* value);
static int is_correct_document(const data* person, const void* value);

void search_record(const char* file_name, search_func search_type, const void* value);
void search_menu(const char* file_name);
