

#define printc(...) printf("\033[" __VA_ARGS__)

#define normalc(...) printc("0m" __VA_ARGS__);
#define redc(...) printc("0;31m" __VA_ARGS__)
#define greenc(...) printc("0;32m" __VA_ARGS__)
#define brownc(...) printc("0;33m" __VA_ARGS__)
#define bluec(...) printc("0;34m" __VA_ARGS__)
#define purplec(...) printc("0;35m" __VA_ARGS__)
#define cyanc(...) printc("0;36m" __VA_ARGS__)

#define yellowc(...) printc("1;33m" __VA_ARGS__)

//#define debugf redc

#ifndef debugf
#define debugf(...)
#endif
