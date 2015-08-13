#include <stdio.h>
#include <string.h>

#define ERROR(...) fprintf(stderr, ## __VA_ARGS__)


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
                printf("MAKE_VOID_FUNC_VOID()\n");
            } else if(strlen(ret_str) == 0) {
                printf("MAKE_VOID_FUNC(%s)\n", arg_str);
            } else if(strlen(arg_str) == 0) {
                printf("MAKE_FUNC_VOID(%s)\n", ret_str);
            } else {
                printf("MAKE_FUNC(%s, %s)\n", ret_str, arg_str);
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
