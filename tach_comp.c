#include "tach.h"

void tach_compile_more(tach_program *prog, tach_ast *ast) {
    if (prog->opcount + 4 < prog->alloc) {
        prog->alloc *= 1.5;
        prog->opcodes = tach_malloc(sizeof(opcode) * prog->alloc);
    }
    if (prog->stringc + 4 < prog->stringa) {
        prog->stringa *= 1.5;
        prog->opcodes = tach_malloc(sizeof(char *) * prog->stringa);
    }
    if (prog->numberc + 4 < prog->numbera) {
        prog->numberc *= 1.5;
        prog->opcodes = tach_malloc(sizeof(char *) * prog->numbera);
    }
    switch (ast->type) {
        case TACH_AST_TYPE_PROGRAM: {
            printf("PROGRAM\n");
            break;
        }
        default: {
            printf("error: cannot handle ast type %d\n", ast->type);
            exit(1);
        }
    }
}

tach_program *tach_compile(tach_ast *ast) {
    tach_program *prog = tach_malloc(sizeof(tach_program));
    prog->alloc = 8;
    prog->numberc = 0;
    prog->numbera = 8;
    prog->stringc = 0;
    prog->stringa = 8;
    prog->opcodes = tach_malloc(sizeof(opcode) * prog->alloc);
    prog->numbers = tach_malloc(sizeof(double) * prog->numbera);
    prog->strings = tach_malloc(sizeof(char *) * prog->stringa);
    tach_compile_more(prog, ast);
    return prog;
}