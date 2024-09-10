#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "settings.h"
#include "text_and_digits_handler.h"


#define IS_SPACE_OR_TAB(char1) (char1 == ' ' || char1 == '\t')


bool preprocess_macro(const char *input_file_name);
#endif