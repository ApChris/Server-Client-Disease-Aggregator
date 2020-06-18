#include "../include/workerFunctions.h"


extern long fileDescriptorR;
extern long fileDescriptorW;
extern long processID;
extern PathNode * filesPathList;
extern Hash * diseaseHash;
extern Hash * patientHash;

extern long errorRecords;
extern long successRecords;
extern PathNode * countries;

extern SumStatistics * generalStatistics;
extern long buffersize;

extern long workerSock;

extern PathNode * workerQueries;

extern char workerCountry[MAXIMUMBUFFER];


// This function is rensponsible for filling the hash with file's records
SumStatistics * FillStructures(const char * patientRecordsFile, Hash * diseaseHash, Hash * patientHash, Date * date, char * country)
{
    // for getline
    char * line = NULL;
    size_t length = 0;
    long read;
    FILE * file;

    // variables that I read from file
    char * recordID = NULL;
    char * status = NULL;
    char * patientFirstName = NULL;
    char * patientLastName = NULL;
    char * diseaseID = NULL;
    long age = TAG;
    Date * entryDate = NULL;
    Date * exitDate = NULL;

    SumStatistics * statisticsList = NULL;
    // for strtok

    char * tok = NULL;

    // The struct that we are going to fill

    entryDate = malloc(sizeof(*entryDate));
    exitDate = malloc(sizeof(*exitDate));

    file = fopen(patientRecordsFile, "r");
    // printf("%s has been opened successfully!!\n\n", patientRecordsFile); // Feedback

    while((read = getline(&line,&length, file)) != -1)
    {
        PatientInfo * info = NULL;

        // Get recordID
        tok = strtok(line, " ");
        recordID = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(recordID,(const char *)tok);

        // Read status
        tok = strtok(NULL," ");
        status = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(status,(const  char *)tok);


        // Read patientFirstName
        tok = strtok(NULL," ");
        patientFirstName = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(patientFirstName,(const  char *)tok);

        // Read patientLastName
        tok = strtok(NULL," ");
        patientLastName = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(patientLastName,(const  char *)tok);

        // Read diseaseID
        tok = strtok(NULL," ");
        diseaseID = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(diseaseID,(const  char *)tok);

        // Read age
        tok = strtok(NULL," ");
        age = atol(tok);

        // if current patient doessn't
        if(!strcmp(status,"ENTER"))
        {
            long flag = TAG;
            if(age < 20)
            {
                flag = 0;
            }
            else if (age < 40)
            {
                flag = 1;
            }
            else if(age < 60)
            {
                flag = 2;
            }
            else
            {
                flag = 3;
            }
            if(SearchInList_Statistics(&statisticsList,diseaseID,flag)){}
            else
                PushNode_Statistics(&statisticsList,diseaseID,flag);


            // Flag to fix the print function
            exitDate -> day = TAG;
            entryDate -> day = date -> day;
            entryDate -> month = date -> month;
            entryDate -> year = date -> year;
            info = PatientInfo_Init(recordID,patientFirstName,patientLastName,diseaseID,country, age, entryDate, exitDate);      // create the
            Hash_Insert(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),info);
            Hash_Insert(patientHash,Hash_Function_DJB2((unsigned char *)recordID),info);

            free(recordID);
            free(patientFirstName);
            free(patientLastName);
            free(diseaseID);

            continue;
        }


        info = Hash_Find_Patient(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID), diseaseID);
        if(info == NULL)
        {
            UpdateErrors_Statistics(&statisticsList,diseaseID);
            printf("ERROR\n");

            // printf("RecordID = %s - %s\n",recordID, status);
            free(recordID);
            free(patientFirstName);
            free(patientLastName);
            free(diseaseID);
            continue;
        }

        info -> exitDate -> day = date -> day;
        info -> exitDate -> month = date -> month;
        info -> exitDate -> year = date -> year;


        free(recordID);
        free(patientFirstName);
        free(patientLastName);
        free(diseaseID);


    }
    free(entryDate);
    free(exitDate);
    free(line);
    fclose(file);

    return statisticsList;
}


