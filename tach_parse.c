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
        }
        if (got == '\t' || got == ' ') {
            got = getc(f);
        }
        else if (got == '\r' || got == '\n' || got == ';') {
            ret->names[ret->count] = ";;";
            ret->types[ret->count] = TACH_TOKEN_NUMBER; 
            ret->count ++;
            got = getc(f);
        }
        else if (got == '$') {
            ret->names[ret->count] = "$";
            ret->types[ret->count] = TACH_TOKEN_DOLLAR;
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
            printf("error: cannot lex");
            exit(1);
        }
    }
    for (uint32_t i = 0; i < ret->count; i++) {
        puts(ret->names[i]);
    }
    return ret;
}