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


char* find_macro_name(char* line){
/*     char token[MAX_LEN_MACRO_NAME];
 */    char* token_ptr = strtok(line, " \t");
    char macr_statement[MAX_LEN_MACRO_NAME];
    strncpy(token_ptr, macr_statement, 4);
    printf("%s", macr_statement);

/*     char* macro_name;
 */    /*TODO!! what will happend in this function. maybe problem with \n
    and the next strings in file.!!!*/

    /*The first word in the line: macr statement*/
    /* if (strcmp(token, macr_statement)){
        token = strtok_r(the_rest, " \t", &the_rest);
    } */
    
    /*The second statement in line - the macro name*/
    /* *macro_name = strdup(token);
    if (*macro_name == NULL){
        printf("no macro name defined");
        exit(1);
    } */
    /*TODO: check if the name is a function or other saved word*/

    /*check for more data after*/
    /* if (strtok_r(the_rest, " \t", &the_rest) != NULL){
        printf("error in input file: more data after macro name");
        exit(1);
    }
    return macro_name; */
    return token_ptr;
}

bool check_validation_macro_line_add_macro_to_macros_list(char *line, macro** macro_list, int macro_counter){
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