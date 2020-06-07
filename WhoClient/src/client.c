#include <stdio.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include "../../WhoServer/include/myVector.h"
#include "../../Master/include/list.h"

#define MAXIMUMLINE 4096

#define MAXIMUMBUFFER 4096

// arguments
char * queryFile;
long numThreads;
long servPort;
char * servIP;

// threads
pthread_mutex_t mutex;
pthread_cond_t condinationVariable;

// MyVecors
myVector * threadsVector = NULL;
myVector * buffer = NULL;


PathNode * queries = NULL;

// sockets
long sock;
long newSock;
long optval = 1;
long bufferSize = 1;

long ReadFromSocket(long fileDescriptor, char * buffer);
// void WriteToSocket(long fileDescriptor, char * buffer);

// long ReadFromSocket(long fileDescriptor, char * message);
void WriteToSocket(long fileDescriptor, char * message);
void * mainThreadServer(void * argvp);
void ReadFile(const char * queryFile)
{
    char * line = NULL;
    size_t length = 0;
    long read;
    FILE * file;


    if((file = fopen(queryFile,"r")) == NULL)
    {
        perror("Client -> fopen has been failed:");
        exit(EXIT_FAILURE);
    }

    // Get line and store it in list
    while((read = getline(&line,&length, file)) != -1)
    {

        PushNode_Path(&queries,line);
        if(line)
        {
            free(line);
        }
        line = NULL;

    }

    Reverse_Path(&queries);
    free(line);
    fclose(file);
}

 // /whoClient -q WhoClient/etc/queryFile.txt -w 10 -sp 4000 -sip localhost

int main(int argc, char const *argv[])
{
    pthread_t * thread;
    char message[MAXBUFFER] = "";
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condinationVariable, NULL);


    if (argc != 9)                          // Check if we have !=9 arguments
    {
      printf("ERROR INPUT!!\nGive for example : ./whoClient -q WhoClient/etc/queryFile.txt -w 10 -sp 4000 -sip localhost\n");
      return -1;
    }

    for(long i = 1; i < argc; i++)      // Get arguments
    {
      if(!strcmp(argv[i],"-q"))         // Get file
      {
          queryFile = (char *)malloc(sizeof(char)*strlen(argv[i+1])+1);
          strcpy(queryFile,argv[i+1]);
      }
      else if(!strcmp(argv[i],"-w"))    // Get number of Threads
      {
         numThreads = atol(argv[i+1]);
         if(numThreads <= 0)
         {
             printf("Invalid threads number!\n");
             exit(EXIT_FAILURE);
         }
      }
      else if(!strcmp(argv[i],"-sp"))    // Get Port
      {
          servPort = atol(argv[i+1]);
          if(servPort <= 0)
          {
              printf("Invalid statistics port number!\n");
              exit(EXIT_FAILURE);
          }
      }
      else if(!strcmp(argv[i],"-sip"))    // Get Ip
      {
         servIP = (char *)malloc(sizeof(char)*strlen(argv[i+1])+1);
         strcpy(servIP,argv[i+1]);
      }
    }

    printf("%s %ld %ld %s\n",queryFile,numThreads,servPort,servIP);


    // Read query file
    ReadFile(queryFile);


    // Vector that holds threads pointers
    threadsVector = Init_MyVector();


    // create main thread
    if(!(thread = (pthread_t *)malloc(sizeof(pthread_t))))
    {
        perror("Pthread malloc has been failed!:");
        exit(EXIT_FAILURE);
    }
    else
    {
        pthread_create(thread, NULL, mainThreadServer ,NULL);
        PushBack_MyVector(threadsVector,thread);
    }

    pthread_cond_signal(&condinationVariable);
    // 
    // for (long i = 0; i < 1; i++) {
    //     pthread_join(GetItem_MyVector(threadsVector,i), NULL);
    // }
    // TESTING PART

    // struct sockaddr_in server;
    // uint64_t serverLength;
    // struct sockaddr * serverPointer;
    //
    // struct sockaddr_in client;
    // struct sockaddr * clientPointer;
    // uint64_t clientLength;
    //
    // // For gethostbyname
    // struct hostent * gtHName;
    //
    // // Create sockets
    // if((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    // {
    //     perror("Client: Socket has been failed:");
    //     exit(EXIT_FAILURE);
    // }
    //
    // // Helps in manipulating options, prevents error such as address already in use
    // if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(long)) < 0)
    // {
    //     perror("Client: Setsockopt has been failed:");
    //     exit(EXIT_FAILURE);
    // }
    //
    // if((gtHName = gethostbyname(servIP)) == NULL)
    // {
    //     perror("Client: gethostbyname has been failed:");
    //     exit(EXIT_FAILURE);
    // }
    //
    // // set server
    // server.sin_family = AF_INET;
    // bcopy((char *) gtHName -> h_addr, (char *) &server.sin_addr, gtHName -> h_length);
    //
    // // server.sin_addr.s_addr = htonl(INADDR_ANY);
    // server.sin_port = htons(servPort);
    // serverPointer = (struct sockaddr *)&server;
    // serverLength = sizeof(server);
    //
    //
    // if(connect(sock, serverPointer, serverLength) == -1)
    // {
    //     perror("Client: connect has been failed:");
    //     exit(EXIT_FAILURE);
    // }
    // printf("edw\n");
    // WriteToSocket(sock, "Hello fRom Client");
    // // printf("Client with pid: %ld -> %s\n",(long)getpid(),message);
    //
    // WriteToSocket(sock, "Bye Bye");
    // // printf("Client with pid: %ld -> %s\n",(long)getpid(),message);
    //
    // printf("Client with pid = %ld is listening on port:%ld\n",(long)getpid(), servPort);
    //
    //
    //


    // END OF TESTING PART


    PrintList_Path(&queries);
    DeleteList_Path(&queries);
    free(queryFile);
    free(servIP);

    return 0;
}




//
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
        perror("Client:WriteToSocket has been failed:");
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




//
void * mainThreadServer(void * argp)
{
    struct sockaddr_in server;
    uint64_t serverLength;
    struct sockaddr * serverPointer;

    struct sockaddr_in client;
    struct sockaddr * clientPointer;
    uint64_t clientLength;

    // For gethostbyname
    struct hostent * gtHName;

    // Create sockets
    if((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Client: Socket has been failed:");
        exit(EXIT_FAILURE);
    }

    // Helps in manipulating options, prevents error such as address already in use
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(long)) < 0)
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


    if(connect(sock, serverPointer, serverLength) == -1)
    {
        perror("Client: connect has been failed:");
        exit(EXIT_FAILURE);
    }
    printf("edw\n");
    WriteToSocket(sock, "Hello fRom Client");
    // printf("Client with pid: %ld -> %s\n",(long)getpid(),message);

    WriteToSocket(sock, "Bye Bye");
    // printf("Client with pid: %ld -> %s\n",(long)getpid(),message);

    printf("Client with pid = %ld is listening on port:%ld\n",(long)getpid(), servPort);




}
