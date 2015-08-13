NUM_WORDS=8
DEF_FILE="definitions.o.c"



all: infinite_functions.o
	clang $^

infinite_functions.o: infinite_functions.c $(DEF_FILE)
	clang -c $<

$(DEF_FILE): gcode.out
	./gcode.out $(NUM_WORDS) > $@

gcode.out: gcode.c
	gcc $< -o $@