void reCreateWorker()
{
    raise(SIGSEGV);
}

void Print_Input(char * patientRecordsFile, long diseaseHashtableNumOfEntries, long countryHashtableNumOfEntries, long bucketSize)
{
    printf("\nYour input was:\n\n-> patientRecordsFile = %s\n-> diseaseHashtableNumOfEntries = %ld\n"
    "-> countryHashtableNumOfEntries = %ld\n-> bucketSize = %ld\n",patientRecordsFile,diseaseHashtableNumOfEntries,countryHashtableNumOfEntries,bucketSize);

}




void listCountries(char * path)
{

    char messageStatistics[MAXBUFFER] = "";

    sprintf(messageStatistics, "%s %ld", workerCountry, (long)getpid());
    WriteToNamedPipe(fileDescriptorW,messageStatistics);
    // write(workerSock,messageStatistics,strlen(messageStatistics));

}

void diseaseFrequency(char * arguments)
{

    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    char * diseaseID;

    char * tok = strtok(arguments," ");
    if(tok == NULL)
    {
        printf("error\n");
        return ;
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
        tResult = 0;
        long res;
        res = Hash_getPatientsInThatPeriod(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),diseaseID,date1,date2,"NULL",0);

        char message[MAXBUFFER];

        sprintf(message,"%ld\n", res);
        WriteToNamedPipe(fileDescriptorW,message);

        // write(workerSock,message,strlen(message));
    }
    // user gave a country
    else
    {
        tResult = 0;
        // store country
        char * country;
        country = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(country,(const  char *)tok);

        tResult = Hash_getPatientsInThatPeriod(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),diseaseID,date1,date2,country,1);
        char message[MAXBUFFER];
        sprintf(message,"%ld\n",tResult);
        WriteToNamedPipe(fileDescriptorW,message);
        // write(workerSock,message,strlen(message));
        free(country);
    }
    free(date1);
    free(date2);

}



void topkAgeRanges(char * arguments)
{
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    // store k
    char * k;

    char * tok = strtok(arguments," ");
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

    SumStatistics * generalStatisticsTemp = (SumStatistics *)malloc(sizeof(SumStatistics));
    generalStatisticsTemp -> diseaseID = NULL;
    generalStatisticsTemp -> cases_0_20 = 0;
    generalStatisticsTemp -> cases_21_40 = 0;
    generalStatisticsTemp -> cases_41_60 = 0;
    generalStatisticsTemp -> cases_over_60 = 0;
    Hash_getStatisticsPatientsInThatPeriod(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),diseaseID,date1,date2,country,generalStatisticsTemp);

    char message[MAXBUFFER];
    long totalPatients = generalStatisticsTemp -> cases_0_20 + generalStatisticsTemp -> cases_21_40 + generalStatisticsTemp -> cases_41_60 + generalStatisticsTemp -> cases_over_60;
    double percentCase[4];
    percentCase[0] = 100*((double)(generalStatisticsTemp -> cases_0_20)/totalPatients);
    percentCase[1] = 100*((double)(generalStatisticsTemp -> cases_21_40)/totalPatients);
    percentCase[2] = 100*((double)(generalStatisticsTemp -> cases_41_60)/totalPatients);
    percentCase[3] = 100*((double)(generalStatisticsTemp -> cases_over_60)/totalPatients);


    if(totalPatients == 0)
    {
        sprintf(message,"Not found any patient!\n");
        WriteToNamedPipe(fileDescriptorW,message);
        // write(workerSock,message,strlen(message));
        free(date1);
        free(date2);
        return;
    }
    long counter = 0;
    long max = -1;
    long position = -1;
    while(counter < atol(k))
    {
        for (long i = 0; i < 4; i++)
        {
            if(percentCase[i] > max)
            {
                max = percentCase[i];
                position = i;
            }
        }

        if(position == 0)
        {
            sprintf(message,"0-20: %0.lf%%\n",percentCase[position]);
            WriteToNamedPipe(fileDescriptorW,message);
            // write(workerSock,message,strlen(message));
        }
        else if(position == 1)
        {
            sprintf(message,"21-40: %0.lf%%\n",percentCase[position]);
            WriteToNamedPipe(fileDescriptorW,message);
            // write(workerSock,message,strlen(message));
        }
        else if(position == 2)
        {
            sprintf(message,"41-60: %0.lf%%\n",percentCase[position]);
            WriteToNamedPipe(fileDescriptorW,message);
            // write(workerSock,message,strlen(message));
        }
        else
        {

            sprintf(message,"60+: %0.lf%%\n",percentCase[position]);
            WriteToNamedPipe(fileDescriptorW,message);
                // write(workerSock,message,strlen(message));

        }
        percentCase[position] = -1;
        counter++;
        max = -1;
        position = -1;
    }

    free(date1);
    free(date2);
    return;
}




