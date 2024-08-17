#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int preprocess_macro(char* input_file_name);


void check_if_user_inserted_files(int argc){
    if (argc == 1){
        /*No input files*/
        fprintf(stderr, "Please run this program again with assembly code file / files\n");
        exit(1);
    }
}

void process_all_the_input_files(int argc, char *argv[]){
    int i;
    check_if_user_inserted_files(argc);
    for (i = 1; i < argc; i++)
    {
        /*all the assembler process for one input file at a time*/
        assembler_process(argv[i]);
    }
}

int main(int argc, char *argv[]){
    
    return 0;
}