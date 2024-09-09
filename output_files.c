#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "output_files.h"
#include "dynamic_list.h"
#include "second_pass.h"
#include "text_handler.h"


bool export_entry_file(DynamicList entry_ptrs, char* entry_file_name){
    return true;
}

bool export_external_file(DynamicList extern_file_data, char* extern_file_path){
    int i;
    FILE *extern_file;

    if (extern_file_data.list_length == 0) /*do not create a file if there aren't extern commands in the assembly code*/
        return true;

    extern_file = fopen(extern_file_path, "w");
    if(!check_if_file_opened_successfully(extern_file)) {
        printf("ERROR - could not create file: %s\n", extern_file_path);
        return false;
    }

    for(i = 0; i< extern_file_data.list_length; i++) {
        fprintf(extern_file,"%s", extern_file_data.items[i]);
    }
    fclose(extern_file);
    return true;
}

bool create_first_pass_parsed_file_and_return(){
    return true;
}
bool found_errors_in_the_assembly_input_file(DynamicList errors_ptrs) {
    /*checks for error lines. if there are error lines,
     * the lines will be printed.
     * the function returns:    true if there are errors
     *                          and false if there are not
     */

    /** TODO: add it to the first pass*/

    int i;
    if(errors_ptrs.list_length == 0) {
        return false;
    }
    for(i = 0; i< errors_ptrs.list_length; i++) {
        printf("%s", ((ParsedLine*)errors_ptrs.items[i])->LineTypes.error_str);
    }
    return true;
}


bool export_output_assembler_files(SecondPassOutput second_pass_output, const char *file_name){
    char* entry_file_name = change_file_extention(string_copy(file_name), FILE_EXTENSION_ENTRY_FILE);
    char* external_file_name = change_file_extention(string_copy(file_name), FILE_EXTENSION_EXTERNAL_FILE);

    if(found_errors_in_the_assembly_input_file(second_pass_output.errors_ptrs)) {
        free(entry_file_name);
        free(external_file_name);
        return false;
    }
    export_entry_file(second_pass_output.entry_ptrs, entry_file_name);
    export_external_file(second_pass_output.extern_file_data, external_file_name);

    free(entry_file_name);
    free(external_file_name);
    return true;
}

