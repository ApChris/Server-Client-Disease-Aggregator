#include "../include/workerNamedPipesFifo.h"

extern long buffersize;

bool UnlinkNamedPipe_FIFO(long pid, char * flag)
{
    char name[50];
    if(!strcmp(flag,"main"))
    {
        sprintf(name, "%s_%ld_main", FIFOPATH, pid);
    }
    else if(!strcmp(flag,"secondary"))
    {
        sprintf(name, "%s_%ld_secondary", FIFOPATH, pid);
    }

    if(unlink(name) < 0)
    {
        perror("Error:Fifo has not been unlinked -> WORKER!!!");
        exit(EXIT_FAILURE);
    }

    return true;
}

long ReadFromNamedPipe(long fileDescriptor, char * buffer)
{
    long bytesNumber;
    long length = 0;
    long counter = 0;

    char finalBuffer[MAXIMUMBUFFER] = "";
    char tempBuffer[MAXIMUMBUFFER] = "";

    while(read(fileDescriptor, &length, sizeof(length)) > 0)
    {


        long quotient = length/buffersize;
        if(quotient == 0)
        {
            // bytesNumber = read(fileDescriptor,tempBuffer, length);
            if( (bytesNumber = read(fileDescriptor,tempBuffer, length)) >= 0)
            {
                char chunkLength[length];
                strcpy(chunkLength,tempBuffer);
                strcat(finalBuffer,chunkLength);
            }

        }
        else
        {

            for (long i = 0; i < quotient; i++)
            {
                char chunk[buffersize];

                if( (bytesNumber = read(fileDescriptor,tempBuffer, buffersize)) >= 0)
                {
                    strcpy(chunk,tempBuffer);

                    strcat(finalBuffer,chunk);

                    // for last bytes
                    if(i + 1 == quotient)
                    {
                        char lastChunk[buffersize];
                        long remainder = length - (quotient*buffersize);
                        bytesNumber = read(fileDescriptor, tempBuffer, remainder);

                        strcpy(lastChunk,tempBuffer);
                        strcat(finalBuffer,lastChunk);

                    }
                }

            }

        }
        counter += length;

    }
    strcpy(buffer, finalBuffer);

    return counter;

}

void WriteToNamedPipe(long fileDescriptor, char * buffer)
{


    long length = strlen(buffer) + 1;

    if(write(fileDescriptor, &length, sizeof(length)) < 0)
    {
        perror("ERROR:WriteToNamedPipe has been failed - WORKER");
        exit(EXIT_FAILURE);
    }
    long quotient = length/buffersize;
    if(quotient == 0)
    {
        write(fileDescriptor,buffer, length);
    }
    else
    {
        for (long i = 0; i < quotient; i++)
        {
            write(fileDescriptor,buffer + buffersize*i, buffersize);
            if(i + 1 == quotient)
            {
                long remainder = length - (quotient*buffersize);
                write(fileDescriptor, buffer + buffersize*(i+1), remainder);
            }
        }
    }

}

long OpenRead(long pid)
{
    char name[50];
    long fileDescriptor;
    sprintf(name, "%s_%ld_main", FIFOPATH, pid);
    if( (fileDescriptor = open(name, O_NONBLOCK | O_RDONLY)) < 0)
    {
        perror("Error:OpenRead from Fifo has been failed -> WORKER");
        exit(EXIT_FAILURE);
    }
    return fileDescriptor;
}

long OpenWrite(long pid)
{
    char name[50];
    long fileDescriptor;
    sprintf(name, "%s_%ld_secondary", FIFOPATH, pid);
    if( (fileDescriptor = open(name, O_WRONLY)) < 0)
    {
        perror("Error:OpenRead from Fifo has been failed -> WORKER");
        exit(EXIT_FAILURE);
    }
    return fileDescriptor;
}
