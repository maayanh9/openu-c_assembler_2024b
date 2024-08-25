#ifndef UTILS_H
#define UTILS_H

#define CHECK_ALLOCATION(ptr) do { \
    if ((ptr) == NULL) { \
        printf("Failed in allocating memory\n"); \
        exit(1); \
    } \
} while (0)

char* string_copy(const char* str_input);

char* change_file_extention(char* file_name, char* file_extention);


#endif