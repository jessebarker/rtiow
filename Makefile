CXXFLAGS = -Wall -Werror -std=c++17 -pedantic -g
RTTARGET = rtiow
RTSRCS = main.cpp
LMINCS = ../
RTOBJS = $(LIBSRCS:.cpp=.o)

# Make sure to build both the library targets and the tests, and generate 
# a make failure if the tests don't pass.
default: $(RTTARGET)

$(RTTARGET): $(RTSRCS)
	$(CXX) $(CXXFLAGS) -I $(LMINCS) -o $@ $^

clean :
	$(RM) $(RTOBJS) $(RTTARGET)
