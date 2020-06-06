#include "../include/hash.h"


long long Hash_Function_DJB2(unsigned char *str) // This function IS NOT mine, source: djb2 by Dan Bernstein
{
    unsigned long hash = 5381;
    int c;

    while( (c = *str++) != '\0')
    {
        hash = ((hash << 5) + hash) + c;

    }
    return hash;
}



// ----------------------------- PATIENTINFO -----------------------------

PatientInfo * PatientInfo_Init(const char * recordID, const char * patientFirstName ,const char * patientLastName,const char * diseaseID, const char * country,const long age, Date * entryDate, Date * exitDate)
{
    PatientInfo * info;
    int char_counter = 0;

    if( (info = malloc(sizeof(*info))) == NULL)
    {
        perror("Error(PatientInfo_Init): ");
        exit(EXIT_FAILURE);
    }

    // recordID
    char_counter = strlen(recordID) + 1;
    if( (info -> recordID = malloc(1 + sizeof(char) * char_counter)) == NULL)
    {
        perror("Error(PatientInfo_Init): ");
        exit(EXIT_FAILURE);
    }
    strcpy(info -> recordID,recordID);
    // info -> recordID[char_counter] = '\0';


    // patientFirstName
    char_counter = strlen(patientFirstName) + 1;
    if( (info -> patientFirstName = malloc(1 + sizeof(char) * char_counter)) == NULL)
    {
        perror("Error(PatientInfo_Init): ");
        exit(EXIT_FAILURE);
    }
    strcpy(info -> patientFirstName,patientFirstName);
    // info -> patientFirstName[char_counter] = '\0';


    // patientLastName
    char_counter = strlen(patientLastName) + 1;
    if( (info -> patientLastName = malloc(1 + sizeof(char) * char_counter)) == NULL)
    {
        perror("Error(PatientInfo_Init): ");
        exit(EXIT_FAILURE);
    }
    strcpy(info -> patientLastName,patientLastName);
    // info -> patientLastName[char_counter] = '\0';


    // diseaseID
    char_counter = strlen(diseaseID) + 1;
    if( (info -> diseaseID = malloc(1 + sizeof(char) * char_counter)) == NULL)
    {
        perror("Error(PatientInfo_Init): ");
        exit(EXIT_FAILURE);
    }
    strcpy(info -> diseaseID,diseaseID);
    // info -> diseaseID[char_counter] = '\0';

    // country
    char_counter = strlen(country) + 1;
    if( (info -> country = malloc(1 + sizeof(char) * char_counter)) == NULL)
    {
        perror("Error(PatientInfo_Init): ");
        exit(EXIT_FAILURE);
    }
    strcpy(info -> country,country);
    // info -> country[char_counter] = '\0';

    // age
    info -> age = age;

    // entryDate
    if( (info -> entryDate = malloc(sizeof(* entryDate))) == NULL)
    {
        perror("Error(PatientInfo): ");
        exit(EXIT_FAILURE);
    }
    info -> entryDate -> day = entryDate -> day;
    info -> entryDate -> month = entryDate -> month;
    info -> entryDate -> year = entryDate -> year;


    // Date
    if((info -> exitDate = malloc(sizeof(* exitDate))) == NULL)
    {
        perror("Error(PatientInfo): ");
        exit(EXIT_FAILURE);
    }
    info -> exitDate -> day = exitDate -> day;
    info -> exitDate -> month = exitDate -> month;
    info -> exitDate -> year = exitDate -> year;

    return info;
}


void PatientInfo_Print(const PatientInfo * info)
{
    if(info -> exitDate -> day == TAG)
    {
        printf("%s %s %s %s %s %ld %ld-%ld-%ld -\n",info -> recordID, info -> patientFirstName, info -> patientLastName,info -> diseaseID, info -> country, info -> age, info -> entryDate -> day, info -> entryDate -> month, info -> entryDate -> year);
    }
    else
    {
        printf("%s %s %s %s %s %ld %ld-%ld-%ld %ld-%ld-%ld\n",info -> recordID, info -> patientFirstName, info -> patientLastName,info -> diseaseID, info -> country, info -> age, info -> entryDate -> day, info -> entryDate -> month, info -> entryDate -> year, info -> exitDate -> day, info -> exitDate -> month, info -> exitDate -> year);
    }
}

