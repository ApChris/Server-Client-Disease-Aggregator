#include "../include/serverFunctions.h"

// arguments
extern long queryPortNum;
extern long statisticsPortNum;

extern long bufferSize;

// mutex conditional variable
extern pthread_mutex_t mutex;
extern pthread_cond_t condinationVariable;

// MyVectors
extern myVector * bufferClient;
extern myVector * bufferWorker;
extern long indexOfVector;
extern long indexOfVectorC;

// extra variables
extern long totalWorkers;
extern long currentWorkers;
extern long totalClients;
extern long currentClients;

extern PathNode * queries;
extern PathNode * countries;
extern Node * pidOfCountries;

// sockets
long sock;
long newSock;
long sockStat;
long newSockStat;
long optval = 1;


void * mainThreadJob(void * argp)
{
    long *id = argp;
    // variables

    // Port
    struct sockaddr_in server;
    unsigned int serverLength;
    struct sockaddr * serverPointer;

    // Statistics
    struct sockaddr_in serverStat;
    unsigned int serverLengthStat;
    struct sockaddr * serverPointerStat;

    struct sockaddr_in client;
    struct sockaddr * clientPointer;
    unsigned int clientLength;

    // For gethostbyname
    struct hostent * gtHName;


    // select()
    fd_set set;
    struct timeval timeOut;
    long rv;


    bufferClient = Init_MyVector();
    bufferWorker = Init_MyVector();
    // Create sockets
    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
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





    // Create sockets
    if((sockStat = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket has been failed:");
        exit(EXIT_FAILURE);
    }

    // Helps in manipulating options, prevents error such as address already in use
    if(setsockopt(sockStat, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(long)) < 0)
    {
        perror("Setsockopt has been failed:");
        exit(EXIT_FAILURE);
    }


    // set server Statistics
    serverStat.sin_family = AF_INET;
    serverStat.sin_addr.s_addr = htonl(INADDR_ANY);
    serverStat.sin_port = htons(statisticsPortNum);
    serverPointerStat = (struct sockaddr *)&serverStat;
    serverLengthStat = sizeof(serverStat);

    if(bind(sockStat, serverPointerStat, serverLengthStat) < 0)
    {
        perror("Bind has been failed:");
        exit(EXIT_FAILURE);
    }

    if(listen(sockStat,5) < 0)
    {
        perror("Listen has been failed:");
        exit(EXIT_FAILURE);
    }


    printf("Server Thread = %ld is listening on ports: --> %ld , %ld <-- \n",*id, queryPortNum, statisticsPortNum);


    // Workers , you have to
    while(1)
    {
        FD_ZERO(&set);
        FD_SET(sockStat, &set);

        timeOut.tv_sec = 5;
        timeOut.tv_usec = 0;

        rv = select(sockStat + 1, &set, NULL, NULL, &timeOut);

        if(rv == -1)
        {
            perror("Error select");
            exit(EXIT_FAILURE);
        }
        else if(rv == 0)
        {
            if(totalWorkers == 0)
            {
                printf("--> 5 seconds have passed -> Still listening for Workers\n");
            }
            else
            {
                printf("--> 5 seconds have passed -> Thread is moving on Clients\n");
                break;

            }
        }
        else
        {
            clientPointer = (struct sockaddr *) &client;
            clientLength = sizeof client;

            if ((newSockStat = accept(sockStat, clientPointer, &clientLength)) < 0)
            {
                perror("Accept has been failed!:");
                exit(EXIT_FAILURE);
            }

            if((gtHName = gethostbyaddr((char *) &client.sin_addr.s_addr, sizeof client
                . sin_addr . s_addr, client . sin_family)) == NULL)
                {
                    perror("Server: Gethostbyaddr has been failed:");
                    exit(EXIT_FAILURE);
                }
                printf("Accepted Worker connection\n");
                PushBack_MyVector(bufferWorker,(long *)newSockStat);
                totalWorkers++;
        }

    }

    // Clients
    while(1)
    {
        FD_ZERO(&set);
        FD_SET(sock, &set);

        timeOut.tv_sec = 5;
        timeOut.tv_usec = 0;

        rv = select(sock + 1, &set, NULL, NULL, &timeOut);

        if(rv == -1)
        {
            perror("Error select");
            exit(EXIT_FAILURE);
        }
        else if(rv == 0)
        {
            if(totalClients == 0)
            {
                printf("--> 5 seconds have passed -> Still listening for Clients\n");
            }
            else
            {
                // pthread_cond_signal(&condinationVariable);
                printf("--> 5 seconds have passed -> Thread has finished with Clients\n");
                break;

            }
        }
        else
        {


            clientPointer = (struct sockaddr *) &client;
            clientLength = sizeof client;
           if ((newSock = accept(sock, clientPointer, &clientLength)) < 0)
           {
               perror("Accept has been failed!:");
               exit(EXIT_FAILURE);
           }

           if((gtHName = gethostbyaddr((char *) &client.sin_addr.s_addr, sizeof client
               . sin_addr . s_addr, client . sin_family)) == NULL)
               {
                   perror("Server: Gethostbyaddr has been failed:");
                   exit(EXIT_FAILURE);
               }

               PushBack_MyVector(bufferClient,(long *)newSock);

               printf("Accepted Client connection\n");

               totalClients++;
               // pthread_cond_signal(&condinationVariable);
        }
    }
    pthread_exit(NULL);
}



void * WorkersThreadJob()
{


    char * tok = NULL;
    pthread_mutex_lock(&mutex);

    char message[MAXIMUMBUFFER] = "";
    char tmpMessage[MAXIMUMBUFFER] = "";

    long bytes = read((long)GetItem_MyVector(bufferWorker,indexOfVector), message, MAXIMUMBUFFER);

    message[bytes] = '\0';
    printf("%s\n",message);
    write((long)GetItem_MyVector(bufferWorker,indexOfVector), "ReadStatistics", strlen("ReadStatics"));
    bytes = read((long)GetItem_MyVector(bufferWorker,indexOfVector), tmpMessage, MAXIMUMBUFFER);
    tmpMessage[bytes] = '\0';
    tok = strtok(tmpMessage, "-");
    PushNode(&pidOfCountries,atol(tok));
    tok = strtok(NULL, "\n");
    PushNode_Path(&countries,tok);

    indexOfVector++;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);

}

void * ClientsThreadJob()
{

    char message[MAXIMUMBUFFER] = "";
    pthread_mutex_lock(&mutex);
    // printf("-----------------------------------------------> edwww\n");
    long bytes = read((long)GetItem_MyVector(bufferClient,indexOfVectorC), message, MAXIMUMBUFFER);

    message[bytes] = '\0';

    // printf("Server Thread %ld received from --->%s\n", (long)GetItem_MyVector(bufferClient,indexOfVectorC), message);
    PushNode_Path(&queries,message);
    write((long)GetItem_MyVector(bufferClient,indexOfVectorC), "Completed", strlen("Completed\0"));

    indexOfVectorC++;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
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
