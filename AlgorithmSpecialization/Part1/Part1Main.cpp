
#include "Lesson1/MergeSort.h"
#include "Lesson2/InversionCount.h"
#include "Lesson2/MutipleMatrix.h"
#include "Lesson2/closerpair.h"
#include "Lesson1/MutipleInteger.h"
#include "Lesson3/quicksort.h"
#include "Lesson4/RandomizedSelection.h"
#include "Lesson4/MinCutByRandomizedContraction.h"

void testLesson1();
void testLesson2();
void testLesson3();
void testLesson4();



int main()
{
    testLesson1();
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

    // Bad peformance becuase copy data. Need to improve in future.
    testMutipleMatrix();
}

void testLesson3()
{
    testQuickSort();
}

void testLesson4()
{
    TestMinCut();
}

