#include "../include/clientFunctions.h"

extern long indexNodeCounter;
// arguments
extern char * queryFile;
extern long numThreads;
extern long servPort;
extern char * servIP;

// threads
extern pthread_mutex_t mutex;
extern pthread_cond_t condinationVariable;


// MyVecors
extern myVector * threadsVector;
extern myVector * buffer;


extern PathNode * queries;

// sockets
extern long sock;
extern long newSock;
extern long optval;
extern long bufferSize;

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

void * SendQueryToServer(void * argp)
{

    long clientSock;
    long *id = argp;

    struct sockaddr_in server;
    uint64_t serverLength;
    struct sockaddr * serverPointer;

    // For gethostbyname
    struct hostent * gtHName;

    char message[MAXIMUMBUFFER] = "";

    // Create sockets
    if((clientSock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Client: Socket has been failed:");
        exit(EXIT_FAILURE);
    }

    // Helps in manipulating options, prevents error such as address already in use
    if(setsockopt(clientSock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(long)) < 0)
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


    if(connect(clientSock, serverPointer, serverLength) == -1)
    {
        perror("Client: connect has been failed:");
        exit(EXIT_FAILURE);
    }

    // lock mutex
    pthread_mutex_lock(&mutex);

    sprintf(message, "Client Thread = %ld ----->%s",(long)*id, GetValue_Path(&queries,indexNodeCounter));
    printf("%s\n",message);
    // WriteToSocket(clientSock, message);
    write(clientSock, message, MAXIMUMBUFFER);


    // increase index from query list
    indexNodeCounter++;

    // unlock it
    pthread_mutex_unlock(&mutex);
    strcpy(message,"\0");
    // ReadFromSocket(clientSock,message);
    long bytes = read(clientSock, message, MAXIMUMBUFFER);

    message[bytes] = '\0';
    // printf("%s\n",message);
    if(!strcmp(message,"Completed"))
    {
        printf("Client Thread %ld has been successfully completed \n", *id);

    }


    // Unblock threads blocked on a cond var
    pthread_cond_signal(&condinationVariable);
    close(clientSock);
    pthread_exit(NULL);
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
