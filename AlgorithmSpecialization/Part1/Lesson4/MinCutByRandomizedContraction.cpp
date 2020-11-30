
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <algorithm>

#include "MinCutByRandomizedContraction.h"

using namespace std;

bool readAdjacencyListFromFile(vector<vector<int>>& adjacencyList, string fileName);
int randomUniform(int range_from, int range_to);
void SelectVertexRandom(const vector<vector<int>>& adjacencyList, int& idxU, int& idxV);
void mergeTwoToSingleVertex(vector<vector<int>>& adjacencyList, int idxU, int idxV);
void removeSelfLoops(vector<vector<int>>& adjacencyList, int idxU);
int ComputeMinCutByRandomizedContractionAlgorithm(vector<vector<int>> adjacencyList);
void removeSelfLoopsFull(vector<vector<int>>& adjacencyList);

void TestMinCut()
{
	/*vector<vector<int>> adjacencyList = vector<vector<int>>{
		{ 1, 2, 4},
		{2, 1, 4, 3},
		{3, 2, 4},
		{4, 1, 2, 3}
		};
	adjacencyList[1].erase(adjacencyList[1].begin() + 2);
	adjacencyList.erase(adjacencyList.begin() + 1);

	auto pos = std::find(adjacencyList[2].begin(), adjacencyList[2].end(), 2);
	adjacencyList[2].erase(pos);*/

	vector<vector<int>> adjacencyList;
	string fileName = "DataTest\\kargerMinCut.txt";
	readAdjacencyListFromFile(adjacencyList, fileName);
	int loopNumber = 1000/*90000*/;

	int minCut = ComputeMinCutByRandomizedContractionAlgorithm(adjacencyList);
	for (int i = 0; i < loopNumber; i++)
	{
		int resultMincut = ComputeMinCutByRandomizedContractionAlgorithm(adjacencyList);
		if (minCut > resultMincut)
		{
			minCut = resultMincut;
			cout << "Update loop Mincut = " << minCut << endl;
		}
	}
	cout << "Result Mincut = " << minCut << endl;
}

int ComputeMinCutByRandomizedContractionAlgorithm(vector<vector<int>> adjacencyList)
{
	int minCutResult = 0;
	int idxU = 0, idxV = 0;

	while (adjacencyList.size() > 2)
	{
		SelectVertexRandom(adjacencyList, idxU, idxV);
		mergeTwoToSingleVertex(adjacencyList, idxU, idxV);
	}

	if (adjacencyList[0].size() != adjacencyList[1].size())
	{
		cout << "Error: two supper vertexs are difference size" << endl;
	}
	minCutResult = adjacencyList[0].size() - 1;

	return minCutResult;
}

void swapRowWithLastRow(vector<vector<int>>& adjacencyList, int row)
{
	int lastRow = adjacencyList.size() - 1;

	for (int i = 0; i < adjacencyList.size(); i++)
	{
		for (int j = 0; j < adjacencyList[i].size(); j++)
		{
			if (adjacencyList[i][j] == row)
			{
				adjacencyList[i][j] = lastRow;
			}
			else if (adjacencyList[i][j] == lastRow)
			{
				adjacencyList[i][j] = row;
			}
		}
	}

	//Swap row with last row
	adjacencyList[row].swap(adjacencyList[lastRow]);

}

void removeSelfLoops(vector<vector<int>>& adjacencyList, int idxU)
{
	int u = adjacencyList[idxU][0];

	for (int i = 1; i < adjacencyList[idxU].size(); i++)
	{
		if (adjacencyList[idxU][i] == u)
		{
			adjacencyList[idxU].erase(adjacencyList[idxU].begin() + i);
		}
	}
}

void removeSelfLoopsFull(vector<vector<int>>& adjacencyList)
{
	for (int i = 0; i < adjacencyList.size(); i++)
	{
		int vertice = adjacencyList[i][0];

		for (int j = 1; j < adjacencyList[i].size(); j++)
		{


			if (adjacencyList[i][j] == vertice)
			{
				adjacencyList[i].erase(adjacencyList[i].begin() + j);
				j = j - 1;
			}
		}
	}
}


void mergeTwoToSingleVertex(vector<vector<int>>& adjacencyList, int idxU, int idxV)
{

	int u = adjacencyList[idxU][0];
	int v = adjacencyList[idxV][0];

	// Delete v on u
	auto possV = std::find(adjacencyList[idxU].begin(), adjacencyList[idxU].end(), v);
	if (possV != adjacencyList[idxU].end())
	{
		adjacencyList[idxU].erase(possV);
	}

	// Delete u on v
	auto possU = std::find(adjacencyList[idxV].begin(), adjacencyList[idxV].end(), u);
	if (possU != adjacencyList[idxV].end())
	{
		adjacencyList[idxV].erase(possU);
	}

	// Add remain v to u
	for (int i = 1; i < adjacencyList[idxV].size(); i++)
	{
		adjacencyList[idxU].push_back(adjacencyList[idxV][i]);
	}

	// Update  and delete
	for (int i = 0; i < adjacencyList.size(); i++)
	{
		for (int j = 1; j < adjacencyList[i].size(); j++)
		{
			if (adjacencyList[i][j] == v)
			{
				adjacencyList[i][j] = u;
			}
		}
	}

	//removeSelfLoops(adjacencyList, idxU);
	removeSelfLoopsFull(adjacencyList);

	adjacencyList.erase(adjacencyList.begin() + idxV);
}

void SelectVertexRandom(const vector<vector<int>>& adjacencyList, int& idxU, int& idxV)
{
	int range_from = 0;
	int range_to = adjacencyList.size() - 1;

	if (adjacencyList.size() <= 2)
	{
		cout << "Error Call SelectVertexRandom" << endl;
		return;
	}
	idxU = randomUniform(range_from, range_to);
	int u = adjacencyList[idxU][0];

	vector<int> listIDXVertexs;
	for (int i = 0; i < adjacencyList.size(); i++)
	{
		vector<int> row = adjacencyList[i];
		if ((i != idxU) && (std::find(row.begin(), row.end(), u) != row.end()))
		{
			listIDXVertexs.push_back(i);
		}
	}


	if (listIDXVertexs.size() > 1)
	{
		idxV = listIDXVertexs[randomUniform(0, listIDXVertexs.size() - 1)];

	}
	else if (listIDXVertexs.size() == 1)
	{
		idxV = listIDXVertexs[0];
	}
	else
	{
		cout << "Error!listVertexs.size = " << listIDXVertexs.size() << endl;
	}
}

int randomUniform(int range_from, int range_to)
{
	std::random_device                  rand_dev;
	std::mt19937                        generator(rand_dev());
	std::uniform_int_distribution<int> distr(range_from, range_to);
	int result = distr(generator);
	if (result < range_from || result > range_to)
	{
		cout << "Error! Random index" << endl;
	}
	return result;
}
bool readAdjacencyListFromFile(vector<vector<int>>& adjacencyList, string fileName)
{
	fstream streamFile(fileName, std::ios_base::in);
	string lineBuffer;
	if (streamFile.is_open() != true)
	{
		return false;
	}

	while (getline(streamFile, lineBuffer, '\n'))
	{
		istringstream is(lineBuffer);
		adjacencyList.push_back(vector<int>(std::istream_iterator<int>(is), std::istream_iterator<int>()));
	}
	streamFile.close();

	return true;
}