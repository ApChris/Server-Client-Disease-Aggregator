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

void Request1()
{

    char message[MAXIMUMBUFFER];
        // pthread_mutex_lock(&mutex);
    // send to every worker listCountriess
    // for (long i = 0; i < totalWorkers; i++)
    // {
    //     sprintf(message,"/listCountries %s",GetValue_Path(&countries,i));
    //
    //     if(write(GetItem_MyVector(bufferWorker,i), message, strlen(message)) < 0)
    //     {
    //         perror("Request1 write");
    //         exit(EXIT_FAILURE);
    //     }
    //     kill(GetItem_MyVector(bufferWorker,i),SIGUSR1);
    //
    // }
    printf("/listCountries\n");
    for (long i = 0; i < totalWorkers; i++)
    {
        printf("%s %ld\n",GetValue_Path(&countries,i),GetValue(&pidOfCountries,i));
    }
    // pthread_mutex_unlock(&mutex);
    // char result[MAXIMUMBUFFER];
    // int bytes;
    // for (long i = 0; i < totalWorkers; i++)
    // {
    //     do
    //     {
    //         usleep(100);
    //
    //     }while((bytes=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);
    //     printf("%s\n",result);
    // }


}

void Request2(char * tok)
{
    printf("-1\n");
    char message[MAXIMUMBUFFER];
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    char * diseaseID;

    tok = strtok(NULL," ");
    printf("-2\n");
    if(tok == NULL)
    {
        printf("error\n");
        return;
    }
    diseaseID = ( char *)malloc(1 + sizeof(char) * strlen(tok));
    strcpy(diseaseID,(const  char *)tok);


    Date * date1 = NULL;
    Date * date2 = NULL;
    date1 = malloc(sizeof(*date1));
    date2 = malloc(sizeof(*date2));
    printf("1\n");
    // // date1
    tok = strtok(NULL, delimiters);

    date1 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> year = (long)atoi(tok);

    // date2
    tok = strtok(NULL,delimiters);
    printf("2\n");
    if(tok == NULL)
    {
        free(date1);
        free(date2);
        printf("error\n");
        return;
    }
    // date2
    date2 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date2 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date2 -> year = (long)atoi(tok);
    // diseaseFrequency H1N1 10-10-2010 20-20-2020 Greece

    tok = strtok(NULL,delimiters);
    // without country
    printf("3\n");
    if(tok == NULL)
    {

        for (long i = 0; i < totalWorkers; i++)
        {

            sprintf(message,"/diseaseFrequency %s %ld-%ld-%ld %ld-%ld-%ld",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year);
            printf("%s\n",message);
            if(write(GetItem_MyVector(bufferWorker,i), message, strlen(message)) < 0)
            {
                perror("Request2 write");
                exit(EXIT_FAILURE);
            }
            // WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
            // kill(GetValue(&workersPidList,i),SIGUSR1);
            kill((long)GetItem_MyVector(bufferWorker,i),SIGUSR1);
            printf("4\n");
        }
    }
    else
    {
        // store country
        char * country;
        country = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(country,(const  char *)tok);

        for (long i = 0; i < totalWorkers; i++)
        {

            char * path = malloc(sizeof(char)*strlen(GetValue_Path(&country,i)) + 1);
            // strcpy(path,GetValue_Path(&subDirectoriesPathList,i));
            // path = strtok(tok,delimiters);
            // path = strtok(tok,delimiters);
            // path = strtok(tok,delimiters);

            if(!strcmp(path,country))
            {
                sprintf(message,"/diseaseFrequency %s %ld-%ld-%ld %ld-%ld-%ld %s",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year, country);
                printf("%s\n",message);
                // WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
                if(write((GetItem_MyVector(bufferWorker,i), message, strlen(message)) < 0)
                    {
                        perror("Request2 write");
                        exit(EXIT_FAILURE);
                    }
                // kill(GetValue(&workersPidList,i),SIGUSR1);
                kill(GetItem_MyVector(bufferWorker,i),SIGUSR1);
                printf("5\n");
            }

        }
    }

    free(date1);
    free(date2);


    char result[MAXIMUMBUFFER];
    int bytes;
    long res = 0;
    for (long i = 0; i < totalWorkers; i++)
    {
        bytes=read(GetItem_MyVector(bufferWorker,i), result, MAXIMUMBUFFER);
        res += atol(result);

    }
    printf("%ld\n",res);
}
