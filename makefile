OBJS2 = WhoServer/build/server.o \
		WhoServer/build/myVector.o



# OBJS2 =
#
# OBJS3 =

CC = gcc
FLAGS = -Wextra -Wall -g -c

TARGET = master
TARGET2 = whoServer
TARGET3 = whoClient


all:  $(TARGET2)

clean:
	$(RM) -r $(TARGET2) WhoServer/build/*


WhoServer/build/server.o: WhoServer/src/server.c
	$(CC) $(FLAGS) $< -o $@

WhoServer/build/myVector.o: WhoServer/src/myVector.c
	$(CC) $(FLAGS) $< -o $@





$(TARGET2) : $(OBJS2)
	$(CC) $(CFLAGS) $^ -o $@ -lm

rebuild: clean all
