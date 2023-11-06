TARGET = dc
CC = clang++
#CC = g++
CXXFLAGS = -Wall -Wextra -Werror -pedantic-errors -fstack-protector-strong \
	-D_FORTIFY_SOURCE=2 -Wformat-security -fsanitize=address -fsanitize=undefined \
	-fstack-clash-protection -Wundef -Wshadow -Wpointer-arith \
	-Wcast-align -Wwrite-strings -ftrapv -std=c++20 -O3

all: $(TARGET)

$(TARGET): main.o eval.a math.a stack.a macro.a
	$(CC) $(CXXFLAGS) $^ -o $@

main.o: main.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

eval.a: eval.o
	ar rcs $@ $^

math.a: math.o
	ar rcs $@ $^

stack.a: stack.o
	ar rcs $@ $^

macro.a: macro.o
	ar rcs $@ $^

eval.o: src/eval.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

math.o: src/math.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

stack.o: src/stack.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

macro.o: src/macro.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.a src/*.gch $(TARGET)
