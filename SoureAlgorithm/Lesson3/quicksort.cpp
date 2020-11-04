#include "quicksort.h"
#include "utilities.h"
#include <random>
#include <exception>

int countCompare = 0;
template<typename T>
void swapValue(T& value1, T& value2)
{
    T temp = value1;
    value1 = value2;
    value2 = temp;
}

template<typename T1>
int partitionFirstPivot(T1* arr, int l, int r)
{
    T1 p = arr[l];
    int i = l + 1;
    for(int j = l + 1; j <= r; j++)
    {
        if(arr[j] < p)
        {
            swapValue(arr[j], arr[i]);
            i++;
            //countCompare++;
        }
    }
    swapValue(arr[l], arr[i-1]);

    return i - 1;
}

template<typename T1>
int partitionFinalPivot(T1* arr, int l, int r)
{
    swapValue(arr[l], arr[r]);
    T1 p = arr[l];
    int i = l + 1;
    for(int j = l + 1; j <= r; j++)
    {
        if(arr[j] < p)
        {
            swapValue(arr[j], arr[i]);
            i++;
            //countCompare++;
        }
    }
    swapValue(arr[l], arr[i-1]);

    return i - 1;
}

template<typename T1>
int partitionMedianOfThreePivot(T1* arr, int l, int r)
{
    int idx1 = l;
    int idx2 = l + (r - l)/2;
    int idx3 = r;
    if(arr[idx2] < arr[idx1])
    {
        swapValue(idx1, idx2);
    }

    if(arr[idx3] < arr[idx1])
    {
        swapValue(idx1, idx3);
    }

    if(arr[idx3] < arr[idx2])
    {
        swapValue(idx2, idx3);
    }

    int idxPivot = idx2;

    swapValue(arr[l], arr[idxPivot]);

    T1 p = arr[l];
    int i = l + 1;
    for(int j = l + 1; j <= r; j++)
    {
        if(arr[j] < p)
        {
            swapValue(arr[j], arr[i]);
            i++;
            //countCompare++;
        }
    }
    swapValue(arr[l], arr[i-1]);

    return i - 1;
}

//template<typename T1>
//int partitionFinalPivot(T1* arr, int l, int r)
//{
//    T1 p = arr[r];
//    int i = l;
//    for(int j = l; j <= r -1; j++)
//    {
//        if(arr[j] < p)
//        {
//            swapValue(arr[j], arr[i]);
//            i++;
//            //countCompare++;
//        }
//    }
//    swapValue(arr[r], arr[i]);

//    return i;
//}

template<typename T1>
int partitionRandom(T1* arr, int l, int r)
{
    // chose pivot random
//    srand(time(NULL));
//    int pivotIdxRandom = l + rand()%(r - l);

    int range_from = l;
    int range_to = r;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int> distr(range_from, range_to);
    int pivotIdxRandom = distr(generator);

    swapValue(arr[l], arr[pivotIdxRandom]);

    return partition(arr, l, r);
}

template<typename T2>
void quickSortRandom(T2* arr, int l, int r)
{
    if( l >= r)
    {
        return;
    }
    // Choose fisrt element of array as Pivot
    int pivotIdx = partitionRandom(arr, l, r);

    // First part
    quickSortRandom(arr, l, pivotIdx -1);

    // Second part
    quickSortRandom(arr,pivotIdx + 1, r);
}

template<typename T2>
void quickSortFirtPivot(T2* arr, int l, int r)
{
//    if(r -l <= 1)
//    {
//        return;
//    }

    if( l >= r)
    {
        return;
    }
    countCompare += r - l;

    // Choose fisrt element of array as Pivot
    int pivotIdx = partitionFirstPivot(arr, l, r);

    // First part
    //countCompare += pivotIdx -1 - l;
    quickSortFirtPivot(arr, l, pivotIdx -1);

    // Second part
     //countCompare += r - (pivotIdx +1);
    quickSortFirtPivot(arr,pivotIdx + 1, r);
}

template<typename T2>
void quickSortFinalPivot(T2* arr, int l, int r)
{
//    if(r -l <= 1)
//    {
//        return;
//    }

    if( l >= r)
    {
        return;
    }
    countCompare += r - l;

    // Choose fisrt element of array as Pivot
    int pivotIdx = partitionFinalPivot(arr, l, r);

    // First part
    //countCompare += pivotIdx -1 - l;
    quickSortFinalPivot(arr, l, pivotIdx -1);

    // Second part
     //countCompare += r - (pivotIdx +1);
    quickSortFinalPivot(arr,pivotIdx + 1, r);
}

template<typename T2>
void quickSortMedianOfThreePivot(T2* arr, int l, int r)
{
//    if(r -l <= 1)
//    {
//        return;
//    }

    if( l >= r)
    {
        return;
    }
    countCompare += r - l;

    // Choose fisrt element of array as Pivot
    int pivotIdx = partitionMedianOfThreePivot(arr, l, r);

    // First part
    //countCompare += pivotIdx -1 - l;
    quickSortMedianOfThreePivot(arr, l, pivotIdx -1);

    // Second part
     //countCompare += r - (pivotIdx +1);
    quickSortMedianOfThreePivot(arr,pivotIdx + 1, r);
}

void testQuickSort()
{
//    int arr[] = {10, 7, 8, 9, 1, 5};
//    int n = sizeof (arr)/sizeof (int);
//    ShowdArray(arr, n, "Input array\n");
//    quickSort<int>(arr, 0, n - 1);
//    ShowdArray(arr, n, "Ouput array\n");
    try {


    vector<int> arr1 = readDataFromFile("/home/tai/OnlineLearning/AlgorithmsSpecialization/Algorithm/SoureAlgorithm/DataTest/QuickSort.txt");
    vector<int> arr2(arr1.begin(), arr1.end());
    int size = arr1.size();
    countCompare = 0;
    //quickSortFirtPivot(arr1.data(), 0, size - 1);
    //quickSortFinalPivot(arr1.data(), 0, size - 1);
    quickSortMedianOfThreePivot(arr1.data(), 0, size - 1);


    std::sort(arr2.begin(), arr2.end());

    bool isCompare = compareArray<int>(arr1.data(), arr2.data(), size);

    if(isCompare != true)
    {
        cout<<"QuickSort is faild!"<<endl;
    }
    else {
        cout<<"QuickSort is passed!"<<endl;
    }

    cout<<"Count compare:"<<countCompare<<endl;

    }  catch (std::exception& e) {
        std::cout<<e.what()<<std::endl;
    }
}
