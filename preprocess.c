#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "preprocess.h"

bool is_it_a_macro_statement_line(char *line){
    /*TODO: see edge cases for macr string*/
    const char *macro_call = "macr ";
    char *position = strstr(line, macro_call);
    if(position != NULL){
        return true;
    }
    return false;
}

int parse_file_with_macros(const char *filename){
    FILE *file = fopen(filename, "r");
    line_macro_state line_state;
    char line[MAX_LEN_LINE_ASSEMBLY_FILE];


    if(file == NULL){
        /*Error opening file*/
        perror("Error: ");
        return 1;
    }

    line_state = OTHER;
    while (fgets(line, MAX_LEN_LINE_ASSEMBLY_FILE, file) != NULL){
        switch (line_state){
            case OTHER:
                if (is_it_a_macro_statement_line(line)){
                    line_state = INSIDE_MACRO;
                }
                break;
            case INSIDE_MACRO:
                printf("%s", line);
                line_state = END_MACRO;
                break;
            case END_MACRO:
                printf("%s", line);
                line_state = OTHER;
                break;
            case CALLED_MACRO:
                break;
            default:
                break;
            /*case
            default:
                printf("error in cases");
                break;*/
        }
    }
    
    fclose(file);
    return 0;
}

int preprocess_macro(const char *input_file_name){
    int ans = 0;
    ans = parse_file_with_macros(input_file_name);
    return ans;
}