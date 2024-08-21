#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "preprocess.h"

bool is_it_a_macro_statement_line(char *line){
    /*TODO: see edge cases for macr string*/
    const char *macro_call = "macr";
    while (IS_SPACE_OR_TAB(*line)){
        line++;
    }
    if(strncmp(line, macro_call, 4) == 0){
        if(IS_SPACE_OR_TAB(line[4])){
            return true;
            /*TODO: make sure the line[4] doesn't do problems.
            I dont think it should*/
        }
    }
    
    return false;
}

int check_how_many_elements_in_line(char *line){
    int c = 0;
    char* token_ptr = strtok(line, " \t");
    while (token_ptr)
    {
        c++;
        token_ptr = strtok(NULL, " \t");
    }
    
    return c;
}


char* find_macro_name(char* line){
    char* macr_name_ptr = strtok(line, " \t");
    char* macro_name = (char*)malloc(MAX_LEN_MACRO_NAME*sizeof(char));

    if (strncmp("macr", macr_name_ptr, 4) != 0){
        printf("Error occurred while parse macro name\n");
        exit(1);
    }

    macr_name_ptr = strtok(NULL, " \t");
    if(macr_name_ptr != NULL){
        strncpy(macro_name, macr_name_ptr, MAX_LEN_MACRO_NAME);
    }
    else{
        /*TODO: check if you need to not exit and return all errors*/
        printf("Missing of macro name\n");
        exit(1);
    }
    printf("%s\n*\n\n", macro_name);
    return macro_name;
}

bool check_validation_macro_line_add_macro_to_macros_list(char *line, macro** macro_list, int macro_counter){
    char* macro_name = find_macro_name(line);
    printf("%c\n\n", macro_name[0]);
    /**macro_list = realloc(*macro_list, (macro_counter + 1) * sizeof(macro *));
    CHECK_ALLOCATION(*macro_list);*/
    return true;
}

int parse_file_with_macros(const char *filename){
    FILE *file = fopen(filename, "r");
    line_macro_state line_state;
    char line[MAX_LEN_LINE_ASSEMBLY_FILE];

    size_t macro_counter = 0;
    macro* macro_list;

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
                    if (check_validation_macro_line_add_macro_to_macros_list(line, &macro_list, macro_counter)){
                        macro_counter++;
                        line_state = INSIDE_MACRO;}
                    else{
                        printf("Error occurred while parsing the macro");
                        exit(1);
                    }
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