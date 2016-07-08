PROGRAM   = main
CXX       = g++
CXXFLAGS  = -g -std=c++0x -Wall -pedantic -Wuninitialized -Wmaybe-uninitialized

$(PROGRAM): main.o
	$(CXX) -o $(PROGRAM) main.o

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)


