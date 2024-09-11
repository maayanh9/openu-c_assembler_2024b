#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "settings.h"
#include "text_and_digits_handler.h"


#define IS_SPACE_OR_TAB(char1) (char1 == ' ' || char1 == '\t')

/* Pre-Processes a given input file for assembly, does not include actual assembly.
 * The output of the pre process will be saved to a file with the same base name but with the extension of ".am"
 * @input_file_name to pre-process.
 */
bool preprocess_macro(const char *input_file_name);
#endif