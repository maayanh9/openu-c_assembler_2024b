#ifndef TEXT_AND_DIGITS_HANDLER_H
#define TEXT_AND_DIGITS_HANDLER_H

#include <stdio.h>
#include <stdbool.h>

#define CHECK_ALLOCATION(ptr) do { \
    if ((ptr) == NULL) { \
        printf("Failed in allocating memory\n"); \
        exit(1); \
    } \
} while (0)

char* string_copy(const char* str_input);

char* change_file_extention(const char* file_name, const char* file_extention);

bool check_if_file_opened_successfully(FILE *file);

int add_element_to_encoding(int current_encoding, int element, int where_to_insert);

int mask_15_bits(int number);
/*int mask_12_bits(int number);*/

#endif