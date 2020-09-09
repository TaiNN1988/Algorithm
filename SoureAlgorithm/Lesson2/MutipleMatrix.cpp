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

#include "Lesson2/MutipleMatrix.h"
#include "utilities.h"

typedef struct
{
    vector<vector<int>> matrix1;
    vector<vector<int>> matrix2;
    vector<vector<int>> expectedMatrix;

}MatrixTest;


std::vector<std::vector<int>> mutipleMatrix(std::vector<std::vector<int>>& matrix1, std::vector<std::vector<int>>& matrix2)
{
    int row = matrix1.size();
    int colum = matrix2[0].size();
    int n = matrix1[0].size();
    std::vector<std::vector<int>> result(row,std::vector<int>(colum,0));

    int sum = 0;

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < colum; j++)
        {
            for(int k = 0; k < n; k++ )
            {
                sum += matrix1[i][k] * matrix2[k][j];
            }
            result[i][j] = sum;
            sum = 0;
        }
    }

    return  result;
}

std::vector<std::vector<int>> strassenMutipleMatrix1(std::vector<std::vector<int>>& matrix1, Position p1, std::vector<std::vector<int>>& matrix2, Position p2, int size)
{
     std::vector<std::vector<int>> result(size, std::vector<int>(size, 0));
    if(size == 2)
    { 
        result[0][0] = matrix1[p1.row][p1.col]*matrix2[p2.row][p2.col] + matrix1[p1.row][p1.col+1]*matrix2[p2.row+1][p2.col];
        result[0][1] = matrix1[p1.row][p1.col]*matrix2[p2.row][p2.col+1] + matrix1[p1.row][p1.col+1]*matrix2[p2.row+1][p2.col+1];
        
        result[1][0] = matrix1[p1.row+1][p1.col]*matrix2[p2.row][p2.col] + matrix1[p1.row+1][p1.col+1]*matrix2[p2.row+1][p2.col];
        result[1][1] = matrix1[p1.row+1][p1.col]*matrix2[p2.row][p2.col+1] + matrix1[p1.row+1][p1.col+1]*matrix2[p2.row+1][p2.col+1];
        
        return  result;
    }
    
    int haflSize = size/2;

    Position A = p1;
    Position B =  Position(p1.row,            p1.col + haflSize);
    Position C = Position(p1.row + haflSize,  p1.col);
    Position D = Position(p1.row + haflSize,  p1.col + haflSize);

    Position E = p2;
    Position F = Position(p2.row,             p2.col+haflSize);
    Position G = Position(p2.row+haflSize,    p2.col);
    Position H = Position(p2.row+haflSize,    p2.col + haflSize);
    
    std::vector<std::vector<int>> partAE = strassenMutipleMatrix1(matrix1, A, matrix2, E, haflSize);
    std::vector<std::vector<int>> partAF = strassenMutipleMatrix1(matrix1, A, matrix2, F, haflSize);

    std::vector<std::vector<int>> partBG = strassenMutipleMatrix1(matrix1, B, matrix2, G, haflSize);
    std::vector<std::vector<int>> partBH = strassenMutipleMatrix1(matrix1, B, matrix2, H, haflSize);

    std::vector<std::vector<int>> partCE = strassenMutipleMatrix1(matrix1, C, matrix2, E, haflSize);
    std::vector<std::vector<int>> partCF = strassenMutipleMatrix1(matrix1, C, matrix2, F, haflSize);

    std::vector<std::vector<int>> partDG = strassenMutipleMatrix1(matrix1, D, matrix2, G, haflSize);
    std::vector<std::vector<int>> partDH = strassenMutipleMatrix1(matrix1, D, matrix2, H, haflSize);

    for(int i = 0; i < haflSize; i++)
    {
        for(int j = 0; j < haflSize; j++)
        {

            result[i][j] = partAE[i][j] + partBG[i][j];

            result[i][j + haflSize] = partAF[i][j] + partBH[i][j];

            result[i + haflSize][j] = partCE[i][j] + partDG[i][j];

            result[i + haflSize][j + haflSize] = partCF[i][j] + partDH[i][j];

        }
    }

    return  result;
}

std::vector<std::vector<int>> addMatrix(std::vector<std::vector<int>>& matrix1, Position p1, std::vector<std::vector<int>>& matrix2, Position p2, int size)
{
    std::vector<std::vector<int>> result(size, std::vector<int>(size, 0));
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            result[i][j] = matrix1[i+p1.row][j+p1.col] + matrix2[i+p2.row][j+p2.col];
        }
    }

    return result;
}

std::vector<std::vector<int>> subMatrix(std::vector<std::vector<int>>& matrix1, Position p1, std::vector<std::vector<int>>& matrix2, Position p2, int size)
{
    std::vector<std::vector<int>> result(size, std::vector<int>(size, 0));
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            result[i][j] = matrix1[i+p1.row][j+p1.col] - matrix2[i+p2.row][j+p2.col];
        }
    }

    return result;
}

