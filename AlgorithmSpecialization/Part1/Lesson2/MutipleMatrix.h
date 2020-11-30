#ifndef MUTIPLEMATRIX_H
#define MUTIPLEMATRIX_H
#include<vector>

#define HAFT_SIZE 256
#define LOOP_NUMBER 10

typedef struct Position
{
    int row;
    int col;
    Position(int r, int c)
    {
        row = r;
        col = c;
    }
}Position;


void testMutipleMatrix();
std::vector<std::vector<int>> mutipleMatrix(std::vector<std::vector<int>>& matrix1, std::vector<std::vector<int>>& matrix2);
std::vector<std::vector<int>> strassenMutipleMatrix1(std::vector<std::vector<int>>& matrix1, Position p1, std::vector<std::vector<int>>& matrix2, Position p2, int size);
std::vector<std::vector<int>> strassenMutipleMatrix2(std::vector<std::vector<int>>& matrix1, Position p1, std::vector<std::vector<int>>& matrix2, Position p2, int size);
//void strassenMutipleMatrix3(int matrix1[][HAFT_SIZE], Position p1, int matrix2[][HAFT_SIZE], Position p2, int outMatrix[][HAFT_SIZE],int size);
//void strassenMutipleMatrix3(int** matrix1, Position p1, int** matrix2, Position p2, int** outMatrix,int size);



#endif // MUTIPLEMATRIX_H