void searchPatientRecord(char * recordID)
{
    char message[MAXBUFFER];

    PatientInfo * info = Hash_Find_Patient(patientHash,Hash_Function_DJB2((unsigned char *)recordID), recordID);

    if(info == NULL)
    {
        sprintf(message, "Not Found");
        WriteToNamedPipe(fileDescriptorW,message);
        // write(workerSock,message,strlen(message));
    }
    else
    {
        if(info -> exitDate -> day == TAG)
        {
            sprintf(message, "%s %s %s %s %s %ld %ld-%ld-%ld --",info -> recordID, info -> patientFirstName, info -> patientLastName,info -> diseaseID, info -> country, info -> age, info -> entryDate -> day, info -> entryDate -> month, info -> entryDate -> year);
        }
        else
        {
            sprintf(message, "%s %s %s %s %s %ld %ld-%ld-%ld %ld-%ld-%ld",info -> recordID, info -> patientFirstName, info -> patientLastName,info -> diseaseID, info -> country, info -> age, info -> entryDate -> day, info -> entryDate -> month, info -> entryDate -> year, info -> exitDate -> day, info -> exitDate -> month, info -> exitDate -> year);
        }
        WriteToNamedPipe(fileDescriptorW,message);
        // write(workerSock,message,strlen(message));
    }
}



void numPatientAdmissions(char * arguments)
{
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    char * diseaseID;

    char * tok = strtok(arguments," ");
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
        tResult = 0;
        long res;
        res = Hash_getPatientsInThatPeriod(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),diseaseID,date1,date2,"NULL",0);

        char message[MAXBUFFER];

        sprintf(message,"%ld\n", res);
        WriteToNamedPipe(fileDescriptorW,message);
        // write(workerSock,message,strlen(message));

    }
    // user gave a country
    else
    {
        tResult = 0;
        // store country
        char * country;
        country = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(country,(const  char *)tok);

        tResult = Hash_getPatientsInThatPeriod(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),diseaseID,date1,date2,country,1);
        char message[MAXBUFFER];
        sprintf(message,"%ld\n", tResult);
        WriteToNamedPipe(fileDescriptorW,message);
        // write(workerSock,message,strlen(message));
        free(country);
    }
    free(date1);
    free(date2);
    return;
}



void numPatientDischarges(char * arguments)
{
    char delimiters[] = " \n\t\r\v\f\n-:,/.><[]{}|-=+*@#$;";

    char * diseaseID;

    char * tok = strtok(arguments," ");
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
    if(tok == NULL)
    {
        tResult = 0;
        long res;
        res = Hash_getExitPatientsInThatPeriod(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),diseaseID,date1,date2,"NULL",0);

        char message[MAXBUFFER];

        sprintf(message,"%ld\n", res);
        WriteToNamedPipe(fileDescriptorW,message);
        // write(workerSock,message,strlen(message));

    }
    // user gave a country
    else
    {
        tResult = 0;
        // store country
        char * country;
        country = ( char *)malloc(1 + sizeof(char) * strlen(tok));
        strcpy(country,(const  char *)tok);

        tResult = Hash_getExitPatientsInThatPeriod(diseaseHash,Hash_Function_DJB2((unsigned char *)diseaseID),diseaseID,date1,date2,country,1);
        char message[MAXBUFFER];
        sprintf(message,"%ld\n", tResult);
        WriteToNamedPipe(fileDescriptorW,message);
        // write(workerSock,message,strlen(message));
        free(country);
    }
    free(date1);
    free(date2);
    return;
}




