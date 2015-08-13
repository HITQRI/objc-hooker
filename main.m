#import "hooker.h"
#import <Foundation/Foundation.h>

@interface Super : NSObject
@end

@implementation Super
-(NSString *)function:(int)x
{
    return [NSString stringWithFormat:@"orig super %d", x];
}
@end

@interface Sub : Super
@end

@implementation Sub
@end

NSString *hook_super(Super *self, SEL _cmd, int x)
{
    return [NSString stringWithFormat:@"hook super %d", x];
}

typedef NSString * (*imp_t)(id, SEL, int);
imp_t orig_bar;
NSString *hook_bar(Sub *self, SEL _cmd, int x)
{
    return [NSString stringWithFormat:@"%@ '%@'", @"hook bar, super is", orig_bar(self, _cmd, x)];
}

void test(Class cls)
{
    const char *result = [[[[cls alloc] init] function:5] UTF8String];
    printf("Calling %s: \"%s\"\n", class_getName(cls), result);
}

int main(int argc, char *argv[])
{
    hook(Sub.class, @selector(function:), (IMP)hook_bar, (IMP *)&orig_bar);
    hook(Super.class, @selector(function:), (IMP)hook_super, NULL);
    printf("\n\n\n");
    test(Super.class);
    test(Sub.class);

    return 0;
}
