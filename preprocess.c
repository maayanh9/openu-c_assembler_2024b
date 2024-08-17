#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_file_and_insert_to_hash_table(const char *filename){
    FILE *file;
    int num;
    file = fopen(filename, "r");
    if(file == NULL){
        /*Error opening file*/
        perror("Error: ");
        exit(1);
    }
    while(fscanf(file, "%d", &num) == 1){
        if(num >= 0 && num <= 28){
            insert_to_hash_table(num, filename);
        }
        else{
            printf("Error:\tin %s:\t%d is not in the range between 0 to %d.\n", filename, num, HASH_TABLE_CAPACITY - 1);
        }
    }
    if(ferror(file)){
        printf("no numbers in file:\t%s", filename);
    }
    fclose(file);
}



int main(int argc, char *argv[]){
    create_new_files_with_macros_inserted(argc, argv);
    return 0;
}