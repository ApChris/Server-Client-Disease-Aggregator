#ifndef __PARENTFUNCTIONS_H_
#define __PARENTFUNCTIONS_H_

#include <stdio.h>
#include <dirent.h>
#include <sys/wait.h>
#include "../include/parentNamedPipesFifo.h"
#include "../include/list.h"
#define PROCESSIDSTRING 50




long CreateWorker(long processID);
void Elimination();
void ReCreateWorker();
void StartReadingFiles();
void SetWorkersForRequests();
#endif