void ReadingFiles(char * path)
{
    printf("%s\n",path);
    // Get country from subDirectoryPath
    char * country = (char *)malloc(1 + sizeof(char) * strlen(path));
    strcpy(country,path);

    char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
    char * tok = NULL;
    tok = strtok(country, delimiters);
    tok = strtok(NULL, delimiters);
    tok = strtok(NULL, delimiters);
    tok = strtok(NULL, delimiters);
    strcpy(country,tok);
    PushNode_Path(&countries,country);
    strcpy(workerCountry,country);
    Date * datesArray = malloc(sizeof(*datesArray));

    struct dirent * subDirectory;

    DIR * subDirectoryPointer;

    if( ( subDirectoryPointer = opendir(path)) == NULL)
    {

        perror("Error: SubDirectory has not been opened! - WORKER");
        exit(EXIT_FAILURE);
    }

    long counter = 0;
    char * currentDate = NULL;
    while( ( subDirectory = readdir(subDirectoryPointer)) != NULL)
    {

        if((!strcmp(subDirectory -> d_name, ".")) || (!strcmp(subDirectory -> d_name, "..")))
        {
            continue;
        }
        currentDate = malloc(1 + sizeof(char) * strlen(subDirectory -> d_name));
        strcpy(currentDate,subDirectory -> d_name);
        char * tok = NULL;
        if(counter == 0) // only for first file
        {

            tok = strtok(currentDate, delimiters);
            datesArray[0].day = atol(tok);
            tok = strtok(NULL, delimiters);
            datesArray[0].month = atol(tok);
            tok = strtok(NULL, delimiters);
            datesArray[0].year = atol(tok);
            counter++;
            free(currentDate);
            continue;
        }
        if( (datesArray = realloc(datesArray, sizeof(*datesArray)*(counter + 1)) ) == NULL)
        {
            perror("Error: Realloc has been failed!- WORKER");
            exit(EXIT_FAILURE);
        }

        tok = strtok(currentDate, delimiters);

        datesArray[counter].day = atol(tok);

        tok = strtok(NULL, delimiters);
        datesArray[counter].month = atol(tok);
        tok = strtok(NULL, delimiters);
        datesArray[counter].year = atol(tok);
        counter++;
        free(currentDate);

    }
    closedir(subDirectoryPointer);


    // Sort dates and push them in a list
    long min = 0;
    for (long i = 0; i < counter; i++)
    {
        for (long j = 0; j < counter; j++)
        {
            if(j+1 <= counter)
            {
                long result = Compare_Date(&datesArray[j],&datesArray[min]);
                if(result == -1)
                {
                    min = j;
                }
            }

        }

        char currentMinDate[12];

        sprintf(currentMinDate, "%ld-%ld-%ld", datesArray[min].day, datesArray[min].month, datesArray[min].year);

        datesArray[min].year = TAG;
        min = 0;

        char * name = (char *)malloc(sizeof(char) * strlen(path) + strlen(currentMinDate) + 2);
        strcpy(name, path);
        strcat(name, "/");
        strcat(name, currentMinDate);

        PushNode_Path(&filesPathList,name);
        free(name);
    }

    Reverse_Path(&filesPathList);
    PrintList_Path(&filesPathList);


    diseaseHash = Hash_Init(11, 512);
    patientHash = Hash_Init(11, 512);
    for (long i = 0; i < LenOfList(filesPathList); i++)
    {

        char * currentDate = malloc(1 + sizeof(char) * strlen(GetValue_Path(&filesPathList, i)));
        strcpy(currentDate,GetValue_Path(&filesPathList, i));
        Date * cdate = malloc(sizeof(*cdate));
        char * tok = NULL;
        tok = strtok(currentDate, delimiters);
        tok = strtok(NULL, delimiters);
        tok = strtok(NULL, delimiters);
        tok = strtok(NULL, delimiters);
        // day
        tok = strtok(NULL, delimiters);

        cdate -> day = atol(tok);

        tok = strtok(NULL, delimiters);
        cdate -> month = atol(tok);
        tok = strtok(NULL, delimiters);
        cdate -> year = atol(tok);

        SumStatistics * statistics = FillStructures(GetValue_Path(&filesPathList, i), diseaseHash, patientHash, cdate, country);

        char messageStatistics[MAXBUFFER];
        // printf("%ld %ld %ld\n", cdate -> day, cdate -> month, cdate -> year);
        long flag = 0;
        while(statistics != NULL)
        {
            errorRecords += statistics -> errorRecords;
            successRecords += statistics -> successRecords;
            if(flag == 0)
            {
                sprintf(messageStatistics, "\n%ld-%ld-%ld\n%s\n%s\nAge range 0-20 years: %ld cases\nAge range 21-40 years: %ld cases\nAge range 41-60 years: %ld cases\nAge range 65+ years: %ld cases\n",cdate -> day, cdate -> month, cdate -> year, country, statistics -> diseaseID,statistics -> cases_0_20,statistics -> cases_21_40,statistics -> cases_41_60,statistics -> cases_over_60);
                flag++;
                statistics = statistics -> next;

                // send to server
                if (write(workerSock, messageStatistics, strlen(messageStatistics)) < 0)
                {
                    perror("worker write");
                    exit(EXIT_FAILURE);
                }
                WriteToNamedPipe(fileDescriptorW,messageStatistics);

            }
            else
            {
                sprintf(messageStatistics, "\n%s\nAge range 0-20 years: %ld cases\nAge range 21-40 years: %ld cases\nAge range 41-60 years: %ld cases\nAge range 65+ years: %ld cases\n",statistics -> diseaseID,statistics -> cases_0_20,statistics -> cases_21_40,statistics -> cases_41_60,statistics -> cases_over_60);
                statistics = statistics -> next;

                // send to server
                if (write(workerSock, messageStatistics, strlen(messageStatistics)) < 0)
                {
                    perror("worker write");
                    exit(EXIT_FAILURE);
                }

                WriteToNamedPipe(fileDescriptorW,messageStatistics);

            }

        }


        free(currentDate);
        free(cdate);
        free(statistics);
    }

    char tempMessage[MAXIMUMBUFFER] = "";
    read(workerSock,tempMessage,MAXIMUMBUFFER);

    sprintf(tempMessage,"%ld-%s\n",(long)getpid(), workerCountry);

    if (write(workerSock, tempMessage, strlen(tempMessage)) < 0)
    {
        perror("Worker - Reading files write");
        exit(EXIT_FAILURE);
    }
    // Deallocates
    free(datesArray);
    free(country);

    DeleteList_Path(&filesPathList);
    free(filesPathList);


}




