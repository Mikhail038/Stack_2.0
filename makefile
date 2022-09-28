CXX_FLAGS = -D _DEBUG -g -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE

all: GREEN

stack2: stack.o f_stack.o
	g++ stack.o f_stack.o -o stack $(CXX_FLAGS)

stack.o: stack.cpp
	g++ -c -o stack.o stack.cpp $(CXX_FLAGS)

f_stack.o: f_stack.cpp
	g++ -c -o f_stack.o f_stack.cpp $(CXX_FLAGS)

clean:
	rm -rf *.o stack2


pstack: pstack.o pf_stack.o
	g++ pstack.o pf_stack.o -o stack $(CXX_FLAGS)

pstack.o: stack.cpp
	g++ -DPURPLE -c -o pstack.o stack.cpp $(CXX_FLAGS)

pf_stack.o: f_stack.cpp
	g++ -DPURPLE -c -o pf_stack.o f_stack.cpp $(CXX_FLAGS)


rstack: rstack.o rf_stack.o
	g++ rstack.o rf_stack.o -o stack $(CXX_FLAGS)

rstack.o: stack.cpp
	g++ -DRED -c -o rstack.o stack.cpp $(CXX_FLAGS)

rf_stack.o: f_stack.cpp
	g++ -DRED -c -o rf_stack.o f_stack.cpp $(CXX_FLAGS)


ostack: ostack.o of_stack.o
	g++ ostack.o of_stack.o -o stack $(CXX_FLAGS)

ostack.o: stack.cpp
	g++ -DORANGE -c -o ostack.o stack.cpp $(CXX_FLAGS)

of_stack.o: f_stack.cpp
	g++ -DORANGE -c -o of_stack.o f_stack.cpp $(CXX_FLAGS)


ystack: ystack.o yf_stack.o
	g++ ystack.o yf_stack.o -o stack $(CXX_FLAGS)

ystack.o: stack.cpp
	g++ -DYELLOW -c -o ystack.o stack.cpp $(CXX_FLAGS)

yf_stack.o: f_stack.cpp
	g++ -DYELLOW -c -o yf_stack.o f_stack.cpp $(CXX_FLAGS)


gstack: gstack.o gf_stack.o
	g++ gstack.o gf_stack.o -o stack $(CXX_FLAGS)

gstack.o: stack.cpp
	g++ -DGREEN -c -o gstack.o stack.cpp $(CXX_FLAGS)

gf_stack.o: f_stack.cpp
	g++ -DGREEN -c -o gf_stack.o f_stack.cpp $(CXX_FLAGS)


PURPLE: pstack

RED: rstack

ORANGE: ostack

YELLOW: ystack

GREEN: gstack
