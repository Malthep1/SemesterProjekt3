# Quick and dirty (does not handle changes in h-file)
SRCS=main.cpp KeyBoardInput.cpp LogSystem.cpp
OBJS=$(SRCS:.cpp=.o)
BASEPATH=..
CXX=arm-rpizw-g++
#for rpizw CXX = arm-rpizw-g++
# 

# Determine whether this is a debug build or not
ifdef DEBUG
#CXXFLAGS=-ggdb -O0
LIBPATH=$(BASEPATH)/lib/target/debug
else
CXXFLAGS=-O2
LIBPATH=$(BASEPATH)/lib/target/release
endif
# Setup the CFLAGS to ensure that the relevant warnings, includes and defines.
CXXFLAGS+=-DOS_LINUX -I$(BASEPATH)/inc

%.o:%cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^
# Then again, note how the flags are NOT part of the linking process
main: $(OBJS)
	$(CXX) -o main $(OBJS)  -L$(LIBPATH) -lOSApi -lrt -lpthread  

all: main

clean:$(OBJS)
	rm -f *.o main
