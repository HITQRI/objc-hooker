NUM_WORDS=40
DEF_FILE="definitions.o.c"



all: infinite_functions.o hooker.o objc_sizeof_type.o main.o
	clang $^ -Os -framework Foundation

main.o: main.m
	clang -c $<

hooker.o: hooker.c
	clang -c $<

objc_sizeof_type.o: objc_sizeof_type.c
	clang -c $<

infinite_functions.o: infinite_functions.c $(DEF_FILE)
	clang -c $< -Os

$(DEF_FILE): gcode.out
	./gcode.out $(NUM_WORDS) > $@

gcode.out: gcode.c
	gcc $< -o $@