void PatientInfo_Deallocate(PatientInfo ** info)
{
    free((*info) -> recordID);
    free((*info) -> patientFirstName);
    free((*info) -> patientLastName);
    free((*info) -> diseaseID);
    free((*info) -> country);
    free((*info) -> entryDate);
    free((*info) -> exitDate);

    free(*info);
    *info = NULL;
}

Date * PatientInfo_EntryDate(const PatientInfo * info)
{
    return info -> entryDate;
}

Date * PatientInfo_ExitDate(const PatientInfo * info)
{
    return info -> exitDate;
}

// ----------------------------- PATIENT -----------------------------
static Patient * Patient_Init(size_t size)
{
    size_t i = 0;
    Patient * patient = malloc(sizeof(*patient));

    if(patient == NULL)
    {
        perror("Error(Patient): ");
        exit(EXIT_FAILURE);
    }
    patient -> length = 0;
    patient -> size = size;
    patient -> next = NULL;
    patient -> info = malloc(sizeof(*patient -> info) * size);

    if(patient -> info == NULL)
    {
        perror("Error(Patient): ");
        exit(EXIT_FAILURE);
    }
    while(i < size)
    {
        patient -> info[i] = NULL;
        i++;
    }
    return patient;
}

static void Patient_Insert(Patient * patient, PatientInfo * info)
{
    if( (patient -> length) < (patient -> size) )
    {
        patient -> info[patient -> length] = info;
        ++patient->length;
    }
    else if(patient -> next != NULL)
    {

        Patient_Insert(patient->next, info);
    }
    else
    {
        patient -> next = Patient_Init(patient -> size);
        Patient_Insert(patient -> next, info);
    }
}


static void Patient_Print(const Patient * patient)
{
    size_t i = 0;
    if( patient == NULL)
    {
        return;
    }
    while(i < patient -> length)
    {

        PatientInfo_Print(patient->info[i]);
        i++;
    }

    Patient_Print(patient -> next);
}


static void Patient_Deallocate(Patient ** patient,bool remove)
{
    size_t i = 0;
    if(patient != NULL)
    {
        if(remove == TRUE)
        {
            while(i < (*patient) -> length)
            {
                PatientInfo_Deallocate(&(*patient) -> info[i]);
                i++;
            }
        }
        free((*patient) -> info);

    }
    free(*patient);
    *patient = NULL;
}

static PatientInfo * Patient_Find_Patient(const Patient * patient, const char * recordID)
{
    size_t i = 0;

    if(patient == NULL)
    {
        return 0;
    }
    while(i < patient -> length)
    {

        if(!strcmp(patient -> info[i] -> recordID,recordID))
        {
            return patient -> info[i];
        }
        i++;
    }
    if(patient -> next != NULL)
    {
        Patient_Find_Patient(patient -> next,recordID);
    }
    // return NULL;

}

static long Patient_getPatientsInThatPeriod(const Patient * patient, const char * diseaseID, Date * date1, Date * date2, const char * country, long flag)
{
    size_t i = 0;

    if(patient == NULL)
    {
        return 0;
    }
    while(i < patient -> length)
    {

        if(!flag)
        {
            if(!strcmp(patient -> info[i] -> diseaseID,diseaseID))
            {
                if( (Compare_Date(patient -> info[i] -> entryDate, date1) != -1) && (Compare_Date(patient -> info[i] -> entryDate, date2) != 1) )
                {
                    tResult++;
                }
            }
        }
        else
        {
            if(!strcmp(patient -> info[i] -> diseaseID,diseaseID))
            {
                if(!strcmp(patient -> info[i] -> country,country))
                {
                    if( (Compare_Date(patient -> info[i] -> entryDate, date1) != -1) && (Compare_Date(patient -> info[i] -> entryDate, date2) != 1) )
                    {
                        tResult++;
                    }
                }
            }
        }



        i++;
    }
    if(patient -> next != NULL)
    {
        Patient_getPatientsInThatPeriod(patient -> next,diseaseID,date1,date2,country,flag);
    }
    return tResult;

}


