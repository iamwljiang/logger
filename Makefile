build-all:build static 
LIB=-lpthread
INC=-I.
CFLAG=-g -Wall
CXX=g++

SOURCE=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SOURCE))
OBJ=$(filter-out main.o,$(OBJS))
.cpp.o:$(SOURCE)
	$(CXX) $(CFLAG) -c $< -o $@

build:$(OBJ)
	$(CXX) $(CFLAG) $(OBJ) main.cpp -o test_logger $(LIB)
static:$(OBJ)
	ar rcs liblogger.a $(OBJ)
clean:
	rm -rf *.o
	rm -rf run.log*
	rm -rf multi*.log*

.PHONY:build-all
