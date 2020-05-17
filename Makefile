CXXFLAGS = -Wall -Werror -std=c++11 -pedantic -O3
RTTARGET = rtiow
RTSRCS = main.cpp

default: $(RTTARGET)

image.o: image.cpp image.h
	$(CXX) $(CXXFLAGS) -I stb -c image.cpp

RTHEADERS := $(wildcard *.h) 

$(RTTARGET): $(RTSRCS) $(RTHEADERS) image.o
	$(CXX) $(CXXFLAGS) image.o -o $@ $(RTSRCS)

clean :
	$(RM) $(RTTARGET) *.o
