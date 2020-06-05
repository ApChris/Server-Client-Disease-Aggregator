OBJS2 = WhoServer/build/server.o \
		WhoServer/build/myVector.o



OBJS3 = WhoClient/build/client.o \
#
# OBJS3 =

CC = gcc
FLAGS = -Wextra -Wall -g -c

TARGET = master
TARGET2 = whoServer
TARGET3 = whoClient


all:  $(TARGET2) $(TARGET3)

clean:
	$(RM) -r $(TARGET2) $(TARGET3) WhoClient/build/* WhoServer/build/*






# SERVER
WhoServer/build/server.o: WhoServer/src/server.c
	$(CC) $(FLAGS) $< -o $@

WhoServer/build/myVector.o: WhoServer/src/myVector.c
	$(CC) $(FLAGS) $< -o $@



# CLIENT
WhoClient/build/client.o: WhoClient/src/client.c
	$(CC) $(FLAGS) $< -o $@



$(TARGET2) : $(OBJS2)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(TARGET3) : $(OBJS3)
	$(CC) $(CFLAGS) $^ -o $@ -lm

rebuild: clean all
