#include "tach.h"

int main() {
    FILE *f = fopen("out.bytecode", "r");
    program *prog = tach_read(f);
    tach_interp(prog);
    fclose(f);
}