static long Patient_getExitPatientsInThatPeriod(const Patient * patient, const char * diseaseID, Date * date1, Date * date2, const char * country, long flag)
{
    size_t i = 0;

    if(patient == NULL)
    {
        return 0;
    }
    while(i < patient -> length)
    {

        if(!flag)
        {
            if(!strcmp(patient -> info[i] -> diseaseID,diseaseID))
            {
                if(patient -> info[i] -> exitDate -> day != TAG)
                {
                    if( (Compare_Date(patient -> info[i] -> entryDate, date1) != -1) && (Compare_Date(patient -> info[i] -> entryDate, date2) != 1) )
                    {
                        tResult++;
                    }
                }

            }
        }
        else
        {
            if(!strcmp(patient -> info[i] -> diseaseID,diseaseID))
            {
                if(!strcmp(patient -> info[i] -> country,country))
                {
                    if(patient -> info[i] -> exitDate -> day != TAG)
                    {
                        if( (Compare_Date(patient -> info[i] -> entryDate, date1) != -1) && (Compare_Date(patient -> info[i] -> entryDate, date2) != 1) )
                        {
                            tResult++;
                        }
                    }
                }
            }
        }



        i++;
    }
    if(patient -> next != NULL)
    {
        Patient_getExitPatientsInThatPeriod(patient -> next,diseaseID,date1,date2,country,flag);
    }
    return tResult;

}



static void Patient_getStatisticsPatientsInThatPeriod(const Patient * patient, const char * diseaseID, Date * date1, Date * date2, const char * country, SumStatistics * statistics)
{
    size_t i = 0;

    if(patient == NULL)
    {
        return;
    }
    while(i < patient -> length)
    {

        if(!strcmp(patient -> info[i] -> diseaseID,diseaseID))
        {
            if(!strcmp(patient -> info[i] -> country,country))
            {
                if( (Compare_Date(patient -> info[i] -> entryDate, date1) != -1) && (Compare_Date(patient -> info[i] -> entryDate, date2) != 1) )
                {
                    if(patient -> info[i] -> age < 20)
                    {
                        statistics -> cases_0_20++;
                    }
                    else if (patient -> info[i] -> age < 40)
                    {
                        statistics -> cases_21_40++;
                    }
                    else if(patient -> info[i] -> age < 60)
                    {
                        statistics -> cases_41_60++;
                    }
                    else
                    {
                        statistics -> cases_over_60++;
                    }
                }
            }
        }

        i++;
    }
    if(patient -> next != NULL)
    {
        Patient_getStatisticsPatientsInThatPeriod(patient -> next,diseaseID,date1,date2,country,statistics);
    }
    return;

}
// ----------------------------- BUCKET NODE -----------------------------
static BucketNode * BucketNode_Init(long long number, size_t size)
{
    BucketNode * bucketnode = malloc(sizeof(*bucketnode));

    if(bucketnode == NULL)
    {
        perror("Error(Patient_BucketNode_Init): ");
        exit(EXIT_FAILURE);
    }

    bucketnode -> number = number;
    bucketnode -> size = size;
    bucketnode -> head = NULL;

    return bucketnode;
}

static void BucketNode_Insert(BucketNode * bucketnode, PatientInfo * info)
{
    if(bucketnode -> head == NULL)
    {
        bucketnode -> head = Patient_Init(bucketnode -> size);
    }
    Patient_Insert(bucketnode -> head, info);

}


static void BucketNode_Deallocate(BucketNode ** bucketnode,bool remove)
{
    if(bucketnode != NULL)
    {
        Patient * patient = (*bucketnode) -> head;
        while(patient != NULL)
        {
            Patient * tempPatient = patient;
            patient = patient -> next;
            Patient_Deallocate(&tempPatient,remove);
        }
    }
    free(*bucketnode);
    *bucketnode = NULL;
}

// ----------------------------- BUCKET -----------------------------
static Bucket * Bucket_Init(size_t bucketSize)
{
    size_t i = 0;
    Bucket * bucket;
    bucket = malloc(sizeof(*bucket));
    if(bucket == NULL)
    {
        perror("Error(Bucket_Init): ");
        exit(EXIT_FAILURE);
    }

    bucket -> length  = 0;
    bucket -> size = bucketSize;
    bucket -> nodes = malloc(sizeof(*bucket -> nodes) * bucketSize);
    bucket -> next = NULL;

    if(bucket -> nodes == NULL)
    {
        perror("Error(Bucket_Init): ");
        exit(EXIT_FAILURE);
    }

    while(i < bucketSize)    // Init every pointer to null
    {
        bucket -> nodes[i] = NULL;
        i++;
    }

    return bucket;
}

