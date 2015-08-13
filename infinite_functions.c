// not sure if this is good practice,
// but the issue with substrate is that
// you need to dynamially allocate arguments for a function, so what im doing instead is generating a bunch of function type signatures. 
// since it's apple we're talking about, most functions will
// not have more than the equivalent of 8 ints as the return value, or 8 ints as the number of arguments.

#include <stdio.h>
#include <objc/runtime.h>

#define BYTE char

//#define PREFIX __substrate_test_

#define GET_STRUCT(X) PREFIX##X

void find_method(Class cls, SEL _cmd, Method * m)
{

}

IMP get_implementation_super(id self, SEL _cmd)
{
    Class super = class_getSuperclass(object_getClass(self));
    Method m;
    find_method(super, _cmd, &m);
    return NULL;
    return method_getImplementation(m);
}

#define INITIAL_STRUCT() typedef BYTE GET_STRUCT(i)
#define MAKE_STRUCT(X) typedef struct PREFIX##X##i {\
    GET_STRUCT(X) previous;\
    BYTE lol;\
} PREFIX##X##i

#define GET_VOID_FUNC_VOID() void##PREFIX##void
#define GET_VOID_FUNC(ARG) void##PREFIX##ARG
#define GET_FUNC_VOID(RET) RET##PREFIX##void
#define GET_FUNC(RET, ARG) RET##PREFIX##ARG

#define MAKE_VOID_FUNC_VOID() static void GET_VOID_FUNC_VOID()(id self, SEL _cmd){\
    IMP i = get_implementation_super(self, _cmd);\
    void (*tmp_t)(id, SEL);\
    tmp_t = (void (*)(id, SEL))i;\
    tmp_t(self, _cmd);\
    \
}
#define MAKE_VOID_FUNC(ARG) static void GET_VOID_FUNC(ARG)(id self, SEL _cmd, GET_STRUCT(ARG) args){\
    IMP i = get_implementation_super(self, _cmd);\
    void (*tmp_t)(id, SEL, GET_STRUCT(ARG));\
    tmp_t = (void (*)(id, SEL, GET_STRUCT(ARG)))i;\
    tmp_t(self, _cmd, args);\
}
#define MAKE_FUNC_VOID(RET) static GET_STRUCT(RET) GET_FUNC_VOID(RET)(id self, SEL _cmd){\
    IMP i = get_implementation_super(self, _cmd);\
    GET_STRUCT(RET) (*tmp_t)(id, SEL);\
    tmp_t = (GET_STRUCT(RET) (*)(id, SEL))i;\
    return tmp_t(self, _cmd);\
}
#define MAKE_FUNC(RET, ARG) static GET_STRUCT(RET) GET_FUNC(RET, ARG)(id self, SEL _cmd, GET_STRUCT(ARG) args){\
    IMP i = get_implementation_super(self, _cmd);\
    GET_STRUCT(RET) (*tmp_t)(id, SEL, GET_STRUCT(ARG));\
    tmp_t = (GET_STRUCT(RET) (*)(id, SEL, GET_STRUCT(ARG)))i;\
    return tmp_t(self, _cmd, args);\
}

#include "definitions.o.c"
