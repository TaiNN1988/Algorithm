////////////////////////////////////////////////////////////////////////////
//
//   Project     : Inversion count
//   Description :
//      Implement Inversion count
//
//   Version:
//         1.0(8/2020)
//   Created By: Tai Nguyen(taingocnguyen1988@gmail.com)
////////////////////////////////////////////////////////////////////////////

#include<cstdlib>
#include "InversionCount.h"
#include <ostream>
#include "utilities.h"

unsigned int countSplitInv(vector<int>& inArr, vector<int>& subLeft, vector<int>& subRight);


// Inversion count O(n^2)
unsigned int inversionCount(vector<int> &arr, int size)
{
    unsigned int count = 0;
    for(int i = 0; i < size - 1; i++)
    {
        for(int j = i + 1; j < size; j++)
        {
            if(arr[j]< arr[i])
                count++;
        }
    }
    return count;
}

// Inversion count re-use merge sort O(nLog(n))
unsigned int countInversionsMergeSort(vector<int>& arr)
{
    if (arr.size() < 2) {
        return 0;
    }
    auto it = arr.begin();
    int m = arr.size()/2;
    vector<int> subLeft = vector<int>(it, it + m);
    vector<int> subRight = vector<int>(it + m, arr.end());

    unsigned int countLeft = countInversionsMergeSort(subLeft);
    unsigned int countRight = countInversionsMergeSort(subRight);

    unsigned int countSplit = countSplitInv(arr, subLeft, subRight);

    return countLeft + countRight + countSplit;
}

unsigned int countSplitInv(vector<int>& inArr, vector<int>& subLeft, vector<int>& subRight)
{
    unsigned int count = 0;
    int i = 0, j = 0;

    for (int k = 0; k < inArr.size(); k++) {

        if (i < subLeft.size() && j < subRight.size()) {
            if (subLeft[i] <= subRight[j]) {
                inArr[k] = subLeft[i];
                i++;
            }
            else if (subLeft[i] > subRight[j]) {
                inArr[k] = subRight[j];
                j++;
                count += subLeft.size() - i;
            }
        }
        else if (i < subLeft.size() && j >= subRight.size()) {
            inArr[k] = subLeft[i];
            i++;
        }
        else {
            inArr[k] = subRight[j];
            j++;
        }

    }
    return count;
}

void testInversionCount()
{
    vector<int> input1 = readDataFromFile("DataTest//IntegerArray.txt");
    vector<int> input2(input1.begin(), input1.end());

    int size = input1.size();

    auto started1 = chrono::high_resolution_clock::now();

    unsigned int expectedCount = inversionCount(input1, size);

    auto done1 = chrono::high_resolution_clock::now();
    double time1 = chrono::duration_cast<std::chrono::milliseconds>(done1-started1).count();
    cout <<"inversionCount O(n2) time(milliseconds): "<<time1 <<endl;
    cout<<"expected count = "<<expectedCount<<endl;

    auto started2 = chrono::high_resolution_clock::now();

    unsigned int actualCount = countInversionsMergeSort(input2);

    auto done2 = chrono::high_resolution_clock::now();
    double time2 = chrono::duration_cast<std::chrono::milliseconds>(done2-started2).count();
    cout <<"inversionCount O(nLog(n)) time(milliseconds): "<<time2 <<endl;
    cout<<"Actual count = "<<actualCount<<endl;
    double count = time1/time2;
    cout<<"Actual time O(nlogn) = "<<count<<" O(n2)"<<endl;

        if(actualCount == expectedCount)
        {
            cout<<"Inverstion count PASS!"<<endl;
        }
        else
        {
            cout<<"Inverstion count Fail!"<<endl;
        }
}

/*int mergeAndCountSplitInv(int* arr, int* subLeft,  int sizeLeft, int* subRight, int sizeRight)
{
    int count = 0;

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
    for(int k = 0; k < size; k++)
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
                count += sizeLeft - i;
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

    return count;
}

int inversionCountSort(int* arr, int size)
{
    if (size <= 1)
        return 0;

    int sizeLeft = size/2;
    int sizeRight = (size + 1)/2;
    int* leftInput = arr;
    int* rightInput = &arr[size/2];
    int leftCount = inversionCountSort(leftInput, sizeLeft);
    int rightCount = inversionCountSort(rightInput, sizeRight);

    // Merge two sup array
    int splitInversionCount = mergeAndCountSplitInv(arr, leftInput, sizeLeft, rightInput, sizeRight);

    return leftCount + rightCount + splitInversionCount;
}*/
