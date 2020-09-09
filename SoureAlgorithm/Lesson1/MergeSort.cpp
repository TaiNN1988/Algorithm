////////////////////////////////////////////////////////////////////////////
//
//   Project     : Merge Sort
//   Description :
//      Implement merge sort
//
//   Version:
//         1.0(8/2020)
//   Created By: Tai Nguyen(taingocnguyen1988@gmail.com)
////////////////////////////////////////////////////////////////////////////

#include<cstdlib>
#include "MergeSort.h"
#include <ostream>

#include "utilities.h"

#define MAX_SZIE 50

void mergeSubLeftRight(std::vector<int> &result, std::vector<int> &subLeft,  std::vector<int> &subRight)
{
    int sizeLeft = subLeft.size();
    int sizeRight = subRight.size();

    int i = 0, j = 0;
    for(int k = 0; k < result.size(); ++k)
    {
        if(i < sizeLeft && j < sizeRight)
        {
            if(subLeft[i] < subRight[j])
            {
                result[k] = subLeft[i];
                i++;
            }
            else if (subLeft[i] > subRight[j]) {
                result[k] = subRight[j];
                j++;
            }
            else {
                result[k] = subRight[j];
                result[k + 1] = subLeft[i];
                k++;
                i++;
                j++;
            }
        }
        else if (i < sizeLeft && j >= sizeRight) {
             result[k] = subLeft[i];
            i++;
        }
        else if (j < sizeRight && i >= sizeLeft) {
             result[k] = subRight[j];
            j++;
        }
    }
}

void swap1(int& a, int & b)
{
    int temp = a;
    a = b;
    b = temp;
}

std::vector<int> mergeSort(int* array, int size)
{
    std::vector<int> result = std::vector<int>(size, 0);

    if(size <= 2)
    {
        if (size == 1)
        {
            result[0] = array[0];
        }

        if(size == 2)
        {
            if(array[0] < array[1])
            {
                result[0] = array[0];
                result[1] = array[1];
            }
            else
            {
                result[0] = array[1];
                result[1] = array[0];
            }

        }

         return  result;
    }

    int sizeLeft = size/2;
    int sizeRight = (size + 1)/2;
    int* leftInput = array;
    int* rightInput = &array[size/2];
    std::vector<int> subLeft = mergeSort(leftInput, sizeLeft);
    std::vector<int> subRight = mergeSort(rightInput, sizeRight);

    // Merge two sup array
   mergeSubLeftRight(result, subLeft, subRight);

    return  result;
}


void mergeSubLeftRight1(int* arr, int* subLeft,  int sizeLeft, int* subRight, int sizeRight)
{
    int size = sizeLeft + sizeRight;
    // Copy data of array to two part
    std::vector<int> left = std::vector<int>(sizeLeft,0);
    for(int i = 0; i < sizeLeft; i++)
    {
        left[i] = subLeft[i];
    }

    std::vector<int> right = std::vector<int>(sizeRight,0);
    for(int i = 0; i < sizeRight; i++)
    {
        right[i] = subRight[i];
    }

    int i = 0, j = 0;
    for(int k = 0; k < size; ++k)
    {
        if(i < sizeLeft && j < sizeRight)
        {
            if(left[i] < right[j])
            {
                arr[k] = left[i];
                i++;
            }
            else if (left[i] > right[j]) {
                arr[k] = right[j];
                j++;
            }
            else {
                arr[k] = right[j];
                arr[k + 1] = left[i];
                k++;
                i++;
                j++;
            }
        }
        else if (i < sizeLeft && j >= sizeRight) {
             arr[k] = left[i];
            i++;
        }
        else if (j < sizeRight && i >= sizeLeft) {
             arr[k] = right[j];
            j++;
        }
    }
}

void mergeSortOptimize(int* arr, int size)
{
    if (size <= 1)
        return;

    int sizeLeft = size/2;
    int sizeRight = (size + 1)/2;
    int* leftInput = arr;
    int* rightInput = &arr[size/2];
    mergeSortOptimize(leftInput, sizeLeft);
    mergeSortOptimize(rightInput, sizeRight);

    // Merge two sup array
   mergeSubLeftRight1(arr, leftInput, sizeLeft, rightInput, sizeRight);
}

void testMergeSort()
{
    int arr[MAX_SZIE];
    vector<int> input1;

    for(int i = 0; i <MAX_SZIE; i++ )
    {
        arr[i] = std::rand()%MAX_SZIE;
        input1.push_back(arr[i]);
    }

    cout<<"Input as: ";
    ShowdArray(arr, MAX_SZIE);
    cout<<endl;

    //std::vector<int> result = mergeSort(arr, MAX_SZIE);
    mergeSortOptimize(arr, MAX_SZIE);

    std::sort(input1.begin(), input1.end());

    bool flas = true;

    cout<<"OutPut by merge sort  : ";
    ShowdArray(arr, MAX_SZIE);

    cout<<"OutPut by build in sort: ";
    ShowdArray(input1, MAX_SZIE);

    for(int i = 0; i < MAX_SZIE; i++)
    {
        if(input1[i] != arr[i])
        {
            flas = false;
            break;
        }
    }
    cout<<endl;

    if(flas)
    {
        cout<<"MergeSort PASS!"<<endl;
    }
    else
    {
        cout<<"MergeSort Fail!"<<endl;
    }
}
