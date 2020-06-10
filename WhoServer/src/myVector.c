#include "../include/myVector.h"


myVector * Init_MyVector()
{
    myVector * vec = (myVector *)malloc(sizeof(myVector));
    vec -> length = 0;
    vec -> items = (void **)malloc(sizeof(void *)* SIZE);
    vec -> size = SIZE;

    return vec;
}

long Size_MyVector(myVector * vec)
{
    return vec -> size;
}

long Length_MyVector(myVector * vec)
{
    return vec -> length;
}


void PushBack_MyVector(myVector * vec, void * item)
{
    if(Length_MyVector(vec) == Size_MyVector(vec))
    {
        vec -> size *= REDOUBLE;
        vec -> items = realloc(vec -> items, sizeof(void *) * vec -> size);
    }

    vec -> items[vec -> length] = item;
    vec -> length++;
}


void * GetItem_MyVector(myVector * vec, long position)
{
    // Check is position is out of bounds
    if(Length_MyVector(vec) > position)
    {
        return vec -> items[position];
    }


}

void Print_MyVector(myVector * vec, char type)
{
    if(type == 'd')
    {
        for (long i = 0; i < Length_MyVector(vec); i++)
        {
            printf("%ld\n", GetItem_MyVector(vec, i));
        }
    }
    else if(type == 's')
    {
        for (long i = 0; i < Length_MyVector(vec); i++)
        {
            printf("%s\n", GetItem_MyVector(vec, i));
        }
    }
    else if(type == 'c')
    {
        for (long i = 0; i < Length_MyVector(vec); i++)
        {
            printf("%c\n", GetItem_MyVector(vec, i));
        }
    }
    else if(type == 'f')
    {
        for (long i = 0; i < Length_MyVector(vec); i++)
        {
            printf("%lf\n", GetItem_MyVector(vec, i));
        }
    }
}

void UpdateItem_MyVector(myVector * vec, void * newItem, long position)
{
    // Check is position is out of bounds
    if(Length_MyVector(vec) > position)
    {
        // update item's value with new item
        vec -> items[position] = newItem;
    }

}

void Deallocate_MyVector(myVector * vec)
{
    free(vec -> items);
    free(vec);
}
