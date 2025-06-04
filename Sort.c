#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Sort.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "Data.h"
#include "Menu.h"
#include "Search_menu.h"




static int compare_two_elements_up(const data* value1, const data* value2)
{
	int s;
	if ((s = strcmp(value1->name, value2->name)) != 0)
		return s;
	if ((s = strcmp(value1->specialization, value2->specialization)) != 0)
		return s;
	if (value1->summa != value2->summa)
		return value1->summa - value2->summa;
	if (value1->rating  != value2->rating )
		return value1->rating - value2->rating ;
	return value1->document - value2->document;

}

static int compare_two_elements_down(const data* value1, const data* value2)
{
	int s;
	if ((s = strcmp(value2->name, value1->name)) != 0)
		return s;
	if ((s = strcmp(value2->specialization, value1->specialization)) != 0)
		return s;
	if (value2->summa != value1->summa)
		return value2->summa - value1->summa;
	if (value2->rating  != value1->rating )
		return value2->rating - value1->rating ;
	return value2->document - value1->document;
}

static void bubble_sort(data* ptr, compare_ptr compare, int n)
{
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (compare(&ptr[j], &ptr[j + 1]) > 0) 
			{
				data temp = ptr[j];
				ptr[j] = ptr[j + 1];
				ptr[j + 1] = temp;
			}
		}
	}
}

static void insertion_sort(data* ptr, compare_ptr compare, int n) 
{
	for (int i = 1; i < n; i++) {
		data value= ptr[i];
		int j = i - 1;
		while (j >= 0 && compare(&ptr[j], &value) > 0) {
			ptr[j + 1] = ptr[j];
			j--;
		}
		ptr[j + 1] = value;
	}
}
///хуйня !!!!!!!!!!!!!!!!!!!!!!!
//сделайть шейкер!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
static void fusion_sort(data* ptr, int left, int right, compare_ptr compare) 
{
	if (left < right) {
		int mid = left + (right - left) / 2;
		fusion_sort(ptr, left, mid, compare);
		fusion_sort(ptr, mid + 1, right, compare);
		fusion1(ptr, left, mid, right, compare);
	}
}

static void fusion1(data* ptr, int left, int mid, int right, compare_ptr compare)
{
	int size1 = mid - left + 1; 
	int size2 = right - mid;

	data* L = malloc(size1 * sizeof(data)); //��������� ��� ������ ����������
	if (L == NULL)
	{
		printf("Malloc error!\n");
		return;
	}
	data* R = malloc(size2 * sizeof(data)); //��������� ��� ������� ����������
	if (R == NULL)
	{
		printf("Malloc error!\n");
		return;
	}
	for (int i = 0; i < size1; i++) L[i] = ptr[left + i];
	for (int j = 0; j < size2; j++) R[j] = ptr[mid + 1 + j];

	int i = 0, j = 0, k = left;
	while (i < size1 && j < size2) {
		if (compare(&L[i], &R[j]) <= 0) {
			ptr[k] = L[i];
			i++;
		}
		else {
			ptr[k] = R[j];
			j++;
		}
		k++;
	}
	while (i < size1)
	{
		ptr[k] = L[i];
		i++;
		k++;
	}
	while (j < size2)
	{
		ptr[k] = R[j];
		j++;
		k++;
	}

	free(L);
	free(R);
}

void sort_menu(const char* file_name)
{
	sort_enum sw = exit_sort;
	int sw2 = 1;
	int massive_size = 100;
	do
	{
		printf("Choose type of sort\n"
			"1 - non-increasing sort\n"
			"2 - non-decreasing sort\n");
		if (scanf_c("%d", &sw2) != 1 || (sw2 != 1 && sw2 != 2))
		{
			printf("Input error!\n");
			continue;
		}
		compare_ptr compare = NULL;
		if (sw2 == 1)
		{
			compare = compare_two_elements_down;
		}
		else
		{
			compare = compare_two_elements_up;
		}
		printf("Enter value of massive which you want to be sorted(Maximum value - 300k)\n");
		if (scanf_c("%d", &massive_size) != 1 || (massive_size < 1 || massive_size > 300000))
		{
			printf("Input error!\n");
			continue;
		}
		data* array = massive_create(massive_size, file_name);
		if (array == NULL)
		{
			perror("Array cannot be created");
			return;
		}
		printf("Choose sorting algoritm from list:\n"
			"1 - Bubble sort\n"
			"2 - Insert sort\n"
			"3 - Fusion sort\n"
			"4 - Back\n");
		scanf_c("%d", &sw);
		switch (sw)
		{
		case bubble:
		{
			clock_t start = clock();
			bubble_sort(array, compare, massive_size);
			clock_t end = clock();
			double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
			printf("Time: %.12lf sec\n", time_spent);
			arr_to_file(array, massive_size);
			free(array);
			return;
		}
		case insert:
		{
			clock_t start = clock();
			insertion_sort(array, compare, massive_size);
			clock_t end = clock();
			double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
			printf("Time: %.12lf sec\n", time_spent);
			arr_to_file(array, massive_size);
			free(array);
			return;
		}
		case fusion:
		{
			clock_t start = clock();
			fusion_sort(array, 0, massive_size - 1, compare);
			clock_t end = clock();
			double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
			printf("Time: %.12lf sec\n", time_spent);
			arr_to_file(array, massive_size);
			free(array);
			return;
		}
		case exit_sort:
			free(array);
			break;
		default:
			printf("Input error!\n");
			free(array);
			break;
		}
	} while (sw != exit_sort);
	return;
}

static data* massive_create(int size_of_arr, const char* file_name)
{
	data* array = malloc(size_of_arr * sizeof(data));
	if (array == NULL)
	{
		perror("Array cannot be created\n");
		return NULL;
	}
	memset(array, 0, size_of_arr * sizeof(data));
	FILE* file = fopen(file_name, "rb");
	if (file == NULL)
	{
		perror("File cannot be opened\n");
		free(array);
		return NULL;
	}
	rewind(file);
	for (int i = 0; i < size_of_arr; i++)
	{
		if (fread(&array[i], sizeof(data), 1, file) != 1)
		{
			free(array);
			fclose(file);
			return NULL;
		}
	}
	fclose(file);
	return array;
}

static void arr_to_file(const data* array, int size_of_arr)
{
	char sort_file[MAX_MISC_SIZE] = { 0 };
	printf("Enter name of file(no more than %d symbols)\n", (MAX_MISC_SIZE - 5));
	scanf_c(" %20[^.\n]", sort_file);
	if (strstr(sort_file, ".bin") == NULL)
		strcat(sort_file, ".bin");
	FILE* file = fopen(sort_file, "wb");
	if (file == NULL)
	{
		perror("File cannot be recreated\n");
		return;
	}
	rewind(file);
	for (int i = 0; i < size_of_arr; i++)
	{
		fwrite(&array[i], sizeof(data), 1, file);
	}
	printf("Sorted data succesfully added to '%s'\n", sort_file);
	fclose(file);
}
