TEST_DIR=./test
NET_DIR=./net
UTIL_DIR=./util
DB_DIR=./db
BUILD_DIR=./build
ARG= -std=c++11 -Wall -g
OBJ= Logger.o Fdevent.o Ionet.o helper.o Epoller.o EventLoop.o hashtable.o parse.o
CC=clang++
# CC=g++

all:$(OBJ)
	$(CC) $(ARG) $(TEST_DIR)/Ionet_test.cc $(BUILD_DIR)/*.o -o ./mydb

helper.o:
	$(CC) $(ARG) -c $(UTIL_DIR)/helper.cc -o $(BUILD_DIR)/helper.o

Logger.o:
	$(CC) $(ARG) -c $(UTIL_DIR)/Logger.cc  -o $(BUILD_DIR)/Logger.o

Fdevent.o:
	$(CC) $(ARG) -c $(NET_DIR)/Fdevent.cc -o $(BUILD_DIR)/Fdevent.o

Epoller.o:
	$(CC) $(ARG) -c $(NET_DIR)/Epoller.cc -o $(BUILD_DIR)/Epoller.o

EventLoop.o:
	$(CC) $(ARG) -c $(NET_DIR)/EventLoop.cc -o $(BUILD_DIR)/EventLoop.o

Ionet.o:
	$(CC) $(ARG) -c $(NET_DIR)/Ionet.cc -o $(BUILD_DIR)/Ionet.o

hashtable.o:
	$(CC) $(ARG) -c $(DB_DIR)/hashtable.cc -o $(BUILD_DIR)/hashtable.o

parse.o:
	$(CC) $(ARG) -c $(UTIL_DIR)/parse.cc -o $(BUILD_DIR)/parse.o
clean:
	rm ./build/*.o;
