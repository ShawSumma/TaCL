#include "tach.h"

tach_tokens *tach_tokenize_file(FILE *f) {
    tach_tokens *ret = tach_malloc(sizeof(tach_tokens));
    ret->count = 0;
    ret->alloc = 64;
    ret->names = tach_malloc(sizeof(char *) * ret->alloc);
    ret->types = tach_malloc(sizeof(tach_token_type) * ret->alloc);
    char got = getc(f);
    while (got != EOF) {
        if (ret->count + 4 > ret->alloc) {
            ret->alloc *= 1.5;
            ret->names = tach_realloc(ret->names , sizeof(char *) * ret->alloc);
            ret->types = tach_realloc(ret->types , sizeof(char *) * ret->alloc);
        }
        if (got == '\t' || got == ' ') {
            got = getc(f);
        }
        else if (got == '\r' || got == '\n' || got == ';') {
            ret->names[ret->count] = ";;";
            ret->types[ret->count] = TACH_TOKEN_NEWLINE; 
            ret->count ++;
            got = getc(f);
        }
        else if (got == '$') {
            ret->names[ret->count] = "$";
            ret->types[ret->count] = TACH_TOKEN_DOLLAR;
            ret->count ++;
            got = getc(f);
        }
        else if (got == '{') {
            ret->names[ret->count] = "{";
            ret->types[ret->count] = TACH_TOKEN_BEGIN;
            ret->count ++;
            got = getc(f);
        }
        else if (got == '}') {
            ret->names[ret->count] = "}";
            ret->types[ret->count] = TACH_TOKEN_END;
            ret->count ++;
            got = getc(f);
        }
        else if (got == '[') {
            ret->names[ret->count] = "[";
            ret->types[ret->count] = TACH_TOKEN_OPEN;
            ret->count ++;
            got = getc(f);
        }
        else if (got == ']') {
            ret->names[ret->count] = "]";
            ret->types[ret->count] = TACH_TOKEN_CLOSE;
            ret->count ++;
            got = getc(f);
        }
        else if ((got >= '0' && got <= '9') || got == '-') {
            uint32_t allocn = 16;
            uint32_t count = 0;
            char *num = tach_malloc(sizeof(char) * allocn);
            bool begin = true;
            while ((got >= '0' && got <= '9') || got == '.' || begin ) {
                begin = false;
                num[count] = got;
                if (count + 4 > allocn) {
                    allocn *= 1.5;
                    num = tach_realloc(num, sizeof(char) * allocn);
                }
                num[count] = got;
                count ++;
                got = getc(f);
            }
            num[count] = '\0';
            ret->names[ret->count] = num;
            ret->types[ret->count] = TACH_TOKEN_NUMBER; 
            ret->count ++;
        }
        else if ((got >= 'a' && got <= 'z') || (got >= 'A' && got <= 'Z')) {
            uint32_t allocn = 16;
            uint32_t count = 0;
            char *name = tach_malloc(sizeof(char) * allocn);
            while ((got >= 'a' && got <= 'z') || (got >= 'A' && got <= 'Z')) {
                name[count] = got;
                if (count + 4 > allocn) {
                    allocn *= 1.5;
                    name = tach_realloc(name, sizeof(char) * allocn);
                }
                name[count] = got;
                count ++;
                got = getc(f);
            }
            name[count] = '\0';
            ret->names[ret->count] = name;
            ret->types[ret->count] = TACH_TOKEN_STRING; 
            ret->count ++;
        }
        else if (got == '(') {
            uint32_t allocn = 64;
            uint32_t count = 0;
            char *str = tach_malloc(sizeof(char) * allocn);
            got = getc(f);
            uint32_t depth = 1;
            while (depth != 0) {
                if (count + 4 > allocn) {
                    allocn *= 1.5;
                    str = tach_realloc(str, sizeof(char) * allocn);
                }
                if (got == ')') {
                    depth --;
                    if (depth == 0) {
                        got = getc(f);
                        break;
                    }
                }
                if (got == '(') {
                    depth ++;
                }
                str[count] = got;
                count ++;
                got = getc(f);
            }
            str[count] = '\0';
            ret->names[ret->count] = str;
            ret->types[ret->count] = TACH_TOKEN_STRING; 
            ret->count ++;
        }
        else {
            printf("error: cannot lex\n");
            exit(1);
        }
    }
    return ret;
}

