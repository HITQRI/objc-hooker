NUM_WORDS=40
DEF_FILE="definitions.o.c"



all: infinite_functions.o main.o
	clang $^ -Os -framework Foundation

main.o: main.c
	clang -c $<

infinite_functions.o: infinite_functions.c $(DEF_FILE)
	clang -c $< -Os

$(DEF_FILE): gcode.out
	./gcode.out $(NUM_WORDS) > $@

gcode.out: gcode.c
	gcc $< -o $@
