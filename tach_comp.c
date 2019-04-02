#include "tach.h"

void tach_compile_more(tach_program *prog, tach_ast *ast) {
    switch (ast->type) {
        case TACH_AST_TYPE_PROGRAM: {
            for (uint32_t i = 0; i < ast->count; i++) {
                if (i != 0) {
                    prog->opcodes[prog->opcount] = OPCODE_POP;
                    prog->opcount ++;
                }
                tach_compile_more(prog, ast->children[i]);
            }
            break;
        }
        case TACH_AST_TYPE_COMMAND: {
            for (uint32_t i = 0; i < ast->count; i++) {
                tach_compile_more(prog, ast->children[i]);
            }      
            prog->opcodes[prog->opcount] = OPCODE_CALL;
            prog->opvalues[prog->opcount] = ast->count - 1;
            prog->opcount ++;
            break;
        }
        case TACH_AST_TYPE_NAME: {
            prog->opcodes[prog->opcount] = OPCODE_NAME;
            prog->opvalues[prog->opcount] = prog->stringc;
            prog->strings[prog->stringc] = ast->str;
            prog->stringc ++;
            prog->opcount ++;
            break;
        }
        case TACH_AST_TYPE_STRING: {
            prog->opcodes[prog->opcount] = OPCODE_STR;
            prog->opvalues[prog->opcount] = prog->stringc;
            prog->strings[prog->stringc] = ast->str;
            prog->stringc ++;
            prog->opcount ++;
            break;
        }
        case TACH_AST_TYPE_NUMBER: {
            prog->opcodes[prog->opcount] = OPCODE_NUM;
            prog->opvalues[prog->opcount] = prog->numberc;
            prog->numbers[prog->numberc] = atof(ast->str);
            prog->numberc ++;
            prog->opcount ++;
            break;
        }
        case TACH_AST_TYPE_BODY: {
            uint32_t beginc = prog->opcount;
            prog->opcodes[prog->opcount] = OPCODE_PROC;
            prog->opcount ++;
            for (uint32_t i = 0; i < ast->count; i++) {
                if (i != 0) {
                    prog->opcodes[prog->opcount] = OPCODE_POP;
                    prog->opcount ++;
                }
                tach_compile_more(prog, ast->children[i]);
            } 
            prog->opcodes[prog->opcount] = OPCODE_RET;
            prog->opvalues[beginc] = prog->opcount - beginc - 1;
            prog->opcount ++;
            break;
        }
        default: {
            printf("error: cannot handle ast type %d\n", ast->type);
            exit(1);
        }
    }
    if (prog->opcount + 4 >= prog->alloc) {
        prog->alloc *= 1.5;
        prog->opcodes = tach_realloc(prog->opcodes, sizeof(opcode) * prog->alloc);
        prog->opvalues = tach_realloc(prog->opvalues, sizeof(uint32_t) * prog->alloc);
    }
    if (prog->stringc + 4 >= prog->stringa) {
        prog->stringa *= 1.5;
        prog->strings = tach_realloc(prog->strings, sizeof(char *) * prog->stringa);
    }
    if (prog->numberc + 4 >= prog->numbera) {
        prog->numbera *= 1.5;
        prog->numbers = tach_realloc(prog->numbers, sizeof(uint32_t) * prog->numbera);
    }
}

tach_program *tach_compile(tach_ast *ast) {
    tach_program *prog = tach_malloc(sizeof(tach_program));
    prog->opcount = 0;
    prog->alloc = 8;
    prog->numberc = 0;
    prog->numbera = 8;
    prog->stringc = 0;
    prog->stringa = 8;
    prog->opcodes = tach_malloc(sizeof(opcode) * prog->alloc);
    prog->opvalues = tach_malloc(sizeof(uint32_t) * prog->alloc);
    prog->numbers = tach_malloc(sizeof(double) * prog->numbera);
    prog->strings = tach_malloc(sizeof(char *) * prog->stringa);
    tach_compile_more(prog, ast);
    return prog;
}