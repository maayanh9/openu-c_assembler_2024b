#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "first_pass.h"
#include "settings.h"

typedef struct SecondPassOutput{
    bool success;
    DynamicList parsed_lines_list;
    DynamicList errors_ptrs;
    DynamicList entry_file_data;
    DynamicList extern_file_data;
    DynamicList direct_labels_ptrs;
    DynamicList object_file;
} SecondPassOutput;

SecondPassOutput second_pass(FirstPassOutput first_pass_output);

#endif
