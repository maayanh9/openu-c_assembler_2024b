#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "dynamic_list.h"

bool assembler(const char *input_file_name);


typedef struct ParsedDataOutput{
    bool success;
    DynamicList parsed_lines_list;
    DynamicList symbols_table;
    DynamicList errors_ptrs;
    DynamicList entry_ptrs;
    DynamicList external_ptrs;
    DynamicList direct_labels_ptrs;
} ParsedDataOutput;

void free_parsed_data_output_dynamic_lists(DynamicList parsed_lines_list, DynamicList symbols_table, DynamicList errors_ptrs, DynamicList entry_ptrs, DynamicList external_ptrs, DynamicList direct_labels_ptrs);

typedef enum{
    DESTINATION = 1,
    SOURCE
}SourceOrDest;

#endif