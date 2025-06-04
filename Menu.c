#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Menu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "Data.h"
#include "Search_menu.h"
#include "Sort.h"



//#define MANUAL_TYPING

void menu()
{
    functions sv = add;
    int index = 0;
    char file_name[MAX_MISC_SIZE] = { 'D','a','t','a','.','b','i','n'};
    while (sv != exit_menu)
    {
        printf("\nChoose file function from list:\n"
            "1 Open file\n"
            "2 Add record\n"
            "3 Delete record\n"
            "4 Search record\n"
            "5 Edit record\n"
            "6 Print all records\n"
            "7 Sort variable amount of records\n"
            "8 Generate %d records\n"
            "9 Exit out of programm\n\n", TOTAL_RECORDS);
        if (scanf_c("%d", &sv) != 1)
        {
            printf("Input error!\n");
            continue;
        }
        switch (sv)
        {
        case open:
            printf("Enter name of file(no more than %d symbols)\n", (MAX_MISC_SIZE - 5));
            scanf_c(" %20[^.\n]", file_name);
            if (strstr(file_name, ".bin") == NULL)
                strcat(file_name, ".bin");
            break;
        case add:
            add_record(file_name);
            break;
        case delrecord:
            printf("Enter record index to delete, enter 0 to back\n");
            if (scanf_c("%d", &index) != 1 || index < 0)
            {
                printf("Input error!\n");
                break;
            }
            if (index == 0)
                break;
            delete_record(file_name, index);
            break;
        case search:
            search_menu(file_name);
            break;
        case edit:
            printf("Enter record index to edit, enter 0 to back\n");
            if (scanf_c("%d", &index) != 1 || index < 0)
            {
                printf("Input error!\n");
                break;
            }
            if (index == 0)
                break;
            edit_record(file_name, index);
            break;
        case print:
            putchar('\n');
            printf("Number of records - %d\n", print_records(file_name));
            break;
        case sortl:
            sort_menu(file_name);
            break;
        case generate:
            generate_records300k();
            break;
        case exit_menu:
            break;
        default:
            printf("Input error!\n");
            break;
        }
    }
}

static int print_records(const char* file_name)
{
    data person;
    FILE* file = fopen(file_name, "rb");
    if (file == NULL)
    {
        perror("File cannot be opened\n");
        return 0;
    }
    rewind(file);
    int i = 0;
    int j = 0;

    printf("%-5s %-48s %-15s %-23s %-23s %-5s\n", "Index", "Name", "Specialization", "Summa", "Rating", "Document");
    printf("\n");

    while (fread(&person, sizeof(person), 1, file) == 1)
    {
        i++;
        if (person.is_deleted != 1)
        {
            printf("%-5d %-48s %-15s %-23d %-23d %-5s\n", i, person.name, person.specialization, person.summa, person.rating, person.document ? "No" : "Yes");
            j++;
        }
    }
    fclose(file);

    return j;
}

static void edit_record(const char* file_name, int index)
{
    FILE* file = fopen(file_name, "rb+");
    if (file == NULL)
    {
        perror("File cannot be opened");
        return;
    }
    fseek(file, (index - 1) * sizeof(data), SEEK_SET);
    data person;
    fread(&person, sizeof(data), 1, file);
    if (person.is_deleted)
    {
        printf("Cannot edit a deleted record!\n");
        fclose(file);
        return;
    }
    printf("Enter full name:\n");
    scanf_c(" %[^\n]", person.name);
    printf("Enter specialization:\n");
    scanf_c(" %[^\n]", person.specialization);
    printf("Enter summa:\n");
    if (scanf_c("%d", &person.summa) != 1)
    {
        printf("Input error\n");
        return;
    }
    printf("Enter rating:\n");
    if (scanf_c("%d", &person.rating) != 1)
    {
        printf("Input error\n");
        return;
    }
    printf("Enter: Is the applicant submitted the original document (yes - 1/no - 0):\n");
    int document_int=0;
    bool document=0;
    if ((scanf_c("%d", &document_int) != 1 )||( (document_int != 0 && document_int != 1)))
    {
        printf("Input error!\n");
        return;
    }
    document = (document_int == 1);
    fseek(file, (index - 1) * sizeof(data), SEEK_SET);
    fwrite(&person, sizeof(person), 1, file);
    fclose(file);
    return;
}

static void delete_record(const char* file_name, int index)
{
    FILE* file = fopen(file_name, "rb+");
    if (file == NULL) {
        perror("File cannot be opened");
        return;
    }
    data person;
    fseek(file, (index - 1) * sizeof(data), SEEK_SET);
    if (fread(&person, sizeof(data), 1, file) != 1)
    {
        printf("Record does not exist!\n");
        fclose(file);
        return;
    }
    if (person.is_deleted)
    {
        printf("Record already deleted!\n");
        fclose(file);
        return;
    }
    person.is_deleted = 1;
    fseek(file, (index - 1) * sizeof(data), SEEK_SET);
    fwrite(&person, sizeof(data), 1, file);
    fclose(file);
    printf("Record successfully deleted!\n");
}

