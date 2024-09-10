#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "output_files.h"
#include "dynamic_list.h"
#include "second_pass.h"
#include "text_handler.h"


bool export_file_from_list(DynamicList list, const char* file_path) {
    int i;
    FILE *file;

    if (list.list_length == 0) /*do not create a file if there aren't extern commands in the assembly code*/
        return true;
    file = fopen(file_path, "w");
    if(!check_if_file_opened_successfully(file)) {
        printf("ERROR - could not create file: %s\n", file_path);
        return false;
    }

    for(i = 0; i< list.list_length; i++) {
        fprintf(file,"%s", (char*)list.items[i]);
    }
    fclose(file);
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

void free_file_names(char *entry_file_path, char *external_file_path, char *object_file_path) {
    free(entry_file_path);
    free(external_file_path);
    free(object_file_path);
}

bool export_output_assembler_files(SecondPassOutput second_pass_output, const char *file_name){
    char* entry_file_path = change_file_extention(string_copy(file_name), FILE_EXTENSION_ENTRY_FILE);
    char* external_file_path = change_file_extention(string_copy(file_name), FILE_EXTENSION_EXTERNAL_FILE);
    char* object_file_path = change_file_extention(string_copy(file_name), FILE_EXTENSION_OBJECT_FILE);


    if(found_errors_in_the_assembly_input_file(second_pass_output.errors_ptrs)) {
        free_file_names(entry_file_path, external_file_path, object_file_path);
        return false;
    }

    /*exporting the entry and extern files*/
    if(!export_file_from_list(second_pass_output.entry_file_data, entry_file_path) ||
        !export_file_from_list(second_pass_output.extern_file_data, external_file_path)) {
        return false;
    }

    free_file_names(entry_file_path, external_file_path, object_file_path);
    return true;
}

