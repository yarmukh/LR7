#pragma once
#include "Data.h"


typedef enum
{
    bubble = 1,
    insert,
    fusion,
    exit_sort
} sort_enum;

typedef int (*compare_ptr)(const data*, const data*);

int compare_two_elements_up(const data* value1, const data* value2);
int compare_two_elements_down(const data* value1, const data* value2);

data* massive_create(int size_of_arr, const char* file_name);
//удалить
void fusion1(data* ptr, int left, int mid, int right, compare_ptr compare);///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void fusion_sort(data* ptr, int left, int right, compare_ptr compare);
//или переписать
void insertion_sort(data* ptr, compare_ptr compare, int n);

void bubble_sort(data* ptr, compare_ptr compare, int n);
//реализовать сортировку шейкер!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void arr_to_file(const data* array, int size_of_arr);

void sort_menu(const char* file_name);
