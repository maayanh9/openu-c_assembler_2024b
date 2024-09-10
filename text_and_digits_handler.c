#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "text_and_digits_handler.h"
#include "settings.h"

char* string_copy(const char* str_input) {
    char* str_copy = (char *)malloc(strlen(str_input) + 1); /* plus 1 for \0*/
    CHECK_ALLOCATION(str_copy);
    strcpy(str_copy, str_input);
    return str_copy;
}

char* change_file_extention(const char* file_name, const char* file_extention){
    char* last_dot_in_file = strrchr(file_name, '.');
    size_t file_len_without_extention;
    size_t new_file_name_len;
    char* new_file_name;
    if (last_dot_in_file){
        file_len_without_extention = last_dot_in_file - file_name;
        new_file_name_len = file_len_without_extention + strlen(file_extention) + 1; /* plus one for \0*/
        new_file_name = (char *)malloc(new_file_name_len);
        CHECK_ALLOCATION(new_file_name);
        strncpy(new_file_name, file_name, file_len_without_extention);
        new_file_name[file_len_without_extention] = '\0';
        strcat(new_file_name, file_extention);
    }
    else{
        printf("invalid file_name:\t%s.\n exiting", file_name);
        exit(1);
    }
    return(new_file_name);
}

bool check_if_file_opened_successfully(FILE *file){
    if(file == NULL){
        /*Error opening file*/
        perror("Error: ");
        return false;
    }
    return true;
}

int add_element_to_encoding(int current_encoding, int element, int where_to_insert) {
    return current_encoding | (element << where_to_insert);
}