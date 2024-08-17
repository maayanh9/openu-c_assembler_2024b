#ifndef PREPROCESS_H
#define PREPROCESS_H

#define MAX_LEN_LINE_ASSEMBLY_FILE 81
#define IS_SPACE_OR_TAB(char1) (char1 == ' ' || char1 == '\t')

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

int preprocess_macro(const char *input_file_name);
#endif