static void Bucket_Insert(Bucket * bucket, long long number, PatientInfo * info)
{
    size_t i = 0;
    while(i < bucket -> length)
    {
        if(bucket -> nodes[i] -> number == number) // If already exists
        {
            BucketNode_Insert(bucket -> nodes[i], info);
            return;
        }
        i++;
    }
    if(bucket -> next != NULL) // Go to last bucket
    {
        Bucket_Insert(bucket -> next, number , info);
    }
    else if(bucket -> length < bucket -> size)  // If the number that can hold this bucket is < of the total number of nodes that can hold then
    {
        bucket -> nodes[bucket->length] = BucketNode_Init(number, bucket -> size);
        BucketNode_Insert(bucket -> nodes[bucket -> length], info);
        ++bucket->length;

    }
    else // That means that we are at the end of buckets and we are going to create a new bucket because the last entry can't be put in previous last bucket
    {

        bucket -> next = Bucket_Init(bucket -> size);
        Bucket_Insert(bucket -> next, number, info);
    }

}


static void Bucket_Print(const Bucket * bucket)
{
    size_t i = 0;
    if(bucket == NULL)
    {
        return;
    }
    while(i < bucket -> length)
    {
        Patient_Print(bucket -> nodes[i] -> head);

        i++;
    }
    Bucket_Print(bucket -> next);
}

static void Bucket_Deallocate(Bucket ** bucket, bool remove)
{
    size_t i = 0;
    if(bucket != NULL)
    {
        while(i < (*bucket)->length)
        {
            BucketNode_Deallocate(&(*bucket)->nodes[i],remove);
            i++;
        }
        free((*bucket)->nodes);
    }
    free(*bucket);
    *bucket = NULL;
}

static PatientInfo * Bucket_Find_Patient(const Bucket * bucket,long long number,const char * recordID)
{
    size_t i = 0;

    if(bucket == NULL)
    {
        return 0;
    }
    while(i < bucket -> length)
    {

        if(bucket -> nodes[i] -> number == number)
        {
            return Patient_Find_Patient(bucket -> nodes[i] -> head,recordID);
        }
        i++;
    }
    if(bucket -> next != NULL)
    {
        Bucket_Find_Patient(bucket -> next, number,recordID);
    }
    // return NULL;

}


static long Bucket_getPatientsInThatPeriod(const Bucket * bucket,long long number, const char * diseaseID, Date * date1, Date * date2, const char * country, long flag)
{
    size_t i = 0;

    if(bucket == NULL)
    {
        return 0;
    }
    while(i < bucket -> length)
    {

        if(bucket -> nodes[i] -> number == number)
        {
            return Patient_getPatientsInThatPeriod(bucket -> nodes[i] -> head,diseaseID, date1, date2, country, flag);
        }
        i++;
    }
    if(bucket -> next != NULL)
    {
        Bucket_getPatientsInThatPeriod(bucket -> next, number,diseaseID, date1, date2, country, flag);
    }
    return tResult;

}


static long Bucket_getExitPatientsInThatPeriod(const Bucket * bucket,long long number, const char * diseaseID, Date * date1, Date * date2, const char * country, long flag)
{
    size_t i = 0;

    if(bucket == NULL)
    {
        return 0;
    }
    while(i < bucket -> length)
    {

        if(bucket -> nodes[i] -> number == number)
        {
            return Patient_getExitPatientsInThatPeriod(bucket -> nodes[i] -> head,diseaseID, date1, date2, country, flag);
        }
        i++;
    }
    if(bucket -> next != NULL)
    {
        Bucket_getExitPatientsInThatPeriod(bucket -> next, number,diseaseID, date1, date2, country, flag);
    }
    return tResult;

}

