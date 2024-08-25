#ifndef SETTINGS_H
#define SETTINGS_H

#define MAX_LEN_LINE_ASSEMBLY_FILE 81
#define MAX_LEN_MACRO_NAME 76
#define MAX_LEN_OF_A_SINGLE_WORD 80

#define FILE_EXTENTION_PREPROCESSOR ".am"
#define FILE_EXTENTION_INPUT_ASSEMBLER_FILE ".as"
#define FILE_EXTENTION_OBJECT_FILE ".ob"
#define FILE_EXTENTION_EXTERNAL_FILE ".ext"

typedef struct LineMetaData{
    int data_counter;
    int instruction_counter;
}LineMetaData;

typedef enum{
    empty_or_comment_line,
    introduction_line,
    command_line
}LineType;

typedef struct ParsedLine{
    HasLabel has_label;
    char label[MAX_LEN_OF_A_SINGLE_WORD];

}ParsedLine;

typedef enum{
    LABEL,
    NO_LABEL
} HasLabel;
typedef enum{
    MOV,
    CMP,
    ADD,
    SUB,
    LEA,
    CLR,
    NOT,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    STOP
} AssemblyCommand;

typedef enum{
    DATA,
    STRING,
    EXTERN,
    ENTRY
} AssemblyDirective;



#endif