std::vector<std::vector<int>> strassenMutipleMatrix2(std::vector<std::vector<int>>& matrix1, Position p1, std::vector<std::vector<int>>& matrix2, Position p2, int size)
{
     std::vector<std::vector<int>> result(size, std::vector<int>(size));
    if(size == 2)
    {
        result[0][0] = matrix1[p1.row][p1.col]*matrix2[p2.row][p2.col] + matrix1[p1.row][p1.col+1]*matrix2[p2.row+1][p2.col];
        result[0][1] = matrix1[p1.row][p1.col]*matrix2[p2.row][p2.col+1] + matrix1[p1.row][p1.col+1]*matrix2[p2.row+1][p2.col+1];

        result[1][0] = matrix1[p1.row+1][p1.col]*matrix2[p2.row][p2.col] + matrix1[p1.row+1][p1.col+1]*matrix2[p2.row+1][p2.col];
        result[1][1] = matrix1[p1.row+1][p1.col]*matrix2[p2.row][p2.col+1] + matrix1[p1.row+1][p1.col+1]*matrix2[p2.row+1][p2.col+1];

        return  result;
    }

    int haflSize = size/2;
    Position A = p1;
    Position B =  Position(p1.row,            p1.col + haflSize);
    Position C = Position(p1.row + haflSize,  p1.col);
    Position D = Position(p1.row + haflSize,  p1.col + haflSize);

    Position E = p2;
    Position F = Position(p2.row,             p2.col+haflSize);
    Position G = Position(p2.row+haflSize,    p2.col);
    Position H = Position(p2.row+haflSize,    p2.col + haflSize);

    std::vector<std::vector<int>> subFH(haflSize, std::vector<int>(haflSize));
    std::vector<std::vector<int>> addAB(haflSize, std::vector<int>(haflSize));
    std::vector<std::vector<int>> addCD(haflSize, std::vector<int>(haflSize));
    std::vector<std::vector<int>> subGE(haflSize, std::vector<int>(haflSize));
    std::vector<std::vector<int>> addAD(haflSize, std::vector<int>(haflSize));
    std::vector<std::vector<int>> addEH(haflSize, std::vector<int>(haflSize));
    std::vector<std::vector<int>> subBD(haflSize, std::vector<int>(haflSize));
    std::vector<std::vector<int>> addGH(haflSize, std::vector<int>(haflSize));
    std::vector<std::vector<int>> subAC(haflSize, std::vector<int>(haflSize));
    std::vector<std::vector<int>> addEF(haflSize, std::vector<int>(haflSize));

    for(int i = 0; i < haflSize; i++)
    {
        for(int j = 0; j < haflSize; j++)
        {
            subFH[i][j] = matrix2[i + F.row][j + F.col] - matrix2[i + H.row][j + H.col];
            subGE[i][j] = matrix2[i + G.row][j + G.col] - matrix2[i + E.row][j + E.col];
            subBD[i][j] = matrix1[i + B.row][j + B.col] - matrix1[i + D.row][j + D.col];
            subAC[i][j] = matrix1[i + A.row][j + A.col] - matrix1[i + C.row][j + C.col];

            addAB[i][j] = matrix1[i + A.row][j + A.col] + matrix1[i + B.row][j + B.col];
            addCD[i][j] = matrix1[i + C.row][j + C.col] + matrix1[i + D.row][j + D.col];
            addAD[i][j] = matrix1[i + A.row][j + A.col] + matrix1[i + D.row][j + D.col];
            addEH[i][j] = matrix2[i + E.row][j + E.col] + matrix2[i + H.row][j + H.col];
            addGH[i][j] = matrix2[i + G.row][j + G.col] + matrix2[i + H.row][j + H.col];
            addEF[i][j] = matrix2[i + E.row][j + E.col] + matrix2[i + F.row][j + F.col];

        }
    }

    std::vector<std::vector<int>> part1 = strassenMutipleMatrix2(matrix1, A, subFH, Position(0,0), haflSize);
    std::vector<std::vector<int>> part2 = strassenMutipleMatrix2(addAB, Position(0,0), matrix2, H, haflSize);
    std::vector<std::vector<int>> part3 = strassenMutipleMatrix2(addCD, Position(0,0), matrix2, E, haflSize);
    std::vector<std::vector<int>> part4 = strassenMutipleMatrix2(matrix1, D, subGE, Position(0,0), haflSize);
    std::vector<std::vector<int>> part5 = strassenMutipleMatrix2(addAD, Position(0,0), addEH, Position(0,0), haflSize);
    std::vector<std::vector<int>> part6 = strassenMutipleMatrix2(subBD, Position(0,0), addGH, Position(0,0), haflSize);
    std::vector<std::vector<int>> part7 = strassenMutipleMatrix2(subAC, Position(0,0), addEF, Position(0,0), haflSize);


    for(int i = 0; i < haflSize; i++)
    {
        for(int j = 0; j < haflSize; j++)
        {
            result[i][j] =part5[i][j] + part4[i][j] - part2[i][j] + part6[i][j];

            result[i][j + haflSize] = part1[i][j] + part2[i][j];

            result[i + haflSize][j] = part3[i][j] + part4[i][j];

            result[i + haflSize][j + haflSize] = part1[i][j] + part5[i][j] - part3[i][j] - part7[i][j];
        }
    }

    return  result;
}

void testMutipleMatrix()
{
    MatrixTest datatest[] = {
        {{
            {1, 2, 3},
            {4, 5, 6}
        },

       {
           {7, 8},
           {9, 10},
           {11, 12}
       },

       {
           {58, 64},
           {139, 154}
       }},

        {{
            {5, 2, 6, 1},
            {0, 6, 2, 0},
            {3, 8, 1, 4},
            {1, 8, 5, 6},
        },

       {
            {7, 5, 8, 0},
            {1, 8, 2, 6},
            {9, 4, 3, 8},
            {5, 3, 7, 9},
       },

       {
             {96, 68, 69, 69},
             {24, 56, 18, 52},
             {58, 95, 71, 92},
             {90, 107, 81, 142},

       }},
    };

//    vector<vector<int>> matrix1 = datatest[1].matrix1;
//    vector<vector<int>> matrix2 = datatest[1].matrix2;
//    vector<vector<int>> expectedMatrix = datatest[1].expectedMatrix;
    int size = 8;
    int in1[512][HAFT_SIZE], in2[512][HAFT_SIZE], outMatrix[512][HAFT_SIZE];

    vector<vector<int>> matrix1 = randomMatrix(size, size);
    vector<vector<int>> matrix2 = randomMatrix(size, size);

    for(int r = 0; r < size; r++)
    {
        for(int c = 0; c < size; c++)
        {
            in1[r][c] = matrix1[r][c];
            in2[r][c] = matrix2[r][c];
        }
    }

    cout<<"Show matrix 1"<<endl;
    showMatrix(matrix1);

     cout<<"Show matrix 2"<<endl;
    showMatrix(matrix2);


    bool flag = true;
    auto started1 = chrono::high_resolution_clock::now();
    vector<vector<int>> expectedMatrix = mutipleMatrix(matrix1, matrix2);
     auto done1 = chrono::high_resolution_clock::now();
     double time1 = chrono::duration_cast<std::chrono::milliseconds>(done1-started1).count();
      cout <<"mutipleMatrix time(seconds): "<<time1 <<endl;

    cout<<"Show expectation matrix"<<endl;
    showMatrix(expectedMatrix);

//    auto started2 = chrono::high_resolution_clock::now();
//   vector<vector<int>> actualMatrix = strassenMutipleMatrix1(matrix1, Position(0,0), matrix2, Position(0,0), matrix1.size());
//    auto done2 = chrono::high_resolution_clock::now();
//    double time2 = chrono::duration_cast<std::chrono::milliseconds>(done2-started2).count();
//     cout <<"strassenMutipleMatrix1 time(seconds): "<<time2 <<endl;
//     cout<<"Show actual matrix"<<endl;
//     showMatrix(actualMatrix);

    auto started2 = chrono::high_resolution_clock::now();
    //strassenMutipleMatrix3((int**)in1, Position(0,0), (int**)in2, Position(0,0), (int**)outMatrix, size);
    auto done2 = chrono::high_resolution_clock::now();
    double time2 = chrono::duration_cast<std::chrono::milliseconds>(done2-started2).count();
     cout <<"strassenMutipleMatrix3 time(seconds): "<<time2 <<endl;
     cout<<"Show actual matrix"<<endl;
     showMatrix((int**)outMatrix, size, size);


     auto started3 = chrono::high_resolution_clock::now();
    vector<vector<int>> actualMatrix2 = strassenMutipleMatrix2(matrix1, Position(0,0), matrix2, Position(0,0), matrix1.size());
     auto done3 = chrono::high_resolution_clock::now();
     double time3 = chrono::duration_cast<std::chrono::milliseconds>(done3-started3).count();
      cout <<"strassenMutipleMatrix2 time(seconds): "<<time3 <<endl;


    cout<<"Show actual matrix"<<endl;
    showMatrix(actualMatrix2);

    for(int i = 0; i< expectedMatrix.size(); i++)
    {
        for(int j = 0; j < expectedMatrix[0].size(); j++)
        {
            if(outMatrix[i][j] != expectedMatrix[i][j] || actualMatrix2[i][j] != expectedMatrix[i][j] )
            {
                flag = false;
                break;
            }
        }
    }

    if(flag)
    {
        cout<<"mutipleMatrix PASS!"<<endl;
    }
    else
    {
        cout<<"mutipleMatrix Fail!"<<endl;
    }
}
