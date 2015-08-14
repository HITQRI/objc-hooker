#import "hooker.h"
#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import "macros.h"

@interface Super : NSObject
@end

@implementation Super
-(NSString *)function:(char)c int:(int)x char:(char)d int:(int)i
{
    printf("[super %corig] %d %c %c %d\n", 37, x, c, d, i);

    return @"(return [super \%orig]";
}
@end

@interface Sub : Super
@end

@implementation Sub
@end


@interface Layer : CALayer
@end

@implementation Layer
@end
typedef NSString * (*imp_t)(id, SEL, char, int, char, int);

imp_t orig_super;
NSString *hook_super(Super *self, SEL _cmd, char c, int x, char d, int i)
{
    printf("[super %chook] %d %c %c %d\n", 37, x, c, d, i);
    return [NSString stringWithFormat:@"(return [super %chook] %@)", 37, orig_super(self, _cmd, c, x, d, i)];
}

imp_t orig_bar;
NSString *hook_bar(Sub *self, SEL _cmd, char c, int x, char d, int i)
{
    printf("[sub %chook] %d %c %c %d\n", 37, x, c, d, i);
    return [NSString stringWithFormat:@"(return [sub %chook] %@)", 37, orig_bar(self, _cmd, c, x, d, i)];
}

typedef void (*ca_imp_t)(id, SEL, CGPoint);
ca_imp_t orig_layer;
ca_imp_t orig_calayer;

void hook_layer(id self, SEL _cmd, CGPoint pos)
{
    purplec("hook layer (%f, %f)\n", pos.x, pos.y);
    return orig_layer(self, _cmd, pos);
}

void hook_calayer(id self, SEL _cmd, CGPoint pos)
{
    purplec("hook calayer (%f, %f)\n", pos.x, pos.y);
    return orig_calayer(self, _cmd, pos);
}

void test_step(Class cls, void (*t)(Class))
{
    greenc("testing ");
    bluec("%s", class_getName(cls));
    greenc(":\n");
    purplec();
    t(cls);
    normalc();
}

void test(Class sub, Class super, SEL _cmd, IMP hook_sub, IMP *orig_sub, IMP hook_super, IMP *orig_super, void (*t)(Class))
{
    yellowc("-----------------------------------------\n");
    bluec("%s", class_getName(sub));
    yellowc(" is a subclass of ");
    bluec("%s", class_getName(super));
    yellowc(", hooking ");
    bluec("%s", sel_getName(_cmd));
    normalc("\n");

    hook(sub, _cmd, hook_sub, orig_sub);
    hook(super, _cmd, hook_super, orig_super);

    test_step(sub, t);
    test_step(super, t);

    printf("\n");
}

void test_sub_super(Class cls)
{
    const char *result = [[[[cls alloc] init] function:'a' int:5 char:'b' int:6] UTF8String];
    printf("%s\n", result);
}

void test_layer(Class cls)
{
    [[[cls alloc] init] setPosition:CGPointMake(2.0, 3.0)];
}


int main(int argc, char *argv[])
{
    test(   Sub.class, Super.class,
            @selector(function:int:char:int:),
            (IMP)hook_bar, (IMP *)&orig_bar,
            (IMP)hook_super, (IMP *)&orig_super,
            test_sub_super
        );

    test(   Layer.class, CALayer.class,
            @selector(setPosition:),
            (IMP)hook_layer, (IMP *)&orig_layer,
            (IMP)hook_calayer, (IMP *)&orig_calayer,
            test_layer
        );

    yellowc("-----------------------------------------\n");

    return 0;
}
