#include "tach.h"

int main(int argc, char **argv) {
    GC_INIT();
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        tach_tokens *toks = tach_tokenize_file(f);
        fclose(f);
        // for (uint32_t i = 0; i < toks->count; i++) {
        //     printf("tok: %d\n", toks->types[i]);
        // }
        tach_ast *ast = tach_parse_tokens(toks);
        tach_program *prog = tach_compile(ast);
        // for (uint32_t i = 0; i < prog->opcount; i++) {
        //     printf("prog: %d\n", prog->opcodes[i]);
        // }
        tach_vm *state = tach_create_state();
        tach_interp(prog, state);
    }
    else {
        printf("give 1 argument\n");
    }
}