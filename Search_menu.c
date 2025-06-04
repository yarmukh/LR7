#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Search_menu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "Data.h"
#include "Menu.h"
#include "Sort.h"

void search_menu(const char* file_name)
{
    data_cord sv = name;
    while (sv != exit_search)
    {
        printf("\nChoose field from list\n"
            "1 Name\n"
            "2 Specialization\n"
            "3 Summa\n"
            "4 Rating\n"
            "5 Document\n"
            "6 Back\n");
        if (scanf_c("%d", &sv) != 1)
        {
            printf("Input error!\n");
            continue;
        }
        switch (sv)
        {
        case name:
        {
            char name[MAX_NAME_SIZE] = { 0 };
            printf("\nEnter the name\n");
            scanf_c(" %[^\n]", name);
            search_record(file_name, is_correct_name, name);
            break;
        }
        case specialization:
        {
            char specialization[MAX_MISC_SIZE];
            printf("\nEnter the Specialization\n");
            scanf_c(" %[^\n]", specialization);
            search_record(file_name, is_correct_specialization, &specialization);
            break;
        }
        case summa:
        {
            int summa;
            printf("\nEnter the summa\n");
            if (scanf_c("%d", &summa) != 1)
            {
                printf("Input error!\n");
                break;
            }
            search_record(file_name, is_correct_summa, &summa);
            break;
        }
        case rating:
        {
            int rating;
            printf("\nEnter the rating\n");
            if (scanf_c("%d", &rating) != 1)
            {
                printf("Input error!\n");
                break;
            }
            search_record(file_name, is_correct_rating, &rating);
            break;
        }
        case document:
        {
            int document_int;
            printf("\nIs the applicant submitted the original document (yes - 1/no - 0)\n");
            if (scanf_c("%d", &document_int) != 1 || (document_int != 0 && document_int != 1))
            {
                printf("Input error!\n");
                break;
            }
            bool document = (document_int == 1);
            search_record(file_name, is_correct_document, &document);
            break;
        }
        case exit_search:
            break;
        default:
            printf("Input error!\n");
            break;
        }
    }
}

static int is_correct_name(const data* person, const void* value)
{
    return strcmp(person->name, (const char*)value) == 0;
}

static int is_correct_specialization(const data* person, const void* value)
{
    return strcmp(person->specialization, (const char*)value) == 0;
}

static int is_correct_summa(const data* person, const void* value)
{
    return person->summa == *(const int*)value;
}

static int is_correct_rating(const data* person, const void* value)
{
    return person->rating == *(const int*)value;
}

static int is_correct_document(const data* person, const void* value)
{
    return person->document == *(const bool*)value;
}

static void search_record(const char* file_name, search_func search_type, const void* value)
{
    FILE* file = fopen(file_name, "rb");
    if (file == NULL)
    {
        perror("File cannot be opened");
        return;
    }
    data person;
    int correct = 0;
    while (fread(&person, sizeof(data), 1, file) == 1)
    {
        if (person.is_deleted != 1 && search_type(&person, value) == 1)
        {
            printf("Found: %s %s %d %d %d\n", person.name, person.specialization, person.summa, person.rating, person.document);
            correct = 1;
        }
    }
    if (correct != 1)
        printf("No records found\n");
    fclose(file);
}