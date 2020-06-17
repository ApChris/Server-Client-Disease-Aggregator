OBJS = 	Master/build/diseaseAggregator.o \
		Master/build/parentNamedPipesFifo.o \
		Master/build/list.o \
		Master/build/parentFunctions.o \
		Master/build/requests.o


OBJSWORKER = 	Master/build/worker.o \
				Master/build/workerFunctions.o \
				Master/build/workerNamedPipesFifo.o \
				Master/build/list.o \
				Master/build/date.o \
				Master/build/hash.o \


OBJS2 = WhoServer/build/server.o \
		WhoServer/build/myVector.o \
		WhoServer/build/serverFunctions.o \
		WhoServer/build/requestsFromServer.o \
		Master/build/list.o \


OBJS3 = WhoClient/build/client.o \
		WhoServer/build/myVector.o	\
		Master/build/list.o \
		Master/build/date.o \
# OBJS3 =

CC = gcc -pthread
FLAGS = -Wextra -Wall -g -c

TARGET = master
TARGETWORKER = worker
TARGET2 = whoServer
TARGET3 = whoClient


all:  $(TARGET) $(TARGETWORKER) $(TARGET2) $(TARGET3)

clean:
	$(RM) -r  $(TARGET) $(TARGETWORKER) $(TARGET2) $(TARGET3) Master/build/* WhoClient/build/* WhoServer/build/*



# MASTER
Master/build/diseaseAggregator.o: Master/src/diseaseAggregator.c
	$(CC) $(FLAGS) $< -o $@

Master/build/parentNamedPipesFifo.o: Master/src/parentNamedPipesFifo.c
	$(CC) $(FLAGS) $< -o $@

Master/build/list.o: Master/src/list.c
	$(CC) $(FLAGS) $< -o $@

Master/build/parentFunctions.o: Master/src/parentFunctions.c
	$(CC) $(FLAGS) $< -o $@

Master/build/requests.o: Master/src/requests.c
	$(CC) $(FLAGS) $< -o $@



Master/build/worker.o: Master/src/worker.c
	$(CC) $(FLAGS) $< -o $@

Master/build/workerFunctions.o: Master/src/workerFunctions.c
	$(CC) $(FLAGS) $< -o $@

Master/build/workerNamedPipesFifo.o: Master/src/workerNamedPipesFifo.c
	$(CC) $(FLAGS) $< -o $@


Master/build/date.o: Master/src/date.c
	$(CC) $(FLAGS) $< -o $@

Master/build/hash.o: Master/src/hash.c
	$(CC) $(FLAGS) $< -o $@




# SERVER
WhoServer/build/server.o: WhoServer/src/server.c
	$(CC) $(FLAGS) $< -o $@

WhoServer/build/myVector.o: WhoServer/src/myVector.c
	$(CC) $(FLAGS) $< -o $@

WhoServer/build/serverFunctions.o: WhoServer/src/serverFunctions.c
	$(CC) $(FLAGS) $< -o $@

WhoServer/build/requestsFromServer.o: WhoServer/src/requestsFromServer.c
	$(CC) $(FLAGS) $< -o $@


# CLIENT
WhoClient/build/client.o: WhoClient/src/client.c
	$(CC) $(FLAGS) $< -o $@

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(TARGETWORKER) : $(OBJSWORKER)
	$(CC) $(CFLAGS) $^ -o $@ -lm


$(TARGET2) : $(OBJS2)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(TARGET3) : $(OBJS3)
	$(CC) $(CFLAGS) $^ -o $@ -lm



rebuild: clean all
