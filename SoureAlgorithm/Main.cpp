#include "utilities.h"

#include "Lesson1/MergeSort.h"
#include "Lesson2/InversionCount.h"
#include "Lesson2/MutipleMatrix.h"
#include "Lesson2/closerpair.h"
#include "Lesson1/MutipleInteger.h"
#include "Lesson3/quicksort.h"
#include <random>

void testLesson1();
void testLesson2();


int main()
{
    //testClosestPair();

    testQuickSort();
    return 0;
}

void testLesson1()
{
     testMutipleInteger();
     testMergeSort();
}

void testLesson2()
{
    testInversionCount();
    testClosestPair();
    testMutipleMatrix();
}


