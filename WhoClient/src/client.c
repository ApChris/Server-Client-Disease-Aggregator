#include "../include/clientFunctions.h"

long indexNodeCounter = 0;
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


 // /whoClient -q WhoClient/etc/queryFile.txt -w 10 -sp 4000 -sip localhost

int main(int argc, char const *argv[])
{
    // arguments
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

    printf("Arguments : %s %ld %ld %s\n",queryFile,numThreads,servPort,servIP);


    // Read query file
    ReadFile(queryFile);

    long threadsIDS[LenOfList(queries)];
    for (long i = 0; i < LenOfList(queries); i++)
    {
        threadsIDS[i] = i;
    }
    // Vector that holds threads pointers
    threadsVector = Init_MyVector();


    // create
    for (long i = 0; i < LenOfList(queries); i++)
    {
        pthread_t thread;
        pthread_create(&thread, NULL, SendQueryToServer ,(void *)&threadsIDS[i]);
        PushBack_MyVector(threadsVector,(long *)thread);

    }

    sleep(5);
    pthread_cond_signal(&condinationVariable);
    for (long i = 0; i < LenOfList(queries); i++)
    {
        if(pthread_join((long)GetItem_MyVector(threadsVector,i), NULL))
        {
            perror("Client: perror has been failed:");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condinationVariable);
    pthread_exit(NULL);
    free(threadsVector);
    free(buffer);
    DeleteList_Path(&queries);
    free(queryFile);
    free(servIP);

    return 0;
}
