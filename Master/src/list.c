#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/list.h"

void PushNode(Node ** head, long item)
{
    Node * node = (Node *)malloc(sizeof(Node));

    node -> item = item;

    // The next pointer of new node
    node -> next = (* head);

    // Set head to point to the new node
    (*head) = node;

}

bool SearchInList(Node ** head, long item)
{

    Node * tmp = *head;
    while(tmp != NULL)
    {
        if(tmp -> item == item)
        {
            return true;
        }

        tmp = tmp -> next;
    }
    return false;
}


long LenOfList(void * head)
{
    long counter = 0;
    // Set current pointer to points at head
    Node * current = head;

    // for every list node
    while(current != NULL)
    {
        counter++;
        current = current -> next;
    }
    return counter;
}

void DeleteList(Node ** head)
{
    Node * current = * head;
    Node * next;

    while(current != NULL)
    {
        // change head
        next = current -> next;
        free(current);
        current = next;
    }


    *head = NULL;
}

void DeleteNode(Node ** head, long item)
{
    Node * tmp = *head;
    Node * prev;


    if(tmp != NULL && tmp -> item == item)
    {
        // change head
        *head = tmp -> next;
        free(tmp);
        return;
    }

    // Search for the item to be deleted
    while(tmp != NULL && tmp -> item != item)
    {
        prev = tmp;
        tmp = tmp -> next;
    }


    // if data doesn't exist in ll
    if(tmp == NULL)
    {
        return;
    }
    // unlink the node from ll
    prev -> next = tmp -> next;

    free(tmp);
}




void PrintList(Node ** head)
{
    Node * tmp = *head;
    while(tmp != NULL)
    {

        printf("%ld ", tmp -> item);

        tmp = tmp -> next;
    }
    printf("\n");
}