tach_ast *tach_parse_single(tach_tokens *toks) {
    if (toks->types[toks->at] == TACH_TOKEN_DOLLAR) {
        tach_ast *ret = tach_malloc(sizeof(tach_ast));
        ret->type = TACH_AST_TYPE_NAME;
        ret->str = toks->names[toks->at+1];
        toks->at += 2;
        return ret;
    }
    if (toks->types[toks->at] == TACH_TOKEN_STRING) {
        tach_ast *ret = tach_malloc(sizeof(tach_ast));
        ret->type = TACH_AST_TYPE_STRING;
        ret->str = toks->names[toks->at];
        toks->at ++;
        return ret;
    }
    if (toks->types[toks->at] == TACH_TOKEN_NUMBER) {
        tach_ast *ret = tach_malloc(sizeof(tach_ast));
        ret->type = TACH_AST_TYPE_NUMBER;
        ret->str = toks->names[toks->at];
        toks->at ++;
        return ret;
    }
    if (toks->types[toks->at] == TACH_TOKEN_OPEN) {
        toks->at ++;
        tach_ast *ret = tach_parse_command(toks);
        toks->at ++;
        return ret;
    }
    if (toks->types[toks->at] == TACH_TOKEN_BEGIN) {
        toks->at ++;
        tach_ast *ret = tach_parse_body(toks);
        toks->at ++;
        return ret;
    }
    printf("error: parse cannot handle something\n");
    exit(1);
}

tach_ast *tach_parse_command(tach_tokens *toks) {
    tach_ast *ret = tach_malloc(sizeof(tach_ast));
    uint32_t alloc = 4;
    ret->children = tach_malloc(sizeof(tach_ast *) * alloc);
    ret->type = TACH_AST_TYPE_COMMAND;
    while (toks->at < toks->count
        && toks->types[toks->at] != TACH_TOKEN_END
        && toks->types[toks->at] != TACH_TOKEN_NEWLINE
        && toks->types[toks->at] != TACH_TOKEN_CLOSE
        && toks->at != toks->count) {
        if (ret->count + 2 > alloc) {
            alloc *= 1.5;
            ret->children = tach_realloc(ret->children, sizeof(tach_ast *) * alloc);
        }
        tach_ast *got = tach_parse_single(toks);
        ret->children[ret->count] = got;
        ret->count ++;
    }
    if (ret->count == 0) {
        ret->type = TACH_AST_TYPE_EMPTY;
        toks->at ++;
        return ret;
    }
    ret->children[0]->type = TACH_AST_TYPE_NAME;
    return ret;
}

tach_ast *tach_parse_body(tach_tokens *toks) {
    tach_ast *ret = tach_malloc(sizeof(tach_ast));
    ret->type = TACH_AST_TYPE_BODY;
    uint32_t alloc = 4;
    ret->children = tach_malloc(sizeof(tach_ast *) * alloc);
    while (toks->at < toks->count && toks->types[toks->at] != TACH_TOKEN_END) {
        if (ret->count + 2 > alloc) {
            alloc *= 1.5;
            ret->children = tach_realloc(ret->children, sizeof(tach_ast *) * alloc);
        }
        if (toks->types[toks->at] == TACH_TOKEN_NEWLINE) {
            toks->at ++;
        }
        else {
            tach_ast *got = tach_parse_command(toks);
            ret->children[ret->count] = got;
            ret->count ++;
        }
    }
    if (toks->at == toks->count) {
        ret->type = TACH_AST_TYPE_PROGRAM;
    }
    return ret;
}

tach_ast *tach_parse_tokens(tach_tokens *f) {
    f->at = 0;
    return tach_parse_body(f);
}