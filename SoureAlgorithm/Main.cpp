#include "utilities.h"

#include "Lesson1/MergeSort.h"
#include "Lesson2/InversionCount.h"
#include "Lesson2/MutipleMatrix.h"
#include "Lesson2/closerpair.h"
#include "Lesson1/MutipleInteger.h"
#include "Lesson3/quicksort.h"
#include <random>
#include "Lesson4/RandomizedSelection.h"

void testLesson1();
void testLesson2();
void testLesson4();


int main()
{
    //testClosestPair();

    //testQuickSort();
    testRSelection();
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

void testLesson4()
{
    testRSelection();
}

