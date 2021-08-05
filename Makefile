CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow

INC = -I example/lib/ -I include/
SRC = $(wildcard src/*.cpp)

all:
	$(CXX) $(CXXFLAGS) $(INC) -c $(SRC) -o bin/6502.o
	$(CXX) $(CXXFLAGS) $(INC) -c example/example.cpp -o bin/example.o
	$(CXX) $(CXXFLAGS) bin/6502.o bin/example.o -o bin/example.exe -L example/lib/build -l sfml-graphics -l sfml-window -l sfml-system

run:
	@cmd /k "cd bin & example.exe & exit"	

clean:
	@cmd /k "cd bin & del example.exe & exit"
