#!/usr/bin/env python2

import sys

def get_int(s):
    try:
        x = int(s)
        return x
    except ValueError:
        return False

err = sys.argv[0] + " numwords"

def error(e=err):
    print e
    exit(1)

if len(sys.argv) != 2:
    error()

numwords = get_int(sys.argv[1])

if numwords is False:
    error()
elif numwords < 1:
    error("numwords must be bigger than 0")


print("//create the structs\n")

print("INITIAL_STRUCT();")

istr = ""
while len(istr) < numwords - 1:
    istr += "i"
    print("MAKE_STRUCT(" + istr + ");")

print("")
print("")
print("")
print("//create the functions\n")

arg_str = ""
while len(arg_str) <= numwords:
    ret_str = ""
    while len(ret_str) <= numwords:
        if len(arg_str) == 0 and len(ret_str) == 0:
            print("MAKE_VOID_FUNC_VOID()")
        elif len(ret_str) == 0:
            print("MAKE_VOID_FUNC(" + arg_str + ")")
        elif len(arg_str) == 0:
            print("MAKE_FUNC_VOID(" + ret_str + ")")
        else:
            print("MAKE_FUNC(" + ret_str + ", " + arg_str + ")")

        ret_str += "i"
    arg_str += "i"
print('''


        //and finally, wrap it all up in a nice function call
        void * get_function(size_t returnsize, size_t argsize)
        {
            if('''+str(numwords)+' < returnsize || '+str(numwords)+''' < argsize) {
                return NULL;
            } else if(returnsize == 0 && argsize == 0) {
                return GET_VOID_FUNC_VOID();
            } else if(returnsize == 0) { //argsize is an actual number
                switch(argsize) {''')
istr = ""
while len(istr) < numwords:
    istr += "i"
    print("                case " + str(len(istr)) + ": return GET_VOID_FUNC(" + istr + ");")
print('''                default: return NULL;
                }
            } else if(argsize == 0) { //returnsize is an actual number
                switch(returnsize) {''')
istr = ""
while len(istr) < numwords:
    istr += "i"
    print("                case " + str(len(istr)) + ": return GET_FUNC_VOID(" + istr + ");")
print('''                default: return NULL;
                }
            } else {
                return NULL;
            }
        }
''')

# you need switch statements here

