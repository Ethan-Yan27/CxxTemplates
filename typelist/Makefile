CXX=g++
CXXFLAGS=-std=c++17 -I .
DEPS = typelist.h

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

hello_typelist: typelist.o
	$(CXX) -o hello_typelist.exe typelist.o

.PHONY: clean
clean:
	rm -f *.o hello_typelist