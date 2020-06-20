#include <stdio.h>


#include "../include/serverFunctions.h"





// arguments
long queryPortNum;
long statisticsPortNum;
long numThreads;
long bufferSize;

// mutex conditional variable
pthread_mutex_t mutex;
pthread_cond_t condinationVariable;

// MyVectors
myVector * threadsVector = NULL;
myVector * bufferClient = NULL;
myVector * bufferWorker = NULL;
long indexOfVector = 0;
long indexOfVectorC = 0;

// extra variables
long totalWorkers = 0;
long currentWorkers = 0;
long totalClients = 0;
long currentClients = 0;

PathNode * queries = NULL;
PathNode * countries = NULL;
Node * pidOfCountries = NULL;

int main(int argc, char const *argv[])
{

    pthread_t * thread;

    threadsVector = Init_MyVector();


    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condinationVariable, NULL);

    // arguments
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

    // set Threads IDS
    long threadsIDS[numThreads];
    for (long i = 0; i < numThreads; i++)
    {
        threadsIDS[i] = i;
    }

    // create main Thread
    if((thread = (pthread_t *)malloc(sizeof(pthread_t))) == NULL)
    {
        perror("Pthread malloc has been failed!:");
        exit(EXIT_FAILURE);
    }
    else
    {
        pthread_create(&thread, NULL, mainThreadJob ,(void *)&threadsIDS[0]);
        PushBack_MyVector(threadsVector, thread);

    }

    if(pthread_join((long)GetItem_MyVector(threadsVector,0), NULL))
    {
        perror("Client: perror has been failed:");
        exit(EXIT_FAILURE);
    }
    printf("-------Statistics-------\n\n");
    for (long i = 1; i <= totalWorkers; i++)
    {

        if((thread = (pthread_t *)malloc(sizeof(pthread_t))) == NULL)
        {
            perror("Pthread malloc has been failed!:");
            exit(EXIT_FAILURE);
        }
        else
        {
            pthread_create(&thread, NULL, WorkersThreadJob ,(void *)&threadsIDS[i]);

            PushBack_MyVector(threadsVector, thread);
            pthread_mutex_lock(&mutex);
            currentWorkers++;
            pthread_mutex_unlock(&mutex);
        }
    }
    for (long i = 1; i <= totalWorkers; i++)
    {
        if(pthread_join((long)GetItem_MyVector(threadsVector,i), NULL))
        {
            perror("Client: perror has been failed:");
            exit(EXIT_FAILURE);
        }
    }

    for (long i = totalWorkers + 1; i <= totalWorkers + totalClients; i++)
    {
        if((thread = (pthread_t *)malloc(sizeof(pthread_t))) == NULL)
        {
            perror("Pthread malloc has been failed!:");
            exit(EXIT_FAILURE);
        }
        else
        {
            pthread_create(&thread, NULL, ClientsThreadJob ,(void *)&threadsIDS[i]);

            PushBack_MyVector(threadsVector, thread);
            pthread_mutex_lock(&mutex);
            currentClients++;
            pthread_mutex_unlock(&mutex);
        }
    }

    for (long i = totalWorkers + 1; i <= totalWorkers + totalClients; i++)
    {
        if(pthread_join((long)GetItem_MyVector(threadsVector,i), NULL))
        {
            perror("Client: perror has been failed:");
            exit(EXIT_FAILURE);
        }
    }


    printf("-----Statistics printing has been terminated--------\n\n");

    printf("-----Requests--------\n\n");
    PrintList_Path(&queries);
    printf("-----Requests printing has been terminated--------\n\n");
    // PrintList_Path(&countries);

    char finalMessage[MAXIMUMBUFFER*4] = "";
    for (long j = 0; j < LenOfList(queries); j++)
    {
        char message[MAXIMUMBUFFER*4] = "";
        sprintf(message, "%s", GetValue_Path(&queries,j));
        char * command = NULL;
        command = strtok(message, ">");
        command = strtok(NULL,"\n");
        if(j == 0)
        {
            sprintf(finalMessage,"%s\n",command);
        }
        else
        {
            strcat(finalMessage,command);
            strcat(finalMessage,"\n");
        }

    }
    // printf("%s\n",finalMessage);
    printf("-----Results--------\n");
    for (long i = 0; i < totalWorkers; i++)
    {
        write((long)GetItem_MyVector(bufferWorker,i), finalMessage ,strlen(finalMessage));
    }
    long flag = 0;
    for (long i = 0; i < totalWorkers; i++)
    {
        char finalResult[MAXIMUMBUFFER*4] = "";
        long bytes = read((long)GetItem_MyVector(bufferWorker,i), finalResult, MAXIMUMBUFFER*4);
        if(bytes != 0 && flag == 0)
        {
            printf("%s\n",finalResult);

            flag = 1;
            break;
        }
    }
    printf("-----Results printing has been terminated--------\n\n");

    return 0;
}
