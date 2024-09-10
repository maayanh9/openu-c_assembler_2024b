#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "output_files.h"
#include "dynamic_list.h"
#include "second_pass.h"
#include "text_handler.h"

typedef enum EntryOrExtern {
    ENTRY_LIST,
    EXTERN_LIST
}EntryOrExtern;

bool export_file_from_list(DynamicList list, const char* file_path, EntryOrExtern entry_or_extern) {
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
        switch (entry_or_extern) {
            case ENTRY_LIST:
                ParsedLine *parsed_entry_line = (ParsedLine *)list.items[i];
                fprintf(file, "%s %04d\n", ((ParsedLine *)list.items[i])->LineTypes.Directive.DirectiveTypes.entry_or_extern, ((ParsedLine *)list.items[i])->mete_data.instruction_counter);
                break;
            case EXTERN_LIST:
                fprintf(file,"%s", (char*)list.items[i]);
                break;
            default:
                break;
        }

    }
    fclose(file);
    return true;
}
bool export_entry_file(DynamicList entry_ptrs, char* entry_file_path){
    return export_file_from_list(entry_ptrs, entry_file_path, ENTRY_LIST);
}

bool export_external_file(DynamicList extern_file_data, char* extern_file_path){
    return export_file_from_list(extern_file_data, extern_file_path, EXTERN_LIST);
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
    /*export_entry_file(second_pass_output.entry_ptrs, entry_file_name);*/
    export_external_file(second_pass_output.extern_file_data, external_file_name);

    free(entry_file_name);
    free(external_file_name);
    return true;
}

