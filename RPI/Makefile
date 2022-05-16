# Quick and dirty (does not handle changes in h-file)
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
CXX=g++
BASEPATH=../inc/OSApiStudent/
LIBPATH=$(BASEPATH)lib/host/release/
# Determine whether this is a debug build or not
ifdef DEBUG
CXXFLAGS=-ggdb -O0
else
CXXFLAGS=-O2
endif
# Setup the CFLAGS to ensure that the relevant warnings, includes and defines.
CXXFLAGS+=-DOS_LINUX -I$(BASEPATH)inc
LNKFLAGS=-L$(LIBPATH) -lOSApi -lrt -lpthread

$(OBJS):$(SRCS)
	$(CXX) $(CXXFLAGS) -c $@ $^
# Then again, note how the flags are NOT part of the linking process
main:$(OBJS)
	$(CXX) -o main $(OBJS) -L$(LIBPATH) -lOSApi -lrt -lpthread

all:main

clean:
	rm -f *.o main
