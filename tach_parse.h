#include "tach.h"

struct tach_tokens {
    char **names;
    tach_token_type *types;
    uint32_t count;
    uint32_t at;
    uint32_t alloc;
};

struct tach_ast {
    char *str;
    tach_ast **children;
    uint32_t count;
    tach_ast_node_type type;
};

tach_tokens *tach_tokenize_file(FILE *f);
tach_ast *tach_parse_single(tach_tokens *toks);
tach_ast *tach_parse_command(tach_tokens *toks);
tach_ast *tach_parse_body(tach_tokens *toks);
tach_ast *tach_parse_tokens(tach_tokens *toks);