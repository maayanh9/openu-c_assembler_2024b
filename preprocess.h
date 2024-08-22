#ifndef PREPROCESS_H
#define PREPROCESS_H

#define MAX_LEN_LINE_ASSEMBLY_FILE 81
#define MAX_LEN_MACRO_NAME 76
#define MAX_LEN_OF_A_SINGLE_WORD 80
#define IS_SPACE_OR_TAB(char1) (char1 == ' ' || char1 == '\t')
#define CHECK_ALLOCATION(ptr) do { \
    if ((ptr) == NULL) { \
        printf("Failed in allocating memory\n"); \
        exit(1); \
    } \
} while (0)

typedef enum {
    SEARCHING_FOR_MACRO_CREATION,
    INSIDE_MACRO,
    END_MACRO,
    CALLED_MACRO,
    OTHER
} line_macro_state;

typedef struct macro{
    char *macro_name;
    int num_of_lines;
    char* macro_beginning;
} macro;

bool preprocess_macro(const char *input_file_name);
#endif