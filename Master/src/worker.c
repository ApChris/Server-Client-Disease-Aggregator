#include <stdio.h>
#include "../include/workerFunctions.h"

long fileDescriptorR;
long fileDescriptorW;
long processID;
PathNode * filesPathList = NULL;
Hash * diseaseHash;
Hash * patientHash;

long errorRecords = 0;
long successRecords = 0;
PathNode * countries = NULL;
PathNode * workerQueries = NULL;
char workerCountry[MAXIMUMBUFFER] = "";

SumStatistics * generalStatistics;
long buffersize;

char * servIP;
long servPort;

// sockets
long sock;
long newSock;
long optval = 1;
long workerSock;

void SigHandler()
{
    printf("MPHHHKA\n");
    char buffer[MAXIMUMBUFFER] = "";
    ReadFromNamedPipe(fileDescriptorR, buffer);
    // read(workerSock, buffer, MAXIMUMBUFFER);
    char * command = (char *)malloc(sizeof(char)*50);
    char * arguments;

    if( (sscanf(buffer, "%49s%m[^\n]", command, &arguments)) != EOF )
    {

        if(!strcmp(command, "/ReadingFiles"))
        {
            // Preprocess arguments that are going to be send
            char * path;
            // char * procID;
            char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
            char * tok = NULL;
            tok = strtok(arguments, delimiters);
            // procID = (char *)malloc(sizeof(char)* strlen(tok));
            // strcpy(procID,tok);
            tok = strtok(NULL, " \n");
            path = (char *)malloc(sizeof(char)* strlen(tok));
            strcpy(path,tok);

            ReadingFiles(path);


        }
        else if(!strcmp(command, "/listCountries"))
        {
            char * path;

            char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
            char * tok = NULL;
            tok = strtok(arguments, delimiters);

            tok = strtok(NULL, delimiters);
            tok = strtok(NULL, delimiters);
            tok = strtok(NULL, delimiters);
            path = (char *)malloc(sizeof(char)* strlen(tok));
            strcpy(path,tok);

            listCountries(path);

        }
        else if(!strcmp(command, "/diseaseFrequency"))
        {

            diseaseFrequency(arguments);

        }
        else if(!strcmp(command, "/topk-AgeRanges"))
        {

            topkAgeRanges(arguments);

        }
        else if(!strcmp(command, "/numPatientAdmissions"))
        {
            numPatientAdmissions(arguments);

        }
        else if(!strcmp(command, "/searchPatientRecord"))
        {
            char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
            char * tok = NULL;
            tok = strtok(arguments, delimiters);

            searchPatientRecord(tok);

        }
        else if(!strcmp(command, "/numPatientDischarges"))
        {
            numPatientDischarges(arguments);

        }
        else if(!strcmp(command, "/reCreateWorker"))
        {
            reCreateWorker();
        }
        else
        {
            printf("Wrong input\n");
        }

        free(command);
    }

}


void SigHandlerServer()
{
    printf("mphka\n");
    char buffer[MAXIMUMBUFFER] = "";
    // ReadFromNamedPipe(fileDescriptorR, buffer);
    read(workerSock, buffer, MAXIMUMBUFFER);
    char * command = (char *)malloc(sizeof(char)*50);
    char * arguments;

    if( (sscanf(buffer, "%49s%m[^\n]", command, &arguments)) != EOF )
    {

        if(!strcmp(command, "/ReadingFiles"))
        {
            // Preprocess arguments that are going to be send
            char * path;
            // char * procID;
            char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
            char * tok = NULL;
            tok = strtok(arguments, delimiters);
            // procID = (char *)malloc(sizeof(char)* strlen(tok));
            // strcpy(procID,tok);
            tok = strtok(NULL, " \n");
            path = (char *)malloc(sizeof(char)* strlen(tok));
            strcpy(path,tok);

            ReadingFiles(path);


        }
        else if(!strcmp(command, "/listCountries"))
        {
            char * path;

            char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
            char * tok = NULL;
            tok = strtok(arguments, delimiters);

            tok = strtok(NULL, delimiters);
            tok = strtok(NULL, delimiters);
            tok = strtok(NULL, delimiters);
            path = (char *)malloc(sizeof(char)* strlen(tok));
            strcpy(path,tok);

            listCountries(path);

        }
        else if(!strcmp(command, "/diseaseFrequency"))
        {

            diseaseFrequency(arguments);

        }
        else if(!strcmp(command, "/topk-AgeRanges"))
        {

            topkAgeRanges(arguments);

        }
        else if(!strcmp(command, "/numPatientAdmissions"))
        {
            numPatientAdmissions(arguments);

        }
        else if(!strcmp(command, "/searchPatientRecord"))
        {
            char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
            char * tok = NULL;
            tok = strtok(arguments, delimiters);

            searchPatientRecord(tok);

        }
        else if(!strcmp(command, "/numPatientDischarges"))
        {
            numPatientDischarges(arguments);

        }
        else if(!strcmp(command, "/reCreateWorker"))
        {
            reCreateWorker();
        }
        else
        {
            printf("Wrong input\n");
        }

        free(command);
    }

}

int main(int argc, const char *argv[])
{

    static struct sigaction terminatingAction;
    static struct sigaction answerAction;
    static struct sigaction answerActionServer;

    terminatingAction.sa_handler = Elimination;
    sigfillset(&(terminatingAction.sa_mask));
    sigaction(SIGTERM, &terminatingAction, NULL);
    sigaction(SIGQUIT, &terminatingAction, NULL);

    answerAction.sa_handler = SigHandler;
    sigfillset(&(answerAction.sa_mask));
    sigaction(SIGUSR1, &answerAction, NULL);

    // answerActionServer.sa_handler = SigHandlerServer;
    // sigfillset(&(answerActionServer.sa_mask));
    // sigaction(SIGINT, &answerActionServer, NULL);


    printf("WORKER has been created with pid: %ld \n",(long)getpid());

    if(argc > 0)
    {

        processID = atol(argv[0]);
        buffersize = atol(argv[2]);
        servPort = atol(argv[3]);
        if((servIP = (char *)malloc(sizeof(char) * strlen(argv[4]) + 1)) == NULL)
        {
            perror("WORKER, main , malloc has been failed");
            exit(EXIT_FAILURE);
        }
        strcpy(servIP,argv[4]);
        fileDescriptorR = OpenRead(processID);
        fileDescriptorW = OpenWrite(processID);
    }


    struct sockaddr_in server;
    uint64_t serverLength;
    struct sockaddr * serverPointer;

    // For gethostbyname
    struct hostent * gtHName;

    char message[MAXIMUMBUFFER] = "";

    // Create sockets
    if((workerSock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Client: Socket has been failed:");
        exit(EXIT_FAILURE);
    }

    // Helps in manipulating options, prevents error such as address already in use
    if(setsockopt(workerSock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(long)) < 0)
    {
        perror("Client: Setsockopt has been failed:");
        exit(EXIT_FAILURE);
    }

    if((gtHName = gethostbyname(servIP)) == NULL)
    {
        perror("Client: gethostbyname has been failed:");
        exit(EXIT_FAILURE);
    }

    // set server
    server.sin_family = AF_INET;
    bcopy((char *) gtHName -> h_addr, (char *) &server.sin_addr, gtHName -> h_length);

    // server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(servPort);
    serverPointer = (struct sockaddr *)&server;
    serverLength = sizeof(server);


    if(connect(workerSock, serverPointer, serverLength) == -1)
    {
        perror("Worker: connect has been failed:");
        exit(EXIT_FAILURE);
    }


    for(;;)
    {
        sleep(1);
    }



}
