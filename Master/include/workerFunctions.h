#ifndef __WORKERFUNCTIONS_H__
#define __WORKERFUNCTIONS_H__

#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include "workerNamedPipesFifo.h"
#include "list.h"
#include "hash.h"



SumStatistics * FillStructures(const char * patientRecordsFile, Hash * diseaseHash, Hash * patientHash, Date * date, char * country);
void reCreateWorker();
void Print_Input(char * patientRecordsFile, long diseaseHashtableNumOfEntries, long countryHashtableNumOfEntries, long bucketSize);
void listCountries();
void diseaseFrequency(char * arguments);
void topkAgeRanges(char * arguments);
void searchPatientRecord(char * recordID);
void numPatientAdmissions(char * arguments);
void numPatientDischarges(char * arguments);
void ReadingFiles(char * path);

void ReadRequests();
void SendRequests();
void Elimination();

#endif
