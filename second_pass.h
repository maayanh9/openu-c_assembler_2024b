#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "first_pass.h"
#include "settings.h"

typedef struct SecondPassOutput{
    bool success;
    DynamicList parsed_lines_list;
    DynamicList entry_file_data;
    DynamicList extern_file_data;
    DynamicList object_file;
    char first_line_object_file[LENGTH_OF_LINE_OBJECT_FILE];
} SecondPassOutput;

/* this is the last rout of processing which is done on the first pass output
 * the second pass includes updating the locations of the labels of direct addressing method
 * and set the input of the ob, ext and ent files.
 */
SecondPassOutput second_pass(FirstPassOutput first_pass_output);

#endif
