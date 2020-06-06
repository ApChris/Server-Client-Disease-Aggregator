#ifndef __LIST_H__
#define __LIST_H__

#define FLAG 1400322
#define MAXBUFFER 4096

#include "date.h"

typedef struct PathNode
{
    char * path;
    struct PathNode * next;
}PathNode;


typedef struct Node
{
    long item;
    struct Node * next;
}Node;


typedef struct SumStatistics
{
    char * diseaseID;
    long cases_0_20;
    long cases_21_40;
    long cases_41_60;
    long cases_over_60;
    long errorRecords;
    long successRecords;
    struct SumStatistics * next;
}SumStatistics;



void PushNode(Node ** head, long item);
bool SearchInList(Node ** head, long item);
long LenOfList(void * head);
void DeleteList(Node ** head);
void DeleteNode(Node ** head, long item);
void PrintList(Node ** head);
void Reverse(Node ** head);
long GetValue(Node ** head, long i);
void UpdateNode(Node ** head, long i, long item);


void PushNode_Path(PathNode ** head, char * path);
bool SearchInList_Path(PathNode ** head, char * path);
void DeleteList_Path(PathNode ** head);
void DeleteNode_Path(PathNode ** head, char * path);
void PrintList_Path(PathNode ** head);
char * GetValue_Path(PathNode ** head, long i);
void Reverse_Path(PathNode ** head);


SumStatistics * SumStatistics_Init();
void PushNode_Statistics(SumStatistics ** head, char * path, long flag);
bool SearchInList_Statistics(SumStatistics ** head, char * path, long flag);
void DeleteList_Statistics(SumStatistics ** head);
void DeleteNode_Statistics(SumStatistics ** head, char * path);
char * PrintList_Statistics(SumStatistics ** head);
char * GetValue_Statistics(SumStatistics ** head, long i);
void Reverse_Statistics(SumStatistics ** head);
bool UpdateErrors_Statistics(SumStatistics ** head, char * diseaseID);
#endif
