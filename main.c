#include "tach.h"

int main(int argc, char **argv) {
    GC_INIT();
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        tach_tokenize_file(f);
        fclose(f);
    }
    FILE *f = fopen("out.bytecode", "r");
    program *prog = tach_read(f);
    tach_interp(prog);
    fclose(f);
}