SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)
EXE= main
BASEPATH=../../inc/OSApiStudent
LIBPATH=$(BASEPATH)/lib/host/release
CXXFLAGS=-DOS_LINUX -I$(BASEPATH)/inc
LNKFLAGS=-L$(LIBPATH) -lOSApi -lrt -lpthread



$(OBJ):$(SRC)
	$(CXX) $(CXXFLAGS) -c $^

main:$(OBJ)
	$(CXX) -o main $(OBJ) -L$(LIBPATH) -lOSApi -lrt -lpthread

all:main

clean:
	rm -f *.o main


