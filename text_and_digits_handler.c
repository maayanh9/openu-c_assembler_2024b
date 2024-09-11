#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "text_and_digits_handler.h"

/* Allocate a buffer for a given string and copy it into it.
 * @str_input: null terminated string.
 */
char* string_copy(const char* str_input) {
    char* str_copy = (char *)malloc(strlen(str_input) + 1); /* plus 1 for \0*/
    CHECK_ALLOCATION(str_copy);
    strcpy(str_copy, str_input);
    return str_copy;
}

/* Replace the filename_extension of a given file path with a given filename_extension  */
char* change_file_extention(const char* file_path, const char* filename_extension){
    char* last_dot_in_file = strrchr(file_path, '.');
    size_t file_len_without_extension;
    size_t new_file_name_len;
    char* new_file_name;
    if (last_dot_in_file){
        file_len_without_extension = last_dot_in_file - file_path;
        new_file_name_len = file_len_without_extension + strlen(filename_extension) + 1; /* plus one for \0*/
        new_file_name = (char *)malloc(new_file_name_len);
        CHECK_ALLOCATION(new_file_name);
        strncpy(new_file_name, file_path, file_len_without_extension);
        new_file_name[file_len_without_extension] = '\0';
        strcat(new_file_name, filename_extension);
    }
    else{
        printf("invalid file_path:\t%s.\n exiting", file_path);
        exit(1);
    }
    return(new_file_name);
}

/* return true if the file opened successfully, false otherwise */
bool check_if_file_opened_successfully(FILE *file){
    if(file == NULL){
        /*Error opening file*/
        perror("Error: ");
        return false;
    }
    return true;
}

/* Encode a given number in a specific given bit offset inside another number.  */
int add_element_to_encoding(int current_encoding, int element, int where_to_insert) {
    return current_encoding | (element << where_to_insert);
}

/* mask the given number to be in 15 bits*/
int mask_15_bits(int number) {
    return number & 32767; /*equals to 0111 1111 1111 1111 in binary*/
}
