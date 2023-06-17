#pragma once
#include <cstdlib>
#include <vector>
#include <iostream>
#include "Node.h"
#include "SquareData.h"

class GridTable
{
private:

	// Private variables
	int squareListSize = 100;
	
	SquareData* startSquareData = nullptr;
	SquareData* goalSquareData = nullptr;

	//std::vector<SquareData*> SquareList;
	std::vector<Node*> openSet;
	std::vector<Node*> closeSet;

	Node* pathToGoal = nullptr;


	void initializeGridTable();

	SquareData* generateRandomSquare(int maxColumn, int maxRow, SquareStatus requestStatus);

	bool squareCompare_IsSamePos(SquareData* dataA, SquareData* dataB);

	SquareData* getSquareData(int requestColumn, int requestRow);

	void getPathToGoal();

	std::vector<Node*> tryExploreNode(Node* exploringNode);

	bool canExploreThisSquare(SquareData* checkingNode);

	Node* findNextExploreNode();

	int getGValue(Node* targetNode);
	
	int getHValue(Node* targetNode);



	void addNewOpenSet(Node* addingNode);

	void removeOpenSet(Node* removingNode);

	void addNewCloseSet(Node* addingNode);


public:

	// Get set functions
	//std::unordered_map<Key, SquareData*> getGridData() { return gridData; }
	std::map<SquareKey, SquareData*> gridData;


	GridTable();
	~GridTable();
};