static void Bucket_getStatisticsPatientsInThatPeriod(const Bucket * bucket,long long number, const char * diseaseID, Date * date1, Date * date2, const char * country, SumStatistics * statistics)
{
    size_t i = 0;

    if(bucket == NULL)
    {
        return;
    }
    while(i < bucket -> length)
    {

        if(bucket -> nodes[i] -> number == number)
        {
            return Patient_getStatisticsPatientsInThatPeriod(bucket -> nodes[i] -> head,diseaseID, date1, date2, country, statistics);
        }
        i++;
    }
    if(bucket -> next != NULL)
    {
        Bucket_getStatisticsPatientsInThatPeriod(bucket -> next, number,diseaseID, date1, date2, country, statistics);
    }
    return;

}
// ----------------------------- HASH -----------------------------------------

Hash * Hash_Init(size_t hashSize, size_t bucketSize)
{
    size_t i = 0;                                        // init a counter for buckettable

    size_t tempBucketSize;                               // That variable holds the number of BucketNodes
    tempBucketSize = bucketSize / sizeof(BucketNode);    // The bytes that I took from user div sizeof Bucket

    Hash * ht;                                           // A pointer to Hash struct
    ht = (Hash *)malloc(sizeof(*ht));

    if(ht == NULL)                                       // Check for error
    {
        perror("Error(Hash_Init): ");
        exit(EXIT_FAILURE);
    }

    ht -> hashSize = hashSize;                                          // Init Hash size
    ht -> bucketSize = tempBucketSize;                                  // Init Bucket size
    ht -> bucketTable = malloc(sizeof(*ht -> bucketTable) * hashSize);  // Create space for hashSize pointers to BucketNodes

    if(ht -> bucketTable == NULL)                       // Check for error
    {
        perror("Error(Hash_Init): ");
        exit(EXIT_FAILURE);
    }

    while(i < hashSize)                                 // Init every pointer to null
    {
        ht -> bucketTable[i] = NULL;
        i++;
    }
    // printf("\nHash_Init has been completed successfully!\n\n");        // Feedback

    return ht;                                          // Return pointer
}



void Hash_Insert(Hash * ht, long long number, PatientInfo * info)
{

    size_t position = number % ht -> hashSize;

    if(ht -> bucketTable[position] == NULL)
    {
        ht -> bucketTable[position] = Bucket_Init(ht -> bucketSize);
    }
    Bucket_Insert(ht -> bucketTable[position], number, info);
}


void Hash_Print(const Hash * ht)
{
    size_t i = 0;
    while(i < ht -> hashSize)
    {
        printf("Position %zu:\n", i);
        Bucket_Print(ht -> bucketTable[i]);
        printf("\n");
        i++;
    }
}


void Hash_Deallocate(Hash ** ht,bool remove)
{
    size_t i = 0;
    while(i < (*ht) -> hashSize)
    {
        Bucket * bucket = (*ht) -> bucketTable[i];
        while(bucket != NULL)
        {
            Bucket * tempBucket = bucket;
            bucket = bucket -> next;
            Bucket_Deallocate(&tempBucket,remove);
        }
        i++;
    }
    free((*ht) -> bucketTable);
    free(*ht);
    *ht = NULL;
}


PatientInfo * Hash_Find_Patient(Hash * ht,long long number, const char * recordID)
{

    size_t position = number % ht -> hashSize;
    return Bucket_Find_Patient(ht -> bucketTable[position],number,recordID);
}


long Hash_getPatientsInThatPeriod(Hash * ht,long long number, const char * diseaseID, Date * date1, Date * date2, const char * country, long flag)
{
    size_t position = number % ht -> hashSize;
    return Bucket_getPatientsInThatPeriod(ht -> bucketTable[position],number,diseaseID, date1, date2, country, flag);

}


long Hash_getExitPatientsInThatPeriod(Hash * ht,long long number, const char * diseaseID, Date * date1, Date * date2, const char * country, long flag)
{
    size_t position = number % ht -> hashSize;
    return Bucket_getExitPatientsInThatPeriod(ht -> bucketTable[position],number,diseaseID, date1, date2, country, flag);

}

void Hash_getStatisticsPatientsInThatPeriod(Hash * ht,long long number, const char * diseaseID, Date * date1, Date * date2, const char * country, SumStatistics * statistics)
{
    size_t position = number % ht -> hashSize;
    return Bucket_getStatisticsPatientsInThatPeriod(ht -> bucketTable[position],number,diseaseID, date1, date2, country, statistics);
}