void Elimination()
{

    close(fileDescriptorW);
    close(fileDescriptorR);
    PrintList_Path(&countries);

    FILE * file;
    char fileName[MAXIMUMBUFFER];

    sprintf(fileName, "etc/logfiles/log_file.%ld.txt", (long)getpid());
    file = fopen(fileName, "w");

    for (long i = 0; i < LenOfList(countries); i++)
    {
        fprintf(file,"%s\n",GetValue_Path(&countries,i));

    }
    fprintf(file, "TOTAL %ld\nSUCCESS %ld\nFAIL %ld\n",successRecords + errorRecords, successRecords, errorRecords);

    fclose(file);

    exit(0);
}







void ReadRequests(char * path)
{
    char message[MAXIMUMBUFFER] = "";
    // select()
    fd_set set;
    struct timeval timeOut;
    long rv;
    long lines = 0;
    char * tok = NULL;
    char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";

    if (read(workerSock, message, MAXIMUMBUFFER) < 0)
    {
        perror("worker read");
        exit(EXIT_FAILURE);
    }
    // if (write(workerSock, "Accepted", strlen("Accepted") + 1) < 0)
    // {
    //     perror("worker write");
    //     exit(EXIT_FAILURE);
    // }
    printf("--------Worker: %ld-------\n%s\n", processID,message);
    for (long i = 0; i < strlen(message); i++)
    {
        if(message[i] == '\n')
        {
            lines++;
        }
    }
    tok = strtok(message, "\n");
    PushNode_Path(&workerQueries,message);
    // printf("%s\n",tok);
    for (long i = 1; i < lines; i++)
    {
        tok = strtok(NULL,"\n");
        PushNode_Path(&workerQueries,tok);
        // printf("%s\n",tok);
    }
    // PrintList_Path(&workerQueries);
    for (long i = 0; i < lines; i++)
    {
        char * command = (char *)malloc(sizeof(char)*MAXIMUMBUFFER);
        char * arguments;
        char * tok = NULL;
        strcpy(command, GetValue_Path(&workerQueries,i));
        command = strtok(command," ");
        arguments = strtok(NULL,"\n");
            // printf("-->%s\n",command);
            // printf("------->%s\n",arguments);
            if(!strcmp(command, "/listCountries"))
            {

            }
            else if(!strcmp(command, "/diseaseFrequency"))
            {
                diseaseFrequency(arguments);

            }
            else if(!strcmp(command, "/topk-AgeRanges"))
            {

                topkAgeRanges(arguments);

            }
            else if(!strcmp(command, "/numPatientAdmissions"))
            {
                numPatientAdmissions(arguments);

            }
            else if(!strcmp(command, "/searchPatientRecord"))
            {
                char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
                char * tok = NULL;
                tok = strtok(arguments, delimiters);

                searchPatientRecord(tok);

            }
            else if(!strcmp(command, "/numPatientDischarges"))
            {
                numPatientDischarges(arguments);

            }
            // else if(!strcmp(command, "/reCreateWorker"))
            // {
            //     reCreateWorker();
            // }
            // else if(!strcmp(command, "/ReadRequests"))
            // {
            //     char * path;
            //     // char * procID;
            //     char delimiters[] = " \n\t\r\v\f\n:,/.><[]{}|=+*@#$-";
            //     char * tok = NULL;
            //     tok = strtok(arguments, delimiters);
            //     // procID = (char *)malloc(sizeof(char)* strlen(tok));
            //     // strcpy(procID,tok);
            //     tok = strtok(NULL, " \n");
            //     path = (char *)malloc(sizeof(char)* strlen(tok));
            //     strcpy(path,tok);
            //     // printf("%s\n",path);
            //     ReadRequests(path);
            // }
            // else
            // {
            //     printf("Wrong input\n");
            // }

            // free(command);

    }
    printf("%ld\n",lines);
    // while(1)
    // {
    //     FD_ZERO(&set);
    //     FD_SET(workerSock, &set);
    //
    //     timeOut.tv_sec = 10;
    //     timeOut.tv_usec = 0;
    //
    //     rv = select(workerSock + 1, &set, NULL, NULL, &timeOut);
    //
    //     if(rv == -1)
    //     {
    //         perror("Error select");
    //         exit(EXIT_FAILURE);
    //     }
    //     else if(rv == 0)
    //     {
    //         // if(totalWorkers == 0)
    //         // {
    //             printf("--> 10 seconds have passed -> Still listening for Workers\n");
    //         // }
    //         // else
    //         // {
    //         //     printf("--> 10 seconds have passed -> Thread is moving on Clients\n");
    //         //     break;
    //         //
    //         // }
    //     }
    //     else
    //     {
    //
    //     }
    // }

}
