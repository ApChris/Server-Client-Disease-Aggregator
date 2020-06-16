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
    //
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

    if(pthread_join(GetItem_MyVector(threadsVector,0), NULL))
    {
        perror("Client: perror has been failed:");
        exit(EXIT_FAILURE);
    }
    printf("Rest threads are going to start!\n");
    for (long i = 1; i <= totalWorkers; i++)
    {
        // printf("totalWorkers %ld --> totalClients %ld %ld\n",totalWorkers,totalClients,i);
        // if(currentWorkers < totalWorkers)
        // {
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
        if(pthread_join(GetItem_MyVector(threadsVector,i), NULL))
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
            if(pthread_join(GetItem_MyVector(threadsVector,i), NULL))
            {
                perror("Client: perror has been failed:");
                exit(EXIT_FAILURE);
            }
        }

        // long quotient = length/bufferSize;
        // if(quotient == 0)
        // {
        //     write(fileDescriptor,buffer, length);
        // }
        // // else split message in chunks
        // else
        // {
        //     for (long i = 0; i < quotient; i++)
        //     {
        //         write(fileDescriptor,buffer + bufferSize*i, bufferSize);
        //         if(i + 1 == quotient)
        //         {
        //             long remainder = length - (quotient*bufferSize);
        // for (long i = totalWorkers + 1; i <= totalWorkers + totalClients; i++)
        // {
        //     write(GetItem_MyVector(bufferWorker,i), "Completed");
        //
        // }

        // for (long i = 0; i < count; i++)
        // {
        //
        // }
        // }
        // else
        // {



        // }

    // }
    // pthread_cond_signal(&condinationVariable);
    // for (long i = 1; i < numThreads; i++)
    // {
    //     if(pthread_join(GetItem_MyVector(threadsVector,i), NULL))
    //     {
    //         perror("Client: perror has been failed:");
    //         exit(EXIT_FAILURE);
    //     }
    // }
    printf("END\n");
    long quotient = totalClients/totalWorkers;
    long queriesPerWorker[totalWorkers];
    long index = 0;
    if(quotient == 0)
    {
        // if we have more workers than clients
    }
    else
    {
        for (long i = 0; i < totalWorkers; i++)
        {
            queriesPerWorker[i] = quotient;
            if(i + 1 == totalWorkers)
            {
                long remainder = totalClients - (quotient*totalWorkers);
                queriesPerWorker[i] = quotient + remainder;
            }
        }
    }
    PrintList_Path(&queries);
    for (long i = 0; i < totalWorkers; i++)
    {


        char finalMessage[MAXIMUMBUFFER] = "";

        for (long j = 0; j < queriesPerWorker[i]; j++)
        {

            char message[MAXIMUMBUFFER] = "";
            sprintf(message, "%s", GetValue_Path(&queries,index));
            char * tok = strtok(message, ">");
            tok = strtok(NULL,"\n");
            if(j == 0)
            {
                sprintf(finalMessage,"%s\n",tok);
            }
            else
            {
                strcat(finalMessage,tok);
                strcat(finalMessage,"\n");

            }
            index++;
        }
        printf("%s\n",finalMessage);
        // sprintf(message, "%s", GetValue_Path(&queries,i));
        // char * tok = strtok(message, ">");
        // tok = strtok(NULL,"\n");
        // strcpy(finalMessage,tok);
        // printf("%s\n", tok);
        // sprintf(message,"Heelo from %ld \n", i);
        write(GetItem_MyVector(bufferWorker,i), finalMessage ,strlen(finalMessage));
    }

    // write(GetItem_MyVector(bufferWorker,randomPick), message,strlen(message));
    // printf("%s\n",message);
    while(1){}

    return 0;
}
