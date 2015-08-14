NUM_BYTES=64
DEF_FILE=definitions.o.c



all: infinite_functions.o hooker.o objc_sizeof_type.o main.o
	clang $^ -framework Foundation -framework QuartzCore

clean:
	rm -f infinite_functions.o hooker.o objc_sizeof_type.o main.o $(DEF_FILE) gcode.out a.out

main.o: main.m macros.h
	clang -c $<

hooker.o: hooker.c macros.h
	clang -c $<

objc_sizeof_type.o: objc_sizeof_type.c macros.h
	clang -c $<

infinite_functions.o: infinite_functions.c $(DEF_FILE) macros.h
	clang -c $<

$(DEF_FILE): gcode.out
	./gcode.out $(NUM_BYTES) > $@

gcode.out: gcode.c macros.h
	gcc $< -o $@
