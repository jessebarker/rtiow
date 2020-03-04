CXXFLAGS = -Wall -Werror -std=c++11 -pedantic -g
RTTARGET = rtiow
RTSRCS = main.cpp
RTOBJS = $(LIBSRCS:.cpp=.o)

default: $(RTTARGET)

image.o: image.cpp image.h
	$(CXX) $(CXXFLAGS) -I stb -c image.cpp

RTHEADERS := $(wildcard *.h) 

$(RTTARGET): $(RTSRCS) $(RTHEADERS) image.o
	$(CXX) $(CXXFLAGS) image.o -o $@ $(RTSRCS)

clean :
	$(RM) $(RTOBJS) $(RTTARGET)
