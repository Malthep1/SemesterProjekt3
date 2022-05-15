SRCS=main.cpp
EXE=main
BASEPATH=../lib/host
INCPATH=../inc

# Determine whether this is a debug build or not
ifdef DEBUG
CFLAGS=-ggdb -O0
LIBPATH=$(BASEPATH)/debug
else
CFLAGS=-O2
LIBPATH=$(BASEPATH)/release
endif
# Setup the CFLAGS to ensure that the relevant warnings, includes and defines.
CFLAGS+=-Wall -D_REENTRANT -DOS_LINUX -I$(INCPATH)

# Note how the flags are part of the compilation of .cpp file(s).
main.o: $(SRCS)
	g++ $(CFLAGS) -c $(SRCS)

# Then again, note how the flags are NOT part of the linking process
main: main.o
	g++ -o main main.o -L$(LIBPATH) -lOSApi -lrt -lpthread

all: main

clean:
	rm -f *.o main