static void add_record(const char* file_name)
{
    data person = { 0 };
    FILE* file = fopen(file_name, "rb+");
    if (file == NULL)
    {
        file = fopen(file_name, "ab+");
        if (file == NULL)
        {
            perror("File cannot be opened or created");
            return;
        }
        printf("File %s is created\n", file_name);
    }
    fseek(file, 0, SEEK_END);
#ifdef MANUAL_TYPING
    printf("Enter full name:\n");
    scanf_c(" %[^\n]", person.name);
    printf("Enter specialization:\n");
    scanf_c(" %[^\n]", person.specialization);
    printf("Enter summa:\n");
    if (scanf_c("%d", &person.summa) != 1)
    {
        printf("Input error\n");
        return;
    }
    printf("Enter rating:\n");
    if (scanf_c("%d", &person.rating) != 1)
    {
        printf("Input error\n");
        return;
    }
    printf("Enter: Is the applicant submitted the original document (yes - 1/no - 0):\n");
    int document_int;
    if (scanf_c("%d", &document_int) != 1 || (document_int != 0 && document_int != 1))
    {
        printf("Input error!\n");
        return;
    }
    bool document = (document_int == 1);
    fwrite(&person, sizeof(person), 1, file);
#else
    printf("Enter the name of the file from which the elements will be copied\n");
    char copy_file_name[MAX_MISC_SIZE];
    scanf_c(" %20[^.\n]", copy_file_name);
    copy_file_name[sizeof(copy_file_name) - 1] = '\0';
    if ((strlen(copy_file_name) + 4) > MAX_MISC_SIZE) 
    {
        printf("File name is too long!\n");
        return;
    }
    if (strstr(copy_file_name, ".bin") == NULL)
        strcat(copy_file_name, ".bin");
    FILE* file_data = fopen(copy_file_name, "rb");
    if (file_data == NULL) {
        fclose(file);
        perror("File cannot be opened");
        return;
    }
    fseek(file_data, 0, SEEK_SET);
    while (fread(&person, sizeof(person), 1, file_data))
    {
        fwrite(&person, sizeof(person), 1, file);
    }
    fclose(file_data);
#endif
    fclose(file);
}

void generate_records300k()
{
    const char* names[] = {
        "Ivanov Ivan Ivanovich",
        "Petrov Petr Petrovich",
        "Sidorova Anna Mikhailovna",
        "Kozlova Ekaterina Sergeevna",
        "Smirnov Dmitrii Aleksandrovich",
        "Alekseeva Mariia Vladimirovna",
        "Vasilev Konstantin Dmitrievich",
        "Nikolaeva Elena Stanislavovna",
        "Kuznetsov Roman Olegovich",
        "Popova Anastasiia Maksimovna",
        "Lebedev Vladislav Semenovich",
        "Novikova Tatiana Pavlovna",
        "Morozov Igor Anatolevich",
        "Volkova Ol'ga Denisovna",
        "Solovev Pavel Evgenevich",
        "Fedorova Liudmila Viktorovna",
        "Belov Aleksandr Nikolaevich",
        "Krylova Veronika Igorevna",
        "Tikhonov Artem Vadimovich",
        "Savelieva Iuliia Olegovna"
    };

    const char* specializations[] = {
        "Matematika",
        "Fizika",
        "Informatika",
        "Himiia",
        "Biologiia",
        "Istoriia",
        "Literatura",
        "Geografiia",
        "Obshchestvoznanie",
        "Angliski yazyk",
        "Ekologiia",
        "Ekonomika",
        "Pravo",
        "Astronomia",
        "Tekhnologia"
    };
    FILE* file = fopen("generated_records.bin", "wb");
    if (!file) 
    {
        perror("File creating error!\n");
        return;
    }
    srand(time(NULL));
    for (int i = 0; i < TOTAL_RECORDS; i++) {
        data record = { 0 };
        strncpy(record.name, names[rand() % 20], MAX_NAME_SIZE - 1);
        strncpy(record.specialization, specializations[rand() % 15], MAX_MISC_SIZE - 1);
        record.summa = 50 + rand() % 500;
        record.rating = 1 + rand() % 24;
        record.document = ( (rand() % 2)==0 );
        record.is_deleted = 0;
        fwrite(&record, sizeof(data), 1, file);
    }

        fclose(file);
        printf("File 'generated_records.bin' succesful created with %d records\n", TOTAL_RECORDS);
    
}
