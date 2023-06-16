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
	Node* initialNode = nullptr;
	SquareData* startSquareData = nullptr;
	SquareData* goalSquareData = nullptr;


	std::vector<SquareData*> SquareList;
	std::vector<Node*> openSet;
	std::vector<Node*> closeSet;




	void initializeGridTable();

	SquareData* generateRandomSquare(int maxColumn, int maxRow, SquareStatus requestStatus);

	bool squareCompare_IsSamePos(SquareData* dataA, SquareData* dataB);

	SquareData* GetSquareData(int requestColumn, int requestRow);

	void getPathToGoal();

	std::vector<Node*> exploreNode(Node* exploringNode);

	bool canExploreThisNode(Node* checkingNode);

	Node* findNextExploreNode();

	void addNewOpenSet(Node* addingNode);

	void removeOpenSet(Node* removingNode);

	void addNewCloseSet(Node* addingNode);


public:

	// Get set functions
	std::vector<SquareData*> GetSquareList() { return SquareList; }


	GridTable();
	~GridTable();
};

