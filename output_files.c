#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "output_files.h"
#include "dynamic_list.h"
#include "second_pass.h"
#include "text_handler.h"


bool export_entry_file(){

}
bool export_external_file(){

}

bool create_first_pass_parsed_file_and_return(){

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


bool export_output_assembler_files(SecondPassOutput second_pass_output){
    if(found_errors_in_the_assembly_input_file(second_pass_output.errors_ptrs)) {
        return false;
    }
    export_entry_file(second_pass_output.entry_ptrs);
    return true;
}

