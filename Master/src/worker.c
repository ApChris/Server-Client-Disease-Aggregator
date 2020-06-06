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

SumStatistics * generalStatistics;
long buffersize;





void SigHandler()
{

    char buffer[MAXIMUMBUFFER] = "";
    ReadFromNamedPipe(fileDescriptorR, buffer);

    char * command = (char *)malloc(sizeof(char)*50);
    char * arguments;

    if( (sscanf(buffer, "%49s%m[^\n]", command, &arguments)) != EOF )
    {

        if(!strcmp(command, "/ReadingFiles"))
        {
            // Preprocess arguments that are going to be send
            char * path;
            char * procID;
            char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
            char * tok = NULL;
            tok = strtok(arguments, delimiters);
            procID = (char *)malloc(sizeof(char)* strlen(tok));
            strcpy(procID,tok);
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

    terminatingAction.sa_handler = Elimination;
    sigfillset(&(terminatingAction.sa_mask));
    sigaction(SIGINT, &terminatingAction, NULL);
    sigaction(SIGTERM, &terminatingAction, NULL);
    sigaction(SIGQUIT, &terminatingAction, NULL);

    answerAction.sa_handler = SigHandler;
    sigfillset(&(answerAction.sa_mask));
    sigaction(SIGUSR1, &answerAction, NULL);


    printf("WORKER has been created with pid: %ld \n",(long)getpid());

    if(argc > 0)
    {
        processID = atol(argv[0]);
        buffersize = atol(argv[2]);
        fileDescriptorR = OpenRead(processID);
        fileDescriptorW = OpenWrite(processID);
    }

    for(;;)
    {
        sleep(1);
    }



}
