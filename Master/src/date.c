/*
	File Name 	: date .c
	Author 		: Christoforos Apostolopoulos
	Project 	: Disease-Monitor , System Programming 2019-2020
*/

#include "../include/date.h"


long Compare_Date(Date * date,Date * dateArg)
{
    // return 0 -> date == dateArg
    // return 1 -> date > dateArg
    // return -1 -> date < dateArg
    if(dateArg -> year > date -> year)
    {
        return -1;
    }
    else if(dateArg -> year < date -> year)
    {
        return 1;
    }
    else // dateArg -> year is == YEAR
    {
        if(dateArg -> month > date -> month)
        {
            return -1;
        }
        else if(dateArg -> month < date -> month)
        {
            return 1;
        }
        else // year & month is the same
        {
            if(dateArg -> day > date -> day)
            {
                return -1;
            }
            else if(dateArg -> day < date -> day)
            {
                return 1;
            }
            else // year/month/day is the same
            {
                return 0;
            }
        }
    }
    return 0;
}
//
// long Compare_Date(Date * date,Date * dateArg)
// {
//     if(dateArg -> year > date -> year)
//     {
//         printf("\n1Wrong date/time input!!!\n");
//         return -1;
//     }
//     else if(dateArg -> year < date -> year)
//     {
//         printf("\nThe date & time from stdin is fine!\n");
//         return 0;
//     }
//     else // dateArg -> year is == YEAR
//     {
//         if(dateArg -> month > date -> month)
//         {
//             printf("\n2Wrong date/time input!!!\n");
//             return -1;
//         }
//         else if(dateArg -> month < date -> month)
//         {
//             printf("\nThe date & time from stdin is fine!\n");
//             return 0;
//         }
//         else // year & month is the same
//         {
//             if(dateArg -> day > date -> day)
//             {
//                 printf("\n3Wrong date/time input!!!\n");
//                 return -1;
//             }
//             else if(dateArg -> day < date -> day)
//             {
//                 printf("\nThe date & time from stdin is fine!\n");
//                 return 0;
//             }
//             else // year/month/day is the same
//             {
//                 printf("\nThe date & time is exactly the same!!\n");
//             }
//         }
//     }
//     return 0;
// }
