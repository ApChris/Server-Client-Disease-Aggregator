
#include "../include/myVector.h"

long queryPortNum;
long statisticsPortNum;
long numThreads;
long bufferSize;


int main(int argc, char const *argv[])
{
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
      }
      else if(!strcmp(argv[i],"-s"))    // Get statisticsPortNum
      {
         statisticsPortNum = atol(argv[i+1]);
      }
      else if(!strcmp(argv[i],"-w"))    // Get numThreads
      {
         numThreads = atol(argv[i+1]);
      }
      else if(!strcmp(argv[i],"-b"))    // Get buffer Size
      {
         bufferSize = atol(argv[i+1]);
      }

    }

    myVector * vector = NULL;
    vector = Init_MyVector(vector);;


    PushBack_MyVector(vector, "Hello");


    printf("%ld\n",vector -> size);
    for (long i = 0; i < 20; i++)
    {
        // printf("Hello\n");
        UpdateItem_MyVector(vector,"Bye",i);
        // printf("%s\n", (char *)vector -> items[i]);
    }

    Deallocate_MyVector(vector);
    return 0;
}
