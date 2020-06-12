#ifndef __SERVERFUNCTIONS_H__
#define __SERVERFUNCTIONS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>

#include "../include/myVector.h"

#define MAXIMUMBUFFER 4096

long ReadFromSocket(long fileDescriptor, char * buffer);
void WriteToSocket(long fileDescriptor, char * buffer);
void * mainThreadJob(void * argp);
void * secondaryThreadJob(void * argp);

#endif
