#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "output_files.h"
#include "dynamic_list.h"
#include "second_pass.h"
#include "text_and_digits_handler.h"

/* Dump a collection of strings into a file delimited by newlines
 * @first_line: if give, will be inserted as the first line in the file before the strings in the list.
 */
bool export_file_from_list_of_strings(DynamicList list, const char* file_path, const char* first_line) {
    int i;
    FILE *file;

    if (list.list_length == 0) /*do not create a file if there aren't extern commands in the assembly code*/
        return true;
    file = fopen(file_path, "w");
    if(!check_if_file_opened_successfully(file)) {
        printf("ERROR - could not create file: %s\n", file_path);
        return false;
    }
    if(first_line != NULL)
        fprintf(file, "%s", first_line);

    for(i = 0; i< list.list_length; i++) {
        fprintf(file,"%s", (char*)list.items[i]);
    }
    fclose(file);
    return true;
}

/* Free all given allocated file paths */
void free_file_names(char *entry_file_path, char *external_file_path, char *object_file_path) {
    free(entry_file_path);
    free(external_file_path);
    free(object_file_path);
}

/* Dump the second pass outputs to a collection of files with a common base name of "file_name" */
bool export_output_assembler_files(SecondPassOutput second_pass_output, const char *file_name){
    char* entry_file_path = change_file_extention(string_copy(file_name), FILE_EXTENSION_ENTRY_FILE);
    char* external_file_path = change_file_extention(string_copy(file_name), FILE_EXTENSION_EXTERNAL_FILE);
    char* object_file_path = change_file_extention(string_copy(file_name), FILE_EXTENSION_OBJECT_FILE);
    bool result = true;

    if(!export_file_from_list_of_strings(second_pass_output.entry_file_data, entry_file_path, NULL) ||
        !export_file_from_list_of_strings(second_pass_output.extern_file_data, external_file_path, NULL) ||
        !export_file_from_list_of_strings(second_pass_output.object_file, object_file_path, second_pass_output.first_line_object_file)) {
        result = false;
    }

    free_file_names(entry_file_path, external_file_path, object_file_path);
    free_dynamic_list(&second_pass_output.entry_file_data);
    free_dynamic_list(&second_pass_output.extern_file_data);
    free_dynamic_list(&second_pass_output.object_file);

    return result;
}

