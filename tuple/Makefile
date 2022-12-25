CXX=g++
CXXFLAGS=-std=c++17 -I .
DEPS = tuple.h

.PHONY: all
all: tuple ebco sfinae

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

tuple: tuple.o
	$(CXX) -o tuple.exe tuple.o

ebco: ebco.o
	$(CXX) -o ebco.exe ebco.o

sfinae: sfinae.o
	$(CXX) -o sfinae.exe sfinae.o

.PHONY: clean
clean:
	rm -f *.o tuple.exe ebco.exe sfinae.exe