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

what people usually do to fix this is with some assembly voodoo.

but i found a "portable" way of doing it....

instead, i set the original to this:

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


now you may be thinking... what about functions with other type signatures?

well.... i generated a metric fuckton of single possible c function type signatures. the only way this wouldn't work is if you're trying to hook a function with a kilobit worth of arguments. if for some crazy reason you want more leniency then change ```NUM_BYTES=``` at the top of the makefile (warning: build time will be longer)

______

the only non-portable thing in here is objc_sizeof_type.c, which i took from WinObjC... so microsoft will take care of that for us.
