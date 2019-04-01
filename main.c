#include "tach.h"

int main(int argc, char **argv) {
    GC_INIT();
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        tach_tokens *toks = tach_tokenize_file(f);
        fclose(f);
        // for (uint32_t i = 0; i < toks->count; i++) {
        //     printf("%s\n", toks->names[i]);
        // }
        tach_ast *ast = tach_parse_tokens(toks);
        tach_program *prog = tach_compile(ast);
        // for (uint32_t i = 0; i < prog->opcount; i++) {
        //     printf("%s\n", tach_opcode_name(prog->opcodes[i]));
        //     if (prog->opcodes[i] == OPCODE_NAME) {
        //         printf("\t%s\n", prog->strings[prog->opvalues[i]]);
        //     }
        // }
        tach_interp(prog);
    }
    else {
        FILE *f = fopen("out.bytecode", "r");
        tach_program *prog = tach_read(f);
        fclose(f);
        tach_interp(prog);
    }
}