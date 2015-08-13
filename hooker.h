#include <objc/runtime.h>

BOOL find_method(Class orig, SEL sel, Method *result);
BOOL hook(Class cls, SEL sel, IMP imp, IMP *orig);
