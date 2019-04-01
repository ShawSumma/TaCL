#include "tach.h"

int main(int argc, char **argv) {
    GC_INIT();
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        tach_ast *ast = tach_parse_tokens(tach_tokenize_file(f));
        tach_compile(ast);
        fclose(f);
    }
    else {
        FILE *f = fopen("out.bytecode", "r");
        tach_program *prog = tach_read(f);
        tach_interp(prog);
        fclose(f);
    }
}