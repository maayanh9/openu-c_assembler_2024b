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

/* Allocate a buffer for a given string and copy it into it.
 * @str_input: null terminated string.
 */
char* string_copy(const char* str_input);

/* Replace the filename_extension of a given file path with a given filename_extension
 * @filename_extension file extansion with the dot before. for example .am
 */
char* change_file_extention(const char* file_path, const char* filename_extension);

/* return true if the file opened successfully, false otherwise */
bool check_if_file_opened_successfully(FILE *file);

/* Encode a given number in a specific given bit offset inside another number.
 * @where_to_insert how many data shifting need to be done. for example i want to
 *      insert 1 here: 1000, i need to shift it 3 times so  where_to_insert will be 3.
 * @element the element you want to shift
 * @current_encoding the current encoding you want to insert the value inside
 */
int add_element_to_encoding(int current_encoding, int element, int where_to_insert);

/* mask the given number to be in 15 bits.*/
int mask_15_bits(int number);

#endif