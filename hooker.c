#include "infinite_functions.h"
#include "objc_sizeof_type.h"
#include <stdlib.h>
#include "hooker.h"
#include <stdbool.h>
#include <stdio.h>

#include "macros.h"

typedef void * word;

int sizes(Method);
int retsizes(Method);

BOOL find_method(Class orig, SEL sel, Method *result) {
    for(Class cls = orig; cls != nil; cls = class_getSuperclass(cls)) {
        unsigned int size;
        Method *methods = class_copyMethodList(cls, &size);
        if(methods == NULL)
            continue;

        for(int i = 0; i != size; i++) {
            Method m = methods[i];
            if(method_getName(m) == sel) {
                free(methods);
                *result = m;
                return cls == orig;
            }
        }

        free(methods);
    }
    *result = nil;
    return false;
}

static BOOL FORCE_SET_IMPL = false;
BOOL hook(Class cls, SEL sel, IMP imp, IMP *orig)
{
    Method method;
    BOOL class_has_method = find_method(cls, sel, &method);

    if(method == nil) {
        fprintf(stderr, "method not found!!!!111!!\n");
        return false;
    }

    if(orig != NULL) {
        *orig = method_getImplementation(method);
    }

    bluec("-[%s %s]: ", class_getName(cls), sel_getName(sel));
    greenc("%s\n", class_has_method ? "HAS method, hooking" :  "DOES NOT have method, adding");
    normalc();

    if(class_has_method) {
        method_setImplementation(method, imp);
    } else {
        int ret = (retsizes(method) + sizeof(word) - 1)/sizeof(word);
        int arg = (sizes(method) + sizeof(word) - 1)/sizeof(word);

        *orig = (IMP)get_function(ret, arg);

        const char *type = method_getTypeEncoding(method);
        debugf("type: %s ret: %d arg: %d\n", type, ret, arg);
        return class_addMethod(cls, sel, imp, type);
    }

    return true;
}

int retsizes(Method m)
{
    char rettype[BUFSIZ];
    method_getReturnType(m, rettype, sizeof(rettype));
    return objc_sizeof_type(rettype);
}

int sizes(Method m)
{
    int biggestTypeSize = 0;
    int numArguments = method_getNumberOfArguments(m);
    for(int i = 0; i < numArguments; i++) {
        char type[BUFSIZ];
        method_getArgumentType(m, i, type, sizeof(type));

        int typeSize = objc_sizeof_type(type);
        if(typeSize > biggestTypeSize) {
            biggestTypeSize = typeSize;
        }
    }

    return biggestTypeSize*numArguments;
}
