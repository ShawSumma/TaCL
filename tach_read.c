
#include "tach.h"

program *tach_read(FILE *f) {
    uint32_t alloc = 8;
    uint32_t numbera = 8;
    uint32_t tach_stringa = 8;

    uint32_t size = 0;
    uint32_t numberc = 0;
    uint32_t tach_stringc = 0;

    opcode *bytecode = tach_malloc(sizeof(opcode) * alloc);
    uint32_t *values = tach_malloc(sizeof(uint32_t) * alloc);
    double *numbers = tach_malloc(sizeof(double) * numbera);
    char **tach_strings = tach_malloc(sizeof(char*) * tach_stringa);

    while (!feof(f)) {
        if (size + 4 >= alloc) {
            alloc *= 1.5;
            bytecode = tach_realloc(bytecode, sizeof(opcode) * alloc);
            values = tach_realloc(values, sizeof(uint32_t) * alloc);
        }
        if (numberc + 4 >= numbera) {
            numbera *= 1.5;
            numbers = tach_realloc(numbers, sizeof(double) * numbera);
        }
        if (tach_stringc + 4 >= tach_stringa) {
            tach_stringa *= 1.5;
            tach_strings = tach_realloc(tach_strings, sizeof(char*) * tach_stringa);
        }
        char opcode[20];
        int errc = fscanf(f, "%s\n", opcode);
        if (!strcmp(opcode, "str")) {
            char str[256];
            int errc = fscanf(f, "%[^\n]", str);
            char *mstr = tach_malloc(sizeof(char)*strlen(str));
            strcpy(mstr, str);
            tach_strings[tach_stringc] = mstr;
            values[size] = tach_stringc;
            tach_stringc ++;
            bytecode[size] = OPCODE_STR;
            size ++;
        }
        else if (!strcmp(opcode, "name")) {
            char str[256];
            int errc = fscanf(f, "%s\n", str);
            char *mstr = tach_malloc(sizeof(char)*strlen(str));
            strcpy(mstr, str);
            tach_strings[tach_stringc] = mstr;
            values[size] = tach_stringc;
            tach_stringc ++;
            bytecode[size] = OPCODE_NAME;
            size ++;
        }
        else if (!strcmp(opcode, "num")) {
            double num;
            int errc = fscanf(f, "%lf\n", &num);
            numbers[numberc] = num;
            values[size] = numberc;
            numberc++;
            bytecode[size] = OPCODE_NUM;
            size ++;
        }
        else if (!strcmp(opcode, "call")) {
            unsigned int num;
            int errc = fscanf(f, "%d\n", &num);            
            values[size] = num;
            bytecode[size] = OPCODE_CALL;
            size ++;
        }
        else if (!strcmp(opcode, "ret")) {
            values[size] = 0;
            bytecode[size] = OPCODE_RET;
            size ++;
        }
        else if (!strcmp(opcode, "pop")) {
            values[size] = 0;
            bytecode[size] = OPCODE_POP;
            size ++;
        }
        else if (!strcmp(opcode, "proc")) {
            unsigned int num;
            int errc = fscanf(f, "%d\n", &num);
            values[size] = num;
            bytecode[size] = OPCODE_PROC;
            size ++;
        }
        else {
            break;
            printf(".opcode %s", opcode);
            values[size] = 0;
            bytecode[size] = OPCODE_ERR;
            size ++;
            exit(1);
        }
    }
    program *prog = tach_malloc(sizeof(program));
    prog->opcodes = bytecode;
    prog->opvalues = values;
    prog->opcount = size;
    prog->tach_strings = tach_strings;
    prog->numbers = numbers;
    return prog;
}
