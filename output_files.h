
#ifndef OUTPUT_FILES_H
#define OUTPUT_FILES_H

#include "second_pass.h"

/* Dump the second pass outputs to a collection of files with a common base name of "file_name" */
bool export_output_assembler_files(SecondPassOutput second_pass_output, const char *file_name);

#endif
