#include "infinite_functions.h"
#include "objc_sizeof_type.h"
#include <stdlib.h>
#include "hooker.h"
#include <stdbool.h>
#include <stdio.h>

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

    printf("-[%s %s]: %s\n", class_getName(cls), sel_getName(sel), class_has_method ? "HAS method, hooking" :  "DOES NOT have method, adding");

    if(class_has_method) {
        method_setImplementation(method, imp);
    } else {
        int ret = retsizes(method);
        int arg = sizes(method) - 16;

        *orig = (IMP)get_function(ret, arg);

        const char *type = method_getTypeEncoding(method);
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
    const char *type = method_getTypeEncoding(m);

    char rettype[BUFSIZ];
    method_getReturnType(m, rettype, sizeof(rettype));

    int arg = 0;

    for(int i = strlen(rettype); i < strlen(type); i++)
    {
        char c = type[i];
        if(c < '0' || c > '9') break;
        arg *= 10;
        arg += c - '0';
    }

    return arg;
}
