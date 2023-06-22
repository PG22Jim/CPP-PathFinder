#pragma once
#include <cstdlib>
#include <vector>
#include <iostream>
#include "Node.h"
#include "SquareData.h"
#include <map>
#include <algorithm>



enum ErrorType
{
	MissingStartGoal,
	NoValidPath,
	NoPathToCancel,
	NoMoreWallToAdd,
	None
};





class GridTable
{
private:
	
	SquareData* startSquareData = nullptr;
	SquareData* goalSquareData = nullptr;

	//std::vector<SquareData*> SquareList;
	//std::vector<Node*> openSet;
	//std::vector<Node*> closeSet;
	std::map<SquareKey, Node*> openSet;
	std::map<SquareKey, Node*> closeSet;


	int maxColumn = 10;
	int maxRow = 10;

	int maxWallNum = 10;
	int currentWallNum = 0;





	Node* pathToGoal = nullptr;


	void initializeGridTable();

	void initializeWalls(int wallNum);

	SquareData* generateRandomSquare(int maxColumn, int maxRow, SquareStatus requestStatus);

	bool squareCompare_IsSamePos(SquareData* dataA, SquareData* dataB);


	std::vector<Node*> getValidNeighborNodes(Node* exploringNode, GridMovement requestingGridMovement);

	bool canExploreThisSquare(SquareData* checkingNode);




	void allocatePathToGoal();


	Node* findNextExploreNode();


	void addNewOpenSet(Node* addingNode);

	void removeOpenSet(Node* removingNode);

	void addNewCloseSet(Node* addingNode);


public:

	std::map<SquareKey, Node*> gridData;
	
	
	// Get set functions
	Node* getPathToGoal() const { return pathToGoal; }
	void clearExistingPath();
	void eraseAllNode();


	SquareData* getSquareData(int requestColumn, int requestRow);
	Node* tryGetGridFromTable(int requestColumn, int requestRow);

	SquareData* getStartSquareData() const { return startSquareData; }
	void setStartSquareData(SquareData* newData) { startSquareData = newData; }

	SquareData* getGoalSquareData() const { return goalSquareData; }
	void setGoalSquareData(SquareData* newData) { goalSquareData = newData; }

	void swapSquareStatus(SquareData* squareA, SquareData* squareB);

	void clearOpenSet();
	void clearCloseSet();

	bool isAbleToAddMoreWall() const { return currentWallNum + 1 <= maxWallNum; }
	void currentWallNumIncrement() { currentWallNum++; }
	void currentWallNumDecrement() { currentWallNum--; }
	
	ErrorType tryPathFinding(GridMovement requestingMovement);

	GridTable();
	GridTable(int column, int row);
	~GridTable();
};

