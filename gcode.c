#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define ERROR(...) fprintf(stderr, ## __VA_ARGS__)


#define INCLUDE_TYPE 1
#define INCLUDE_NAME 2
#define INCLUDE_BOTH 3
void print_args(size_t len, int include)
{
    for(int i = 0; i < len; i++) {
        if(INCLUDE_TYPE & include) {
            printf("word");
        }

        if(INCLUDE_NAME & include) {
            if(INCLUDE_TYPE & include) {
                printf(" ");
            }
            for(int j = 0; j <= i; j++) {
                printf("i");
            }
        }

        if(i < len - 1) {
            printf(", ");
        }
    }
}

void print_ret_type(const char *ret)
{
    if(strlen(ret) == 0) {
        printf("void");
    } else {
        printf("GET_STRUCT(%s)", ret);
    }
}

void print_func_body(const char *ret, size_t arg)
{

    printf(
"{\n"
"    IMP imp = get_implementation_super((id)i, (SEL)ii);\n"
"    "
    );
    print_ret_type(ret);
    printf(" (*tmp_t)(");
    print_args(arg, INCLUDE_TYPE);
    printf(") = (");
    print_ret_type(ret);
    printf(" (*)(");
    print_args(arg, INCLUDE_TYPE);
    printf("))imp;\n");
    printf(
"    return tmp_t("
    );
    print_args(arg, INCLUDE_NAME);
    printf(");\n");

    printf(
"}\n"
    );
}

int main(int argc, char *argv[])
{
    if(argc != 2) {
        ERROR("%s numwords\n", argv[0]);
        return 1;
    }

    int numwords = 0;

    for(int i = 0; i < strlen(argv[1]); i++) {
        char c = argv[1][i];
        if(c < '0' || c > '9') {
            ERROR("%s numwords\n", argv[0]);
            return 1;
        }

        numwords *= 10;
        numwords += c - '0';
    }

    if(numwords < 1) {
        ERROR("numwords must be bigger than 0\n");
    }

    printf(
"//create the structs\n"
"INITIAL_STRUCT();\n"
    );

    char istr[numwords + 2];
    strcpy(istr, "");

    while(strlen(istr) < numwords - 1) {
        strcat(istr, "i");
        printf("MAKE_STRUCT(%s);\n", istr);
    }

    printf("\n\n\n//create the functions\n\n");

    char arg_str[numwords + 2];
    char ret_str[numwords + 2];
    strcpy(arg_str, "");
    while(strlen(arg_str) <= numwords) {
        strcpy(ret_str, "");
        while(strlen(ret_str) <= numwords) {
            if(strlen(arg_str) == 0 && strlen(ret_str) == 0) {
                printf("void * GET_VOID_FUNC_VOID() = NULL;\n");
            } else if(strlen(ret_str) == 0) {
                if(strlen(arg_str) == 1) {
                    printf("void * GET_VOID_FUNC(%s) = NULL;\n", arg_str);
                } else {
                    printf("void GET_VOID_FUNC(%s)(", arg_str);
                    print_args(strlen(arg_str), INCLUDE_BOTH);
                    printf(")\n");
                    print_func_body(ret_str, strlen(arg_str));
                }
            } else if(strlen(arg_str) == 0) {
                printf("void * GET_FUNC_VOID(%s) = NULL;\n", ret_str);
            } else {
                if(strlen(arg_str) == 1) {
                    printf("void * GET_FUNC(%s, %s) = NULL;\n", ret_str, arg_str);
                } else {
                    printf("GET_STRUCT(%s) GET_FUNC(%s, %s)(", ret_str, ret_str, arg_str);
                    print_args(strlen(arg_str), INCLUDE_BOTH);
                    printf(")\n");
                    print_func_body(ret_str, strlen(arg_str));
                }
            }
            strcat(ret_str, "i");
        }
        strcat(arg_str, "i");
    }
    printf(
"\n\n\n"
"//and finally, wrap it all up in a nice function call\n"
"void * get_function(size_t ret, size_t arg)\n"
"{\n"
"    if(ret > %d || arg > %d) {\n"
"        return NULL;\n"
"    } else if(ret == 0 && arg == 0) {\n"
"        return GET_VOID_FUNC_VOID();\n"
"    } else if(ret == 0) {\n" //argsize is an actual number
"        switch(arg) {\n"
    , numwords, numwords);

    strcpy(istr, "");
    while(strlen(istr) < numwords) {
        strcat(istr, "i");
        printf(
"        case %lu: return GET_VOID_FUNC(%s);\n"
        , strlen(istr), istr);
    }
    printf(
"        default: NULL;\n"
"        }\n"
"    } else if(arg == 0) {\n" //retsize is an actual number
"        switch(ret) {\n"
    );

    strcpy(istr, "");
    while(strlen(istr) < numwords) {
        strcat(istr, "i");
        printf(
"        case %lu: return GET_FUNC_VOID(%s);\n"
        , strlen(istr), istr);
    }
    printf(
"        default: NULL;\n"
"        }\n"
"    } else {\n"
"        switch(arg) {\n"
    );

    //THE HARDEST PART!!!!!!
    strcpy(arg_str, "");
    while(strlen(arg_str) < numwords) {
        strcat(arg_str, "i");
        strcpy(ret_str, "");
        printf(
"        case %lu:\n"
"            switch(ret) {\n"
        , strlen(arg_str));
        while(strlen(ret_str) < numwords) {
            strcat(ret_str, "i");
            printf(
"            case %lu: return GET_FUNC(%s, %s);\n"
            , strlen(ret_str), ret_str, arg_str);
        }
        printf(
"            default: return NULL;\n"
"            }\n"
        );
    }

    printf(
"        default: return NULL;\n"
"        }\n"
"    }\n"
"    return NULL;\n"
"}\n"
    );


}
