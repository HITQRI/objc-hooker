// not sure if this is good practice,
// but the issue with substrate is that
// you need to dynamially allocate arguments for a function, so what im doing instead is generating a bunch of function type signatures. 
// since it's apple we're talking about, most functions will
// not have more than the equivalent of 8 ints as the return value, or 8 ints as the number of arguments.

#include <stdio.h>

#define BYTE char

//#define PREFIX __substrate_test_

#define GET_STRUCT(X) PREFIX##X

#define INITIAL_STRUCT() typedef BYTE GET_STRUCT(i)
#define MAKE_STRUCT(X) typedef struct PREFIX##X##i {\
    GET_STRUCT(X) previous;\
    BYTE lol;\
} PREFIX##X##i

#define GET_VOID_FUNC_VOID() void##PREFIX##void
#define GET_VOID_FUNC(ARG) void##PREFIX##ARG
#define GET_FUNC_VOID(RET) RET##PREFIX##void
#define GET_FUNC(RET, ARG) RET##PREFIX##ARG

#define MAKE_VOID_FUNC_VOID() void GET_VOID_FUNC_VOID()(){}
#define MAKE_VOID_FUNC(ARG) void GET_VOID_FUNC(ARG)(GET_STRUCT(ARG) args){}
#define MAKE_FUNC_VOID(RET) GET_STRUCT(RET) GET_FUNC_VOID(RET)(){\
    GET_STRUCT(RET) lol;\
    return lol;\
}
#define MAKE_FUNC(RET, ARG) GET_STRUCT(RET) GET_FUNC(RET, ARG)(GET_STRUCT(ARG) args){\
    GET_STRUCT(RET) lol;\
    return lol;\
}

#include "definitions.o.c"

#define PRINT_SIZE(X) printf("%lu\n", sizeof(GET_STRUCT(X)))

int main(int argc, char *argv[])
{

    return 0;
}
