CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++23 -pedantic -ggdb -I./raylib-cpp/include
LIBS=-lraylib

all: main.o game.o game.hxx
	@$(CXX) $(CXXFLAGS) $(LIBS) $^ -o out
	@echo CXX $^

%.o: %.cxx Makefile
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo CXX $<

clean:
	rm *.o out

.PHONY: clean
