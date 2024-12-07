CXX = g++ 
CXXFLAGS = -Wall -std=c++17


all: my_program

my_program: main.o
	$(CXX) $(CXXFLAGS) -o my_program main.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
	rm -f my_program main.o