void Reverse(Node ** head)
{
    Node * prev = NULL;
    Node * current = *head;
    Node * next = NULL;

    while(current != NULL)
    {
        next = current -> next;
        current -> next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}


long GetValue(Node ** head, long i)
{

    Node * tmp = *head;
    long j = 0;
    while(tmp != NULL)
    {
        if(j == i)
        {
            return tmp -> item;
        }
        tmp = tmp -> next;
        j++;
    }
    return FLAG;
}

void UpdateNode(Node ** head, long i, long item)
{

    Node * tmp = *head;
    long j = 0;
    while(tmp != NULL)
    {
        if(j == i)
        {
            tmp -> item = item;
        }
        tmp = tmp -> next;
        j++;
    }
}


/////////////////////////////// Path ///////////////////////////////


void PushNode_Path(PathNode ** head, char * path)
{
    PathNode * node = (PathNode *)malloc(sizeof(PathNode));


    node -> path = ( char *)malloc(1 + sizeof(char) * strlen(path));
    strcpy(node -> path,(const  char *)path);

    // The next pointer of new node
    node -> next = (* head);

    // Set head to point to the new node
    (*head) = node;

}

bool SearchInList_Path(PathNode ** head, char * path)
{

    PathNode * tmp = *head;
    while(tmp != NULL)
    {
        if(!strcmp(tmp -> path, path))
        {
            return true;
        }
        tmp = tmp -> next;
    }
    return false;
}

char * GetValue_Path(PathNode ** head, long i)
{

    PathNode * tmp = *head;
    long j = 0;
    while(tmp != NULL)
    {
        if(j == i)
        {
            return tmp -> path;
        }
        tmp = tmp -> next;
        j++;
    }
    return NULL;
}


void DeleteList_Path(PathNode ** head)
{
    PathNode * current = * head;
    PathNode * next;

    while(current != NULL)
    {
        // change head
        next = current -> next;
        free(current -> path);
        free(current);
        current = next;
    }


    *head = NULL;
}

void DeleteNode_Path(PathNode ** head, char * path)
{
    PathNode * tmp = *head;
    PathNode * prev;


    if(tmp != NULL && !strcmp(tmp -> path, path))
    {
        // change head
        *head = tmp -> next;
        free(tmp -> path);
        free(tmp);
        return;
    }

    // Search for the item to be deleted
    while(tmp != NULL && strcmp(tmp -> path, path))
    {
        prev = tmp;
        tmp = tmp -> next;
    }




    // if data doesn't exist in ll
    if(tmp == NULL)
    {
        return;
    }
    // unlink the node from ll
    prev -> next = tmp -> next;
    free(tmp -> path);
    free(tmp);
}



void PrintList_Path(PathNode ** head)
{
    PathNode * tmp = *head;
    while(tmp != NULL)
    {
        printf("%s\n",tmp -> path);
        tmp = tmp -> next;
    }
    printf("\n");
}

void Reverse_Path(PathNode ** head)
{
    PathNode * prev = NULL;
    PathNode * current = *head;
    PathNode * next = NULL;

    while(current != NULL)
    {
        next = current -> next;
        current -> next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}




/////////////////////////////// Statistics ///////////////////////////////
SumStatistics * SumStatistics_Init()
{
    SumStatistics * statistics = (SumStatistics *)malloc(sizeof(statistics));
    statistics -> diseaseID = NULL;
    statistics -> cases_0_20 = 0;
    statistics -> cases_21_40 = 0;
    statistics -> cases_41_60 = 0;
    statistics -> cases_over_60 = 0;
    return statistics;
}


void PushNode_Statistics(SumStatistics ** head, char * diseaseID, long flag)
{
    SumStatistics * statistics = (SumStatistics *)malloc(sizeof(SumStatistics));


    statistics -> diseaseID = ( char *)malloc(1 + sizeof(char) * strlen(diseaseID));
    strcpy(statistics -> diseaseID,(const  char *)diseaseID);

    // The next pointer of new statistics
    statistics -> next = (* head);

    if(flag == 0)
    {
        statistics -> cases_0_20 = 1;
        statistics -> cases_21_40 = 0;
        statistics -> cases_41_60 = 0;
        statistics -> cases_over_60 = 0;
    }
    else if(flag == 1)
    {
        statistics -> cases_0_20 = 0;
        statistics -> cases_21_40 = 1;
        statistics -> cases_41_60 = 0;
        statistics -> cases_over_60 = 0;
    }
    else if(flag == 2)
    {
        statistics -> cases_0_20 = 0;
        statistics -> cases_21_40 = 0;
        statistics -> cases_41_60 = 1;
        statistics -> cases_over_60 = 0;
    }
    else
    {
        statistics -> cases_0_20 = 0;
        statistics -> cases_21_40 = 0;
        statistics -> cases_41_60 = 0;
        statistics -> cases_over_60 = 1;
    }

    // Set head to point to the new node
    (*head) = statistics;

}

bool SearchInList_Statistics(SumStatistics ** head, char * diseaseID, long flag)
{

    SumStatistics * tmp = *head;
    while(tmp != NULL)
    {
        if(!strcmp(tmp -> diseaseID, diseaseID))
        {
            if(flag == 0)
            {
                tmp -> cases_0_20++;
            }
            else if(flag == 1)
            {
                tmp -> cases_21_40++;
            }
            else if(flag == 2)
            {
                tmp -> cases_41_60++;
            }
            else
            {
                tmp -> cases_over_60++;
            }
            tmp -> successRecords++;
            return true;
        }
        tmp = tmp -> next;
    }
    return false;
}


bool UpdateErrors_Statistics(SumStatistics ** head, char * diseaseID)
{

    SumStatistics * tmp = *head;
    while(tmp != NULL)
    {
        if(!strcmp(tmp -> diseaseID, diseaseID))
        {

            tmp -> errorRecords++;
            return true;
        }
        tmp = tmp -> next;
    }
    return false;
}

char * GetValue_Statistics(SumStatistics ** head, long i)
{

    SumStatistics * tmp = *head;
    long j = 0;
    while(tmp != NULL)
    {
        if(j == i)
        {
            return tmp -> diseaseID;
        }
        tmp = tmp -> next;
        j++;
    }
    return NULL;
}


void DeleteList_Statistics(SumStatistics ** head)
{
    SumStatistics * current = * head;
    SumStatistics * next;

    while(current != NULL)
    {
        // change head
        next = current -> next;
        free(current -> diseaseID);
        free(current);
        current = next;
    }


    *head = NULL;
}

void DeleteNode_Statistics(SumStatistics ** head, char * diseaseID)
{
    SumStatistics * tmp = *head;
    SumStatistics * prev;


    if(tmp != NULL && !strcmp(tmp -> diseaseID, diseaseID))
    {
        // change head
        *head = tmp -> next;
        free(tmp -> diseaseID);
        free(tmp);
        return;
    }

    // Search for the item to be deleted
    while(tmp != NULL && strcmp(tmp -> diseaseID, diseaseID))
    {
        prev = tmp;
        tmp = tmp -> next;
    }




    // if data doesn't exist in ll
    if(tmp == NULL)
    {
        return;
    }
    // unlink the node from ll
    prev -> next = tmp -> next;
    free(tmp -> diseaseID);
    free(tmp);
}



char * PrintList_Statistics(SumStatistics ** head)
{
    SumStatistics * tmp = *head;
    char message[MAXBUFFER];
    char * messagePtr = message;
    while(tmp != NULL)
    {
        sprintf(message, "%s\nCases 0-20: %ld\nCases 21-40: %ld\nCases 41-60: %ld\nCases 65+: %ld\n",tmp -> diseaseID,tmp -> cases_0_20,tmp -> cases_21_40,tmp -> cases_41_60,tmp -> cases_over_60);
        tmp = tmp -> next;
    }
    // printf("\n");
    printf("%s\n",message);
    return messagePtr;
}

void Reverse_Statistics(SumStatistics ** head)
{
    SumStatistics * prev = NULL;
    SumStatistics * current = *head;
    SumStatistics * next = NULL;

    while(current != NULL)
    {
        next = current -> next;
        current -> next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}
