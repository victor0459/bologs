CXX=g++
CFLAGS=-g -O2 -Wall -fPIC -Wno-deprecated 

INC=-I../include
LIB=-L../lib -lreactor -lpthread
OBJS = $(addsuffix .o, $(basename $(wildcard *.cc)))

all:
		        $(CXX) -o server $(CFLAGS)  server.cpp $(INC) $(LIB)
		        $(CXX) -o client $(CFLAGS)  client.cpp $(INC) $(LIB)

clean:
		        -rm -f *.o hello_lars lars_reactor_test

