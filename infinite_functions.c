// not sure if this is good practice,
// but the issue with substrate is that
// you need to dynamially allocate arguments for a function, so what im doing instead is generating a bunch of function type signatures. 

#include <stdio.h>
#include <objc/runtime.h>
#include "macros.h"

typedef void * word;

//#define PREFIX __substrate_test_

#define GET_STRUCT(X) PREFIX##X

#include "hooker.h"

IMP get_implementation_super(id self, SEL _cmd)
{
    debugf("getting super for <%p>...\n", self);
    Class super = class_getSuperclass(object_getClass(self));
    Method m;
    find_method(super, _cmd, &m);
    return method_getImplementation(m);
}

#define INITIAL_STRUCT() typedef word GET_STRUCT(i)
#define MAKE_STRUCT(X) typedef struct PREFIX##X##i {\
    GET_STRUCT(X) previous;\
    word lol;\
} PREFIX##X##i

#define GET_VOID_FUNC_VOID() void##PREFIX##void
#define GET_VOID_FUNC(ARG) void##PREFIX##ARG
#define GET_FUNC_VOID(RET) RET##PREFIX##void
#define GET_FUNC(RET, ARG) RET##PREFIX##ARG

#include "definitions.o.c"
