#include "tach.h"

int main(int argc, char **argv) {
    GC_INIT();
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        tach_tokens *toks = tach_tokenize_file(f);
        fclose(f);
        tach_ast *ast = tach_parse_tokens(toks);
        tach_program *prog = tach_compile(ast);
        tach_interp(prog);
    }
    else {
        printf("give 1 argument");
    }
}