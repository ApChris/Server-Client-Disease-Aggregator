#include "../include/parentNamedPipesFifo.h"


extern long bufferSize;

bool CreateNamedPipe_FIFO(long pid, char * flag)
{
    char name[80];
    if(!strcmp(flag,"main"))
    {
        sprintf(name, "%s_%ld_main", FIFOPATH, pid);
    }
    else if(!strcmp(flag,"secondary"))
    {
        sprintf(name, "%s_%ld_secondary", FIFOPATH, pid);
    }

    if(mkfifo(name, 0666) < 0)
    {
        if(errno != EEXIST)
        {
            perror("Error:Fifo has not been created!!! - PARENT");
            exit(EXIT_FAILURE);
        }

    }
    return true;
}

bool UnlinkNamedPipe_FIFO(long pid, char * flag)
{
    char name[80];
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
        perror("Error:Fifo has not been unlinked!!! - PARENT");
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


        long quotient = length/bufferSize;
        if(quotient == 0)
        {
            if( (bytesNumber = read(fileDescriptor,tempBuffer, length)) >= 0)
            {
                char chunkLength[length];
                strcpy(chunkLength,tempBuffer);
                strcat(finalBuffer,chunkLength);
            }

        }
        else
        {
            // for every chunk
            for (long i = 0; i < quotient; i++)
            {
                char chunk[bufferSize];

                // read bufferSize bytes and append them to finalBuffer
                if( (bytesNumber = read(fileDescriptor,tempBuffer, bufferSize)) >= 0)
                {
                    strcpy(chunk,tempBuffer);

                    strcat(finalBuffer,chunk);

                    // for last bytes
                    if(i + 1 == quotient)
                    {
                        char lastChunk[bufferSize];
                        long remainder = length - (quotient*bufferSize);
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
        perror("ERROR:WriteToNamedPipe has been failed - PARENT");
        exit(EXIT_FAILURE);
    }

    // if bufferSize > length . just read length bytes
    long quotient = length/bufferSize;
    if(quotient == 0)
    {
        write(fileDescriptor,buffer, length);
    }
    // else split message in chunks
    else
    {
        for (long i = 0; i < quotient; i++)
        {
            write(fileDescriptor,buffer + bufferSize*i, bufferSize);
            if(i + 1 == quotient)
            {
                long remainder = length - (quotient*bufferSize);
                write(fileDescriptor, buffer + bufferSize*(i+1), remainder);
            }
        }
    }
}

long OpenRead(long pid)
{
    long fileDescriptor;

    char name[50];

    sprintf(name, "%s_%ld_secondary", FIFOPATH, pid);

    if( (fileDescriptor = open(name, O_NONBLOCK | O_RDONLY)) < 0)
    {
        perror("Error:OpenRead from Fifo has been failed - PARENT");
        exit(EXIT_FAILURE);
    }

    return fileDescriptor;
}


long OpenWrite(long pid)
{
    long fileDescriptor;

    char name[50];

    sprintf(name, "%s_%ld_main", FIFOPATH, pid);

    if( (fileDescriptor = open(name, O_WRONLY)) < 0)
    {
        perror("Error:OpenRead from Fifo has been failed - PARENT");
        exit(EXIT_FAILURE);
    }

    return fileDescriptor;
}
