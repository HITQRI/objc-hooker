#import "hooker.h"
#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>

@interface Super : NSObject
@end

@implementation Super
-(NSString *)function:(char)c int:(int)x char:(char)d int:(int)i
{
    return [NSString stringWithFormat:@"orig super %d %c %c %d", x, c, d, i];
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

NSString *hook_super(Super *self, SEL _cmd, char c, int x, char d, int i)
{
    return [NSString stringWithFormat:@"hook super %d %c %c %d", x, c, d, i];
}

typedef NSString * (*imp_t)(id, SEL, char, int, char, int);
imp_t orig_bar;
NSString *hook_bar(Sub *self, SEL _cmd, char c, int x, char d, int i)
{
    return [NSString stringWithFormat:@"%@ '%@'", @"hook bar, super is", orig_bar(self, _cmd, c, x, d, i)];
}

void test(Class cls)
{
    const char *result = [[[[cls alloc] init] function:'a' int:5 char:'b' int:6] UTF8String];
    printf("Calling %s: \"%s\"\n", class_getName(cls), result);
}

typedef void (*ca_imp_t)(id, SEL, CGPoint);
ca_imp_t orig_layer;
ca_imp_t orig_calayer;

void hook_layer(id self, SEL _cmd, CGPoint pos)
{
    printf("hook layer\n");
    return orig_layer(self, _cmd, pos);
}

void hook_calayer(id self, SEL _cmd, CGPoint pos)
{
    printf("hook calayer\n");
    return orig_calayer(self, _cmd, pos);
}

int main(int argc, char *argv[])
{
    hook(Sub.class, @selector(function:int:char:int:), (IMP)hook_bar, (IMP *)&orig_bar);
    hook(Super.class, @selector(function:int:char:int:), (IMP)hook_super, NULL);
    printf("\n\n\n");
    test(Super.class);
    test(Sub.class);
    printf("\n\n\n");
    hook(Layer.class, @selector(setPosition:), (IMP)hook_layer, (IMP *)&orig_layer);
    hook(CALayer.class, @selector(setPosition:), (IMP)hook_calayer, (IMP *)&orig_calayer);

    printf("testing CALayer:\n");
    [[[CALayer alloc] init] setPosition:CGPointMake(2.0, 3.0)];
    printf("testing Layer:\n");
    [[[Layer alloc] init] setPosition:CGPointMake(1.0, 2.0)];

    return 0;
}
