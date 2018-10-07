PROGRAM   = main
CXX       = g++
CXXFLAGS  = -g -std=c++17 -Wall \
            -I../../3rd-party/ericniebler/range-v3/include \
            -I../../3rd-party/nlohmann/json/include \
            -fexceptions
LD_FLAGS  = -lboost_system

$(PROGRAM): main.o service.o
	$(CXX) $(LD_FLAGS)  -o $(PROGRAM) main.o service.o

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)


