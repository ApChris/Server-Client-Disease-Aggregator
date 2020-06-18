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
    // long quotient = totalClients/totalWorkers;
    long queriesPerWorker[totalWorkers];
    for (size_t i = 0; i < totalWorkers; i++) {
        queriesPerWorker[i] = 0;
    }
    // long index = 0;
    // if(quotient == 0)
    // {
    //     // if we have more workers than clients
    // }
    // else
    // {
    //     for (long i = 0; i < totalWorkers; i++)
    //     {
    //         queriesPerWorker[i] = quotient;
    //         if(i + 1 == totalWorkers)
    //         {
    //             long remainder = totalClients - (quotient*totalWorkers);
    //             queriesPerWorker[i] = quotient + remainder;
    //         }
    //     }
    // }
    PrintList_Path(&queries);
    PrintList_Path(&countries);

    char finalMessage[MAXIMUMBUFFER] = "";
    for (size_t j = 0; j < LenOfList(queries); j++)
    {
        char message[MAXIMUMBUFFER] = "";
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
    printf("%s\n",finalMessage);
    // for (size_t i = 0; i < totalWorkers; i++) {
    //     write(GetItem_MyVector(bufferWorker,i), finalMessage ,strlen(finalMessage));
    // }
    // for (size_t i = 0; i < totalWorkers; i++)
    // {
    //     for (size_t j = 0; j < LenOfList(queries); j++)
    //     {
    //         char finalMessage[MAXIMUMBUFFER] = "";
    //         read(GetItem_MyVector(bufferWorker,i), finalMessage ,MAXIMUMBUFFER);
    //         printf("%s\n",finalMessage);
    //     }
    //
    // }
    // bool used[LenOfList(queries)];
    // for (size_t i = 0; i < LenOfList(queries); i++) {
    //     used[i] = false;
    // }
    // for (long i = 0; i < totalWorkers; i++)
    // {
    //
    //     char finalMessage[MAXIMUMBUFFER] = "";
    //     for (size_t j = 0; j < LenOfList(queries); j++)
    //     {
    //         if(used[j] == true)continue;
    //         char message[MAXIMUMBUFFER] = "";
    //         char messageTemp[MAXIMUMBUFFER] = "";
    //         // if(!strcmp(GetValue_Path(&queries,j)))
    //         sprintf(message, "%s", GetValue_Path(&queries,j));
    //         sprintf(messageTemp, "%s", GetValue_Path(&queries,j));
    //         char * tok = NULL;
    //         tok = strtok(message, ">");
    //         tok = strtok(NULL," ");
    //         char * arguments = strtok(NULL,"\n");
    //         char * command = NULL;
    //         command = strtok(messageTemp, ">");
    //         command = strtok(NULL,"\n");
    //
    //         // printf("command : %s\n", command);
    //
    //         // printf("arguments: %s\n",arguments);
    //         if(!strcmp(tok, "/listCountries\n"))
    //         {
    //
    //             Request1();
    //             used[j] = true;
    //             // DeleteNode_Path(&queries,GetValue_Path(&queries,j));
    //         }
    //         else if(!strcmp(tok, "/diseaseFrequency"))
    //         {
    //             char * tokTemp = strtok(arguments," ");
    //             tokTemp = strtok(NULL," ");
    //             tokTemp = strtok(NULL," ");
    //             tokTemp = strtok(NULL,"\n");
    //             // printf("diseaseFrequency ---------------> %s\n",tokTemp);
    //             if(tokTemp != NULL)
    //             {
    //                 if(!strcmp(tokTemp,GetValue_Path(&countries,i)))
    //                 {
    //                     if(j == 0)
    //                     {
    //                         sprintf(finalMessage,"%s\n",command);
    //                     }
    //                     else
    //                     {
    //                         strcat(finalMessage,command);
    //                         strcat(finalMessage,"\n");
    //                     }
    //                     used[j] = true;
    //                     queriesPerWorker[i]++;
    //                 }
    //             }
    //             else
    //             {
    //                 if(j == 0)
    //                 {
    //                     sprintf(finalMessage,"%s\n",command);
    //                 }
    //                 else
    //                 {
    //                     strcat(finalMessage,command);
    //                     strcat(finalMessage,"\n");
    //                 }
    //                 used[j] = true;
    //                 queriesPerWorker[i]++;
    //             }
    //
    //             // DeleteNode_Path(&queries,GetValue_Path(&queries,j));
    //         }
    //         else if(!strcmp(tok, "/topk-AgeRanges"))
    //         {
    //             char * tokTemp = strtok(arguments," ");
    //
    //             tokTemp = strtok(NULL," ");
    //             // printf("tok->%s\n",tokTemp);
    //             if(!strcmp(tokTemp,GetValue_Path(&countries,i)))
    //             {
    //                 if(j == 0)
    //                 {
    //                     sprintf(finalMessage,"%s\n",command);
    //                 }
    //                 else
    //                 {
    //                     strcat(finalMessage,command);
    //                     strcat(finalMessage,"\n");
    //
    //                 }
    //                 used[j] = true;
    //                 queriesPerWorker[i]++;
    //             }
    //
    //             // DeleteNode_Path(&queries,GetValue_Path(&queries,j));
    //         }
    //         else if(!strcmp(tok, "/numPatientAdmissions"))
    //         {
    //             char * tokTemp = strtok(arguments," ");
    //             tokTemp = strtok(NULL," ");
    //             tokTemp = strtok(NULL," ");
    //             tokTemp = strtok(NULL,"\n");
    //             if(tokTemp != NULL)
    //             {
    //                 if(!strcmp(tokTemp,GetValue_Path(&countries,i)))
    //                 {
    //                     if(j == 0)
    //                     {
    //                         sprintf(finalMessage,"%s\n",command);
    //                     }
    //                     else
    //                     {
    //                         strcat(finalMessage,command);
    //                         strcat(finalMessage,"\n");
    //
    //                     }
    //                     used[j] = true;
    //                     queriesPerWorker[i]++;
    //                 }
    //             }
    //             else
    //             {
    //                 if(j == 0)
    //                 {
    //                     sprintf(finalMessage,"%s\n",command);
    //                 }
    //                 else
    //                 {
    //                     strcat(finalMessage,command);
    //                     strcat(finalMessage,"\n");
    //                 }
    //                 used[j] = true;
    //                 queriesPerWorker[i]++;
    //             }
    //
    //         }
    //         else if(!strcmp(tok, "/searchPatientRecord"))
    //         {
    //             // char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
    //             // char * tok = NULL;
    //             // tok = strtok(arguments, delimiters);
    //
    //             // searchPatientRecord(tok);
    //             if(j == 0)
    //             {
    //                 sprintf(finalMessage,"%s\n",command);
    //             }
    //             else
    //             {
    //                 strcat(finalMessage,command);
    //                 strcat(finalMessage,"\n");
    //
    //             }
    //             used[j] = true;
    //             queriesPerWorker[i]++;
    //             // DeleteNode_Path(&queries,GetValue_Path(&queries,j));
    //         }
    //         else if(!strcmp(tok, "/numPatientDischarges"))
    //         {
    //             char * tokTemp = strtok(arguments," ");
    //             tokTemp = strtok(NULL," ");
    //             tokTemp = strtok(NULL," ");
    //             tokTemp = strtok(NULL,"\n");
    //             if(tokTemp != NULL)
    //             {
    //                 if(!strcmp(tokTemp,GetValue_Path(&countries,i)))
    //                 {
    //                     if(j == 0)
    //                     {
    //                         sprintf(finalMessage,"%s\n",command);
    //                     }
    //                     else
    //                     {
    //                         strcat(finalMessage,command);
    //                         strcat(finalMessage,"\n");
    //
    //                     }
    //                     used[j] = true;
    //                     queriesPerWorker[i]++;
    //                 }
    //             }
    //             else
    //             {
    //                 if(j == 0)
    //                 {
    //                     sprintf(finalMessage,"%s\n",command);
    //                 }
    //                 else
    //                 {
    //                     strcat(finalMessage,command);
    //                     strcat(finalMessage,"\n");
    //                 }
    //                 used[j] = true;
    //                 queriesPerWorker[i]++;
    //             }
    //             // DeleteNode_Path(&queries,GetValue_Path(&queries,j));
    //         }
    //
    //     }
    //     printf("WORKER:%ld ---> \n%s\n",i,finalMessage);
    //     if(strcmp(finalMessage, ""))
    //     {
    //         write(GetItem_MyVector(bufferWorker,i), finalMessage ,strlen(finalMessage));
    //
    //     }
    // }
    //
    //
    // for (long i = 0; i < totalWorkers; i++)
    // {
    //     // printf("Worker:%ld -> %ld queries\n",i,queriesPerWorker[i]++);
    //     //
    //     for (long j = 0; j < queriesPerWorker[i]; j++)
    //     {
    //         char temp[MAXIMUMBUFFER] = "";
    //         read(GetItem_MyVector(bufferWorker,i),  temp ,MAXIMUMBUFFER);
    //         printf("Worker:%ld\n\tQuery:%s\n",i,temp);
    //     }
    //
    // }
    //
    //
    //     char finalMessage[MAXIMUMBUFFER] = "";
    //
    //     for (long j = 0; j < queriesPerWorker[i]; j++)
    //     {
    //
    //         char message[MAXIMUMBUFFER] = "";
    //         sprintf(message, "%s", GetValue_Path(&queries,index));
    //         char * tok = strtok(message, ">");
    //         tok = strtok(NULL,"\n");
    //         if(j == 0)
    //         {
    //             sprintf(finalMessage,"%s\n",tok);
    //         }
    //         else
    //         {
    //             strcat(finalMessage,tok);
    //             strcat(finalMessage,"\n");
    //
    //         }
    //         index++;
    //     }
    //     printf("%s\n",finalMessage);
    //     // sprintf(message, "%s", GetValue_Path(&queries,i));
    //     // char * tok = strtok(message, ">");
    //     // tok = strtok(NULL,"\n");
    //     // strcpy(finalMessage,tok);
    //     // printf("%s\n", tok);
    //     // sprintf(message,"Heelo from %ld \n", i);
    //     write(GetItem_MyVector(bufferWorker,i), finalMessage ,strlen(finalMessage));
    //
    //     for (size_t j = 0; j < queriesPerWorker[i]; j++) {
    //         /* code */
    //         char temp[MAXIMUMBUFFER] = "";
    //         read(GetItem_MyVector(bufferWorker,i),  temp ,MAXIMUMBUFFER);
    //         printf("Worker:%ld -> query :%ld\n%s\n",i,j,temp);
    //     }
    //     // read(GetItem_MyVector(bufferWorker,i),  temp ,MAXIMUMBUFFER);
    //     // printf("%s\n",temp);
    //     // if(!strcmp(temp,"Accepted"))
    //     // {
    //     //     printf("Worker:%ld --> Transfer has been completed!\n",i);
    //     // }
    //
    //
    // }



    // write(GetItem_MyVector(bufferWorker,randomPick), message,strlen(message));
    // printf("%s\n",message);
    // Request1();
    // char * tok;
    // char delimiters[] = " \n\t\r\v\f\n:,.><[]{}|=+*@#$";
    // char * temp = (char *)malloc(sizeof(char) * strlen("/diseaseFrequency COVID-2019 10-10-2010 10-10-2020")+ 1);
    // strcpy(temp,"/diseaseFrequency COVID-2019 10-10-2010 10-10-2020");
    // tok = strtok(temp,delimiters);
    // printf("%s\n",tok);
    // Request2(tok);
    while(1){}

    return 0;
}
