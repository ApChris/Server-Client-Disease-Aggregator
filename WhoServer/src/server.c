#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "../include/myVector.h"


#define MAXIMUMBUFFER 4096

// arguments
long queryPortNum;
long statisticsPortNum;
long numThreads;
long bufferSize;

// threads
pthread_mutex_t mutex;
pthread_cond_t condinationVariable;

// MyVecors
myVector * threads = NULL;
myVector * buffer = NULL;

// sockets
long sock;
long newSock;
long optval = 1;

long ReadFromSocket(long fileDescriptor, char * buffer);
void WriteToSocket(long fileDescriptor, char * buffer);

int main(int argc, char const *argv[])
{

    struct sockaddr_in server;
    uint64_t serverLength;
    struct sockaddr * serverPointer;

    struct sockaddr_in client;
    struct sockaddr * clientPointer;
    uint64_t clientLength;

    // For gethostbyname
    struct hostent * gtHName;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condinationVariable, NULL);

    if (argc != 9)                          // Check if we have !=9 arguments
    {
      printf("ERROR INPUT!!\nGive for example : ./whoServer -q 100 -s 200 -w 5 -b 100\n");
      return -1;
    }

    for(long i = 1; i < argc; i++)      // Get arguments
    {
      if(!strcmp(argv[i],"-q"))         // Get queryPortNum
      {
          queryPortNum = atol(argv[i+1]);
          if(queryPortNum <= 0)
          {
              printf("Invalid query port number!\n");
              exit(EXIT_FAILURE);
          }
      }
      else if(!strcmp(argv[i],"-s"))    // Get statisticsPortNum
      {
         statisticsPortNum = atol(argv[i+1]);
         if(statisticsPortNum <= 0)
         {
             printf("Invalid statistics port number!\n");
             exit(EXIT_FAILURE);
         }
      }
      else if(!strcmp(argv[i],"-w"))    // Get numThreads
      {
         numThreads = atol(argv[i+1]);
          if(numThreads <= 0)
         {
             printf("Invalid threads number!\n");
             exit(EXIT_FAILURE);
         }
      }
      else if(!strcmp(argv[i],"-b"))    // Get buffer Size
      {
         bufferSize = atol(argv[i+1]);
         if(bufferSize <= 0)
         {
             perror("Invalid buffer size!");
             exit(EXIT_FAILURE);
         }
      }

    }

    // Create sockets
    if((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket has been failed:");
        exit(EXIT_FAILURE);
    }

    // Helps in manipulating options, prevents error such as address already in use
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(long)) < 0)
    {
        perror("Setsockopt has been failed:");
        exit(EXIT_FAILURE);
    }


    // set server
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(queryPortNum);
    serverPointer = (struct sockaddr *)&server;
    serverLength = sizeof(server);

    if(bind(sock, serverPointer, serverLength) < 0)
    {
        perror("Bind has been failed:");
        exit(EXIT_FAILURE);
    }

    if(listen(sock,5) < 0)
    {
        perror("Listen has been failed:");
        exit(EXIT_FAILURE);
    }

    printf("Server with pid = %ld\n",(long)getpid());
    char message[MAXIMUMBUFFER] = "";

    clientPointer = (struct sockaddr *) &client;
    clientLength = sizeof client;

    if((newSock = accept(sock, clientPointer, &clientLength)) < 0)
    {
        perror("Server: accept has been failed:");
        exit(EXIT_FAILURE);
    }

    if((gtHName = gethostbyaddr((char *) &client.sin_addr.s_addr, sizeof client
        . sin_addr . s_addr, client . sin_family)) == NULL)
        {
            perror("Server: Gethostbyaddr has been failed:");
            exit(EXIT_FAILURE);
        }
        printf("EDWW\n");

        // strcpy(message,"Hello World");
    ReadFromSocket(newSock,message);
     printf("%s\n",message);
    printf("EDWW\n");
    ReadFromSocket(newSock,message);
     printf("%s\n",message);
    for (long i = 0; i < 3; i++)
    {


        // printf("%s\n",message);
    }
    printf("EDWW\n");
    return 0;
}


long ReadFromSocket(long fileDescriptor, char * buffer)
{
    long bytesNumber;
    long length = 0;
    long counter = 0;

    char finalBuffer[MAXIMUMBUFFER] = "";
    char tempBuffer[MAXIMUMBUFFER] = "";

    if(read(fileDescriptor, &length, sizeof(length)) > 0)
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

void WriteToSocket(long fileDescriptor, char * buffer)
{
    long length = strlen(buffer) + 1;

    if(write(fileDescriptor, &length, sizeof(length)) < 0)
    {
        perror("Server:WriteToSocket has been failed:");
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
