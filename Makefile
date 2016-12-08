main: main.o SomeFunction.o
	g++ -std=c++11 -o main main.o SomeFunction.o

.cpp.o:
	g++ -std=c++11 -c $< -o $@
