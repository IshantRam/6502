CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow

INC = -I include/
SRC = $(wildcard src/*.cpp)

all:
	$(CXX) $(CXXFLAGS) $(INC) $(SRC) example/example.cpp -o bin/example.exe

run:
	bin/example.exe

clean:
	rm bin/*.*