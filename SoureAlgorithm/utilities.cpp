#include "utilities.h"


void writeToFile(std::vector<Point> &arr)
{
    ofstream file;
    file.open("ListPoint.txt");

    file<<"[";
    for(int i = 0; i < arr.size(); i++)
    {
        file<<"["<<arr[i].x<<","<<arr[i].y<<"]; ";
    }
     file<<"]"<<endl;

}

vector<int> readDataFromFile(string path)
{
    vector<int> result;
    fstream file(path.c_str(),ios_base::in);

    int value;
    while (file>>value) {
        result.push_back(value);
    }

    //getchar();

    return result;
}
void ShowdArray(int*arr, int size, string msg)
{
    cout<<msg;
    for(int i = 0; i < size && i <100; i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}

void ShowdArray(vector<int>arr, int size)
{
    for(int i = 0; i < arr.size() && i <100; i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}

void showMatrix(vector<vector<int>> matrix)
{
    for(int i = 0; i < matrix.size() && i < 10; i++)
    {
        for(int j = 0; j < matrix[0].size() && j < 10; j++)
        {
            cout<<matrix[i][j]<<" ";
        }
         cout<<endl;
    }
}

void showMatrix(int** matrix, int Row, int Col)
{
    for(int i = 0; i < Row && i < 10; i++)
    {
        for(int j = 0; j < Col && j < 10; j++)
        {
            cout<<matrix[i][j]<<" ";
        }
         cout<<endl;
    }
}


int randValue(int range)
{
    return std::rand()%range;
}

bool checkvalue(int* arr, int size, int value)
{
    for(int i = 0; i < size ; i++)
    {
        if(arr[i] == value)
            return false;
    }
    return  true;
}


vector<vector<int>> randomMatrix(int row, int col)
{
    vector<vector<int>> result(row, vector<int>(col, 0));

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            result[i][j] = std::rand()%5;
        }
    }
    return result;
}

