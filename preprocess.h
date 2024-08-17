#ifndef PREPROCESS_H
#define PREPROCESS_H

#define MAX_LEN_LINE_ASSEMBLY_FILE 81

typedef enum {
    SEARCHING_FOR_MACRO_CREATION,
    INSIDE_MACRO,
    END_MACRO,
    CALLED_MACRO,
    OTHER
} line_macro_state;

struct macro{
    char *macro_name;
    int num_of_lines;
    char* macro_beginning;
};


int parse_file_with_macros(const char *filename);
#endif