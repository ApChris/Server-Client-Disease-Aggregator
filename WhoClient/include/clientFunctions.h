#ifndef __CLIENTFUNCTIONS_H__
#define __CLIENTFUNCTIONS_H__

#include <stdio.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include "../../WhoServer/include/myVector.h"
#include "../../Master/include/list.h"

#define MAXIMUMLINE 4096

#define MAXIMUMBUFFER 4096

void * SendQueryToServer(void * argp);
long ReadFromSocket(long fileDescriptor, char * buffer);
void WriteToSocket(long fileDescriptor, char * buffer);
void ReadFile(const char * queryFile);
#endif
