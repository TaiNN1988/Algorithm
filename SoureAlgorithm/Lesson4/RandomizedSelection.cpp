#include "Lesson4/RandomizedSelection.h"
#include "utilities.h"
#include <random>
#include <exception>

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
        }
    }
    swapValue(arr[l], arr[i-1]);

    return i - 1;
}

template<typename T1>
int partitionRandom(T1* arr, int l, int r)
{
    int range_from = l;
    int range_to = r;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int> distr(range_from, range_to);
    int pivotIdxRandom = distr(generator);

    swapValue(arr[l], arr[pivotIdxRandom]);

    return partitionFirstPivot(arr, l, r);
}

template<typename T2>
T2 RandomizedSelection(T2* arr, int l, int r, int idxOrderStatistic)
{
    if( l == r)
    {
        return arr[l];
    }
    // Choose fisrt element of array as Pivot
    int pivotIdx = partitionRandom(arr, l, r);
    int k = pivotIdx - l + 1;
    int k1 = pivotIdx + 1;
    std::cout<<"pivotIdx = "<<pivotIdx<<"k = " <<k<<endl;
    cout<<"K1= "<<k1<<endl;

    if(idxOrderStatistic == k)
    {
        return arr[pivotIdx];
    }
    else if (idxOrderStatistic < k) {
         // First part
        return RandomizedSelection(arr, l, pivotIdx -1, idxOrderStatistic);

    }

    // Second part
    return RandomizedSelection(arr, pivotIdx + 1, r, idxOrderStatistic - k);

}

void testRSelection()
{

    vector<int> arr1  = {10, 7, 8, 9, 1, 5, 2};
    vector<int> arr2(arr1.begin(), arr1.end());
    int size = arr1.size();
    int orderStatistic = 5;
    // Show input
    ShowdArray(arr1, arr1.size());

    int value = RandomizedSelection(arr1.data(), 0, size - 1, orderStatistic);

     // Show output sort
    std::sort(arr2.begin(), arr2.end());

     ShowdArray(arr2, arr2.size());

    cout<<"OrderStatistic = "<<orderStatistic<<"    value = "<<value<<endl;
}
