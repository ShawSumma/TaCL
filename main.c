#include "tach.h"

int main(int argc, char **argv) {
    GC_INIT();
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        puts(argv[1]);
        tach_tokens *toks = tach_tokenize_file(f);
        fclose(f);
        for (uint32_t i = 0; i < toks->count; i++) {
            printf("%s\n", toks->names[i]);
        }
        tach_ast *ast = tach_parse_tokens(toks);
        tach_program *prog = tach_compile(ast);
        for (uint32_t i = 0; i < prog->opcount; i++) {
            puts(tach_opcode_name(prog->opcodes[i]));
        }
        tach_interp(prog);
    }
    else {
        FILE *f = fopen("out.bytecode", "r");
        tach_program *prog = tach_read(f);
        fclose(f);
        tach_interp(prog);
    }
}