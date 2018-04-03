CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
CLASSES=

all: my-router

my-router: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp routerFunctions.cpp

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM my-router *.tar.gz

tarball: clean
	tar -cvf project2_15315136.tar.gz
