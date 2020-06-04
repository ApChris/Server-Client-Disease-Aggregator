#ifndef __MYVECTOR_H__
#define __MYVECTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 20
#define REDOUBLE 2

typedef struct myVector
{
    long length;
    long size;
    void ** items;
}myVector;

myVector * Init_MyVector();
void PushBack_MyVector(myVector * vec, void * item);
long Size_MyVector(myVector * vec);
long Length_MyVector(myVector * vec);
void * GetItem_MyVector(myVector * vec, long position);
void UpdateItem_MyVector(myVector * vec, void * newItem, long position);
void Deallocate_MyVector(myVector * vec);

#endif
