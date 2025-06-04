#pragma once
#include "Data.h"


typedef enum
{
    open = 1,
    add,
    delrecord,
    search,
    edit,
    print,
    sortl,
    generate,
    exit_menu
} functions;

void menu();

void add_record(const char* file_name);

void delete_record(const char* file_name, int index);

void edit_record(const char* file_name, int index);

int print_records(const char* file_name);

void generate_records300k();

