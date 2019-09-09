.DEFAULT_GOAL := goal

CPPFLAGS=$(shell llvm-config --cxxflags)
LDFLAGS=$(shell llvm-config --ldflags --libs)

goal: main.out

main.out: main.ll
	clang++ main.ll -o main.out

main.ll: main_ir_generator.out
	./main_ir_generator.out > main.ll

main_ir_generator.out: main.cpp
	clang++ $(LDFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o *.out *.ll
