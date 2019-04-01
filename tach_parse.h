#include "tach.h"

struct tach_tokens {
    char **names;
    tach_token_type *types;
    uint32_t count;
    uint32_t alloc;
};

struct tach_ast {
    tach_ast_node_type ast;
    
};

tach_tokens *tach_tokenize_file(FILE *f);
tach_ast_node_type *tach_parse_file(FILE *f);
tach_ast_node_type *tach_parse_tokens(tach_tokens *f);