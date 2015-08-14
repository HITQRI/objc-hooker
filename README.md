## The Problem:

so the issue with maintaining substrate's obj-c part is that it's fairly platform specific.

the reason why it's platform specific is because of this:

```objc
@interface Super
-(void)func;
@end

@interface Sub : Super
@end
```

say you hook the subclasses func, then the superclasses.

what will happen is when you call %orig on the subclasses func, you'll get not the hooked superclasses function, but the original one. bad.

## General solution:

what people usually do to fix this is with some assembly voodoo.

## My solution:

but i found a "portable" way of doing it.... "portable" as in, it depends on undefined behavior. but iOS frameworks use clang, so whatevs.

so basically, i set the original to this:

```objc
NSString *orig(id self, SEL _cmd)
{
    Class super = class_getSuperclass(self);
    Method m;
    find_method(super, _cmd, &m);
    IMP i = method_getImplementation(m);

    return i(self, _cmd);
}
```


what about functions with other type signatures?

well.... i generated a lot of single possible c function type signatures... like this:

```objc
typedef void * word;

void with_2args(word a, word b)
{
    //...
}

void with_3args(word a, word b, word c)
{
    //...
}
```

the only way this wouldn't work:

* You have over a kilobit worth of arguments (not happening in Apple frameworks)
* You are hooking a function that passes a packed struct as an argument (not very likely)

if you're trying to hook a function with more than a kilobit worth of arguments. you change ```NUM_BYTES=``` at the top of the makefile (warning: build time will exponentially grow)

______

the only non-portable thing in here is objc_sizeof_type.c, which i took from WinObjC... so microsoft will take care of that for us.
