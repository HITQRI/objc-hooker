NUM_BYTES=128
DEF_FILE="definitions.o.c"



all: infinite_functions.o hooker.o objc_sizeof_type.o main.o
	clang $^ -framework Foundation

main.o: main.m
	clang -c $<

hooker.o: hooker.c
	clang -c $<

objc_sizeof_type.o: objc_sizeof_type.c
	clang -c $<

infinite_functions.o: infinite_functions.c $(DEF_FILE)
	clang -c $<

$(DEF_FILE): gcode.out
	./gcode.out $(NUM_BYTES) > $@

gcode.out: gcode.c
	gcc $< -o $@
