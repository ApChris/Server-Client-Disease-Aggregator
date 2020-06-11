#include "../include/parentFunctions.h"


extern Node * writeNamedPipeList;
extern long totalWorkers;
extern long bufferSize;
extern char * path;
extern Node * workersPidList;
extern Node * readNamedPipeList;
extern PathNode * subDirectoriesPathList;
extern bool flagEliminate;


extern long servPort;
extern char * servIP;


void StartReadingFiles_Workers()
{
    printf("\n\n\n\n\n\n\n\nStart Reading\n");
    char message[MAXIMUMBUFFER];


    // for every worker
    for (long i = 0; i < totalWorkers; i++)
    {

        // send
        sprintf(message,"/ReadingFiles %ld %s %ld %s",i,GetValue_Path(&subDirectoriesPathList,i), servPort, servIP);

        WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);

        kill(GetValue(&workersPidList,i),SIGUSR1);


    }
    sleep(1);

    char result[MAXIMUMBUFFER] = "";
    for (long i = 0; i < totalWorkers; i++)
    {

        long bytes = 0;
        do
        {
            usleep(1);
        }while((bytes = ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);

        printf("%s\n",result);
        printf("--------------------------------------\n");
    }

}

long CreateWorker(long processID)
{
    // array of arguments
    char ** workerArguments;

    pid_t workerPid;

    if( (workerArguments = (char **)malloc(sizeof(char *)*6)) == NULL)
    {
        perror("Error: 1st malloc has been failed from CreateWorker function!");
        exit(EXIT_FAILURE);
    }

    if( (workerArguments[0] = (char *)malloc(sizeof(char)* PROCESSIDSTRING)) == NULL)
    {
        perror("Error: malloc has been failed from CreateWorker function!");
        exit(EXIT_FAILURE);
    }


    // convert processID to string
    sprintf(workerArguments[0], "%ld", processID);


    // Length of list , Number of subdirectories
    long totalSubDirectories = LenOfList(subDirectoriesPathList);

    long i = 0;
    while(i < totalSubDirectories)
    {
        if( (i%totalWorkers) == processID)
        {
            workerArguments[1] = (char *)malloc(1 + sizeof(char)* strlen(GetValue_Path(&subDirectoriesPathList,i) ));
            strcpy(workerArguments[1],GetValue_Path(&subDirectoriesPathList,i));
        }
        i++;
    }

    // Send bufferSize to workers
    if( (workerArguments[2] = (char *)malloc(sizeof(char)* PROCESSIDSTRING)) == NULL)
    {
        perror("Error: malloc has been failed from CreateWorker function!");
        exit(EXIT_FAILURE);
    }
    sprintf(workerArguments[2], "%ld", bufferSize);


    if( (workerArguments[3] = (char *)malloc(sizeof(char)* PROCESSIDSTRING)) == NULL)
    {
        perror("Error: malloc has been failed from CreateWorker function!");
        exit(EXIT_FAILURE);
    }

    sprintf(workerArguments[3], "%ld", servPort);

    if( (workerArguments[4] = (char *)malloc(sizeof(char)* strlen(servIP) + 1)) == NULL)
    {
        perror("Error: malloc has been failed from CreateWorker function!");
        exit(EXIT_FAILURE);
    }
    sprintf(workerArguments[4], "%s", servIP);

    // Put Null because argv from workers need to know where to stop
    workerArguments[5] = NULL;
    // printf("--> %s %s %s %s %s\n",workerArguments[0],workerArguments[1],workerArguments[2],workerArguments[3],workerArguments[4]);
    // if child
    if( (workerPid = fork()) == 0)
    {
        execvp("./worker", workerArguments);


    }
    else
    {
        usleep(1);
        for (long j = 0; j < 6; j++)
        {
            free(workerArguments[j]);
        }
        free(workerArguments);
        return workerPid;
    }
    return workerPid;
}



void Elimination()
{
    printf("Elimination has been activated!\n");
    long currentActiveWorkers;
    currentActiveWorkers = totalWorkers;
    int stat;
    long i = 0;
    flagEliminate = true;
    while(i < totalWorkers)
    {
        close(GetValue(&writeNamedPipeList,i));
        close(GetValue(&readNamedPipeList,i));
        UnlinkNamedPipe_FIFO(i,"main");
        UnlinkNamedPipe_FIFO(i,"secondary");
        kill(GetValue(&workersPidList,i), SIGTERM);
        wait(&stat);
        currentActiveWorkers--;
        printf("Active workers: %ld\n", currentActiveWorkers);
        i++;
    }
    free(path);
    DeleteList_Path(&subDirectoriesPathList);
    DeleteList(&workersPidList);
    DeleteList(&readNamedPipeList);
    DeleteList(&writeNamedPipeList);

}

//
void ReCreateWorker()
{
    long i = 0;
    if(flagEliminate == true)
    {
        return;
    }
    pid_t pid;
    pid = wait(NULL);
    printf("Worker has been killed : %ld\n", (long)pid);

    while (i < totalWorkers)
    {
        if(GetValue(&workersPidList,i) == pid)
        {

            UpdateNode(&workersPidList, i, CreateWorker(i));
            printf("New worker has been created using pid : %ld\n", GetValue(&workersPidList,i));
            break;
        }
        i++;
    }

}
