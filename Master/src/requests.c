#include "../include/requests.h"

extern Node * writeNamedPipeList;
extern long totalWorkers;
extern long bufferSize;
extern char * path;
extern Node * workersPidList;
extern Node * readNamedPipeList;
extern long flagEliminate;
extern PathNode * subDirectoriesPathList;

extern PathNode * queries;
extern PathNode * queriesResults;

void Request_1()
{

    char message[MAXIMUMBUFFER];

    // send to every worker listCountriess
    for (long i = 0; i < totalWorkers; i++)
    {
        sprintf(message,"/listCountries %s",GetValue_Path(&subDirectoriesPathList,i));
        WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
        kill(GetValue(&workersPidList,i),SIGUSR1);

    }

    char result[MAXIMUMBUFFER] = "";
    char resultFinal[MAXIMUMBUFFER] = "/listCountries\n";
    int bytes;
    for (long i = 0; i < totalWorkers; i++)
    {
        do
        {
            usleep(100);

        }while((bytes=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);
        printf("%s\n",result);
        strcat(resultFinal,result);
    }
    PushNode_Path(&queriesResults, resultFinal);

}

void Request_2(char * tok)
{
    char message[MAXIMUMBUFFER];
    char resultFinal[MAXIMUMBUFFER] = "";

    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    char * diseaseID;

    tok = strtok(NULL," ");
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

    // // date1
    tok = strtok(NULL, delimiters);

    date1 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> year = (long)atoi(tok);

    // date2
    tok = strtok(NULL,delimiters);

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

    if(tok == NULL)
    {

        for (long i = 0; i < totalWorkers; i++)
        {
            sprintf(message,"/diseaseFrequency %s %ld-%ld-%ld %ld-%ld-%ld",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year);
            sprintf(resultFinal,"/diseaseFrequency %s %ld-%ld-%ld %ld-%ld-%ld\n",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year);

            WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
            kill(GetValue(&workersPidList,i),SIGUSR1);
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

            char * path = malloc(sizeof(char)*strlen(GetValue_Path(&subDirectoriesPathList,i)) + 1);
            strcpy(path,GetValue_Path(&subDirectoriesPathList,i));
            path = strtok(tok,delimiters);
            path = strtok(tok,delimiters);
            path = strtok(tok,delimiters);

            if(!strcmp(path,country))
            {
                sprintf(message,"/diseaseFrequency %s %ld-%ld-%ld %ld-%ld-%ld %s",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year, country);
                sprintf(resultFinal,"/diseaseFrequency %s %ld-%ld-%ld %ld-%ld-%ld %s\n",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year, country);

                WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
                kill(GetValue(&workersPidList,i),SIGUSR1);

            }
        }
    }

    free(date1);
    free(date2);


    char result[MAXIMUMBUFFER];
    char resultTemp[MAXIMUMBUFFER] = "";
    int bytes;
    long res = 0;
    for (long i = 0; i < totalWorkers; i++)
    {
        do
        {
            usleep(100);

        }while((bytes=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);
        res += atol(result);

    }
    sprintf(resultTemp,"%ld\n",res);
    strcat(resultFinal,resultTemp);
    PushNode_Path(&queriesResults,resultFinal);
    printf("%ld\n",res);
}

void Request_3(char * tok)
{
    char message[MAXIMUMBUFFER];
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    // store k
    char * k;
    tok = strtok(NULL," ");
    k = ( char *)malloc(1 + sizeof(char) * strlen(tok));
    strcpy(k,(const  char *)tok);

    // store country
    tok = strtok(NULL," ");
    char * country;
    country = ( char *)malloc(1 + sizeof(char) * strlen(tok));
    strcpy(country,(const  char *)tok);

    // store diseaseID
    char * diseaseID;
    tok = strtok(NULL," ");
    diseaseID = ( char *)malloc(1 + sizeof(char) * strlen(tok));
    strcpy(diseaseID,(const  char *)tok);


    Date * date1 = NULL;
    Date * date2 = NULL;
    date1 = malloc(sizeof(*date1));
    date2 = malloc(sizeof(*date2));

    // // date1
    tok = strtok(NULL, delimiters);
    date1 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> year = (long)atoi(tok);

    // date2
    tok = strtok(NULL,delimiters);
    date2 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date2 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date2 -> year = (long)atoi(tok);
    //
    //

    for (long i = 0; i < totalWorkers; i++)
    {
        tok = NULL;
        char * currentCountry = (char *)malloc(1 + sizeof(char)*strlen(GetValue_Path(&subDirectoriesPathList,i)));
        strcpy(currentCountry, GetValue_Path(&subDirectoriesPathList,i));

        tok = strtok(currentCountry, delimiters);
        tok = strtok(NULL, delimiters);
        tok = strtok(NULL, delimiters);
        tok = strtok(NULL, delimiters);
        if(!country)
        {
            printf("Flag\n");
        }
        else
        {
            if(!strcmp(country,tok))
            {
                sprintf(message,"/topk-AgeRanges %s %s %s %ld-%ld-%ld %ld-%ld-%ld",k, country, diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year);

                WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
                kill(GetValue(&workersPidList,i),SIGUSR1);
            }
        }

    }


    char result[MAXBUFFER];
    char resultFinal[MAXIMUMBUFFER] = "";
    sprintf(resultFinal,"/topk-AgeRanges %s %s %s %ld-%ld-%ld %ld-%ld-%ld\n",k, country, diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year);
    int bytes;

    for (long i = 0; i < totalWorkers; i++)
    {
        char * currentCountry = (char *)malloc(1 + sizeof(char)*strlen(GetValue_Path(&subDirectoriesPathList,i)));
        strcpy(currentCountry, GetValue_Path(&subDirectoriesPathList,i));

        tok = strtok(currentCountry, delimiters);
        tok = strtok(NULL, delimiters);
            tok = strtok(NULL, delimiters);
            tok = strtok(NULL, delimiters);
            if(!strcmp(country,tok))
            {
                do
                {
                    usleep(100);

                }while((bytes=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);

                printf("%s\n",result);
                strcat(resultFinal,result);
            }


    }
    PushNode_Path(&queriesResults,resultFinal);

}

// /topk-AgeRanges 3 China COVID-2019 10-10-2010 10-10-2020

void Request_4(char * recordID)
{

    char message[MAXIMUMBUFFER];
    char resultFinal[MAXIMUMBUFFER] = "";
    for (long i = 0; i < totalWorkers; i++)
    {
        sprintf(message,"/searchPatientRecord %s",recordID);
        WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
        kill(GetValue(&workersPidList,i),SIGUSR1);

    }

    char result[MAXIMUMBUFFER];
    long bytes;
    long counterNotFound = 0;
    for (long i = 0; i < totalWorkers; i++)
    {
        do
        {
            usleep(100);

        }while((bytes=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);
        if(strcmp(result,"Not Found"))
        {
            sprintf(resultFinal,"/searchPatientRecord %s\n%s\n",recordID,result);
            printf("%s\n", result);
        }
        else
        {
            counterNotFound++;
        }
        // printf("%s\n",result);
    }
    if(counterNotFound == totalWorkers)
    {
        sprintf(resultFinal,"/searchPatientRecord %s\n%s\n",recordID,result);
        printf("%s\n",result);
    }
    PushNode_Path(&queriesResults,resultFinal);
}


void Request_5(char * tok)
{
    char message[MAXIMUMBUFFER];
    char resultFinal[MAXIMUMBUFFER] = "";
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    char * diseaseID;

    tok = strtok(NULL," ");
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

    // // date1
    tok = strtok(NULL, delimiters);

    date1 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> year = (long)atoi(tok);

    // date2
    tok = strtok(NULL,delimiters);

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


    tok = strtok(NULL,delimiters);
    // without country
    char * country = NULL;

    if(tok == NULL)
    {

        for (long i = 0; i < totalWorkers; i++)
        {

            sprintf(message,"/numPatientAdmissions %s %ld-%ld-%ld %ld-%ld-%ld\n",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year);
            sprintf(resultFinal,"/numPatientAdmissions %s %ld-%ld-%ld %ld-%ld-%ld\n",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year);

            WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
            kill(GetValue(&workersPidList,i),SIGUSR1);
        }
    }
    else
    {
        // store country

        country = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(country,(const  char *)tok);

        for (long i = 0; i < totalWorkers; i++)
        {

            char * path = malloc(sizeof(char)*strlen(GetValue_Path(&subDirectoriesPathList,i)) + 1);
            strcpy(path,GetValue_Path(&subDirectoriesPathList,i));
            path = strtok(tok,delimiters);
            path = strtok(tok,delimiters);
            path = strtok(tok,delimiters);

            if(!strcmp(path,country))
            {
                sprintf(message,"/numPatientAdmissions %s %ld-%ld-%ld %ld-%ld-%ld %s\n",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year, country);
                sprintf(resultFinal,"/numPatientAdmissions %s %ld-%ld-%ld %ld-%ld-%ld %s\n",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year, country);

                WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
                kill(GetValue(&workersPidList,i),SIGUSR1);

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
        char resultTemp[MAXIMUMBUFFER] = "";
        do
        {
            usleep(100);

        }while((bytes=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);
        res += atol(result);
        char * tok = NULL;
        char * currentCountry = (char *)malloc(1 + sizeof(char)*strlen(GetValue_Path(&subDirectoriesPathList,i)));
        strcpy(currentCountry, GetValue_Path(&subDirectoriesPathList,i));

        tok = strtok(currentCountry, delimiters);
        tok = strtok(NULL, delimiters);
        tok = strtok(NULL, delimiters);
        tok = strtok(NULL, delimiters);
        if(!country)
        {

            printf("%s %ld\n",tok, atol(result));
            sprintf(resultTemp,"%s %ld\n",tok, atol(result));
            strcat(resultFinal,resultTemp);
        }
        else
        {
            if(!strcmp(country,tok))
            {
                printf("%s %ld\n",tok, atol(result));
                sprintf(resultTemp,"%s %ld\n",tok, atol(result));
                strcat(resultFinal,resultTemp);
            }
        }


    }
    PushNode_Path(&queriesResults, resultFinal);

}



void Request_6(char * tok)
{
    char message[MAXIMUMBUFFER];
    char resultFinal[MAXIMUMBUFFER] = "";
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    char * diseaseID;

    tok = strtok(NULL," ");
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

    // // date1
    tok = strtok(NULL, delimiters);

    date1 -> day = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> month = (long)atoi(tok);

    tok = strtok(NULL,delimiters);
    date1 -> year = (long)atoi(tok);

    // date2
    tok = strtok(NULL,delimiters);

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


    tok = strtok(NULL,delimiters);
    // without country
    char * country = NULL;

    if(tok == NULL)
    {

        for (long i = 0; i < totalWorkers; i++)
        {

            sprintf(message,"/numPatientDischarges %s %ld-%ld-%ld %ld-%ld-%ld",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year);

            WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
            kill(GetValue(&workersPidList,i),SIGUSR1);
        }
    }
    else
    {
        // store country

        country = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(country,(const  char *)tok);

        for (long i = 0; i < totalWorkers; i++)
        {

            char * path = malloc(sizeof(char)*strlen(GetValue_Path(&subDirectoriesPathList,i)) + 1);
            strcpy(path,GetValue_Path(&subDirectoriesPathList,i));
            path = strtok(tok,delimiters);
            path = strtok(tok,delimiters);
            path = strtok(tok,delimiters);

            if(!strcmp(path,country))
            {
                sprintf(message,"/numPatientDischarges %s %ld-%ld-%ld %ld-%ld-%ld %s",diseaseID, date1 -> day, date1 -> month, date1 -> year,date2 -> day, date2 -> month, date2 -> year, country);

                WriteToNamedPipe(GetValue(&writeNamedPipeList,i), message);
                kill(GetValue(&workersPidList,i),SIGUSR1);

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
        char resultTemp[MAXIMUMBUFFER] = "";
        do
        {
            usleep(100);

        }while((bytes=ReadFromNamedPipe(GetValue(&readNamedPipeList,i), result))<=0);
        res += atol(result);
        char * tok = NULL;
        char * currentCountry = (char *)malloc(1 + sizeof(char)*strlen(GetValue_Path(&subDirectoriesPathList,i)));
        strcpy(currentCountry, GetValue_Path(&subDirectoriesPathList,i));

        tok = strtok(currentCountry, delimiters);
        tok = strtok(NULL, delimiters);
        tok = strtok(NULL, delimiters);
        tok = strtok(NULL, delimiters);
        if(!country)
        {
            printf("%s %ld\n",tok, atol(result));
            sprintf(resultTemp,"%s %ld\n",tok, atol(result));
            strcat(resultFinal,resultTemp);
        }
        else
        {
            if(!strcmp(country,tok))
            {
                printf("%s %ld\n",tok, atol(result));
                sprintf(resultTemp,"%s %ld\n",tok, atol(result));
                strcat(resultFinal,resultTemp);
            }
        }

    }

    PushNode_Path(&queriesResults, resultFinal);

}

void reCreate()
{
    long i = 0;
    kill(GetValue(&workersPidList,i),SIGSEGV);
}


static long Read_Requests_Parse(char * request)
{

    char * tok;
    char delimiters[] = " \n\t\r\v\f\n:,.><[]{}|=+*@#$";
    tok = strtok(request,delimiters);

    if(tok != NULL)
    {
        if(!strcmp(tok,"/listCountries"))
        {
            Request_1();

            return false;

        }
        else if(!strcmp(tok,"/diseaseFrequency"))
        {
            Request_2(tok);
            return false;

        }
        else if(!strcmp(tok,"/topk-AgeRanges"))
        {
            Request_3(tok);
            return false;

        }
        else if(!strcmp(tok,"/searchPatientRecord"))
        {
            tok = strtok(NULL, delimiters);
            Request_4(tok);
            return false;
        }
        else if(!strcmp(tok,"/numPatientAdmissions"))
        {
            Request_5(tok);
            return false;
        }
        else if(!strcmp(tok,"/numPatientDischarges"))
        {
            Request_6(tok);
            return false;
        }
        else if(!strcmp(tok,"/reCreateWorker"))
        {
            reCreate();
            return false;
        }
        else if(!strcmp(tok,"/exit"))
        {
            printf("exiting\n");
            Elimination();
            return true;

        }
        return false;

    }
    return true;

}


long Read_Requests()
{

    for (long i = 0; i < LenOfList(queries); i++)
    {
        Read_Requests_Parse(GetValue_Path(&queries,i));
    }

    printf("---------------------\n");
    PrintList_Path(&queriesResults);


    char messageSend[MAXIMUMBUFFER*4] = "Requests\n";
    for (long i = 0; i < LenOfList(queriesResults); i++)
    {
        strcat(messageSend,GetValue_Path(&queriesResults,i));
    }
    for (long i = 0; i < totalWorkers; i++)
    {
        WriteToNamedPipe(GetValue(&writeNamedPipeList,i), messageSend);
        kill(GetValue(&workersPidList,i),SIGUSR1);
    }


    return -1;
}
