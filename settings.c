#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "settings.h"



const int addressing_to_fit_object_file[4] = {1, 2, 4, 8}; /* 0001, 0010, 0100, 1000 in decimal */

const int a_r_e_fields[3] = {4, 2, 1}; /* set the right bit in the order on A=100, R=010, E=001 */


const char *instructions_commands_and_addressing[LEN_OF_COMMANDS_LIST][4] = {
    /*command, dst, src, how_many_parameters*/
    {"mov", "123", "0123", "2"},
    {"cmp", "0123", "0123", "2"},
    {"add", "123", "0123", "2"},
    {"sub", "123", "0123", "2"},
    {"lea", "123", "1", "2"},
    {"clr", "123", NULL, "1"},
    {"not", "123", NULL, "1"},
    {"inc", "123", NULL, "1"},
    {"dec", "123", NULL, "1"},
    {"jmp", "12", NULL, "1"},
    {"bne", "12", NULL, "1"},
    {"red", "123", NULL, "1"},
    {"prn", "0123", NULL, "1"},
    {"jsr", "12", NULL, "1"},
    {"rts", NULL, NULL, "0"},
    {"stop", NULL, NULL, "0"}
};

const char* directives_list[LEN_OF_DIRECTIVE_LIST] = {"data", "string", "extern", "entry"};
