// Copyright © 2022 Jim Chen, All Rights Reserved

#pragma once
#include <cstdlib>
#include <vector>
#include <iostream>
#include "Node.h"
#include "SquareData.h"
#include <map>
#include <algorithm>
#include <queue>
#include <chrono>
#include <ctime> 

// Message variables
const sf::String MISSSTARTENDMESSAGE = "Cannot Find Path Without Start or End Point.";
const sf::String NOVALIDPATHMESSAGE = "No Valid Path, Make Sure There is a Path.";


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


	std::map<SquareKey, Node*> openSet;
	std::map<SquareKey, Node*> closeSet;

	int maxWallNum = 10;
	int currentWallNum = 0;




	Node* pathToGoal = nullptr;

	// initialization functions
	void initializeGridTable();

	Node* findNextExploreNode(FCostType costType);
	std::vector<Node*> getValidNeighborNodes(Node* exploringNode, GridMovement requestingGridMovement);
	bool canExploreThisSquare(SquareData* checkingNode);

	void allocatePathToGoal();

	bool squareCompare_IsSamePos(SquareData* dataA, SquareData* dataB);
	bool NodeCompare(const Node* leftNode, const Node* rightNode, FCostType costType) const;


	// open set and close set adding and removing function
	void addNewOpenSet(Node* addingNode);
	void removeOpenSet(Node* removingNode);
	void addNewCloseSet(Node* addingNode);

	std::string getPathFindMessage(float elapsedTime, GridMovement requestingMovement, FCostType costType) const;

public:

	std::map<SquareKey, Node*> gridData;
	
	
	sf::String tryPathFinding(GridMovement requestingMovement, FCostType costType);
	Node* getPathToGoal() const { return pathToGoal; }
	void clearExistingPath();

	void swapSquareStatus(SquareData* squareA, SquareData* squareB);



	// wall functions
	bool isAbleToAddMoreWall() const { return currentWallNum + 1 <= maxWallNum; }
	void currentWallNumIncrement() { currentWallNum++; }
	void currentWallNumDecrement() { currentWallNum--; }
	

	// Get set functions

	bool isAblePathFind();

	SquareData* getSquareData(int requestColumn, int requestRow);
	Node* tryGetGridFromTable(int requestColumn, int requestRow);

	SquareData* getStartSquareData() const { return startSquareData; }
	void setStartSquareData(SquareData* newData) { startSquareData = newData; }

	SquareData* getGoalSquareData() const { return goalSquareData; }
	void setGoalSquareData(SquareData* newData) { goalSquareData = newData; }

	// constructor and destructor
	GridTable();
	~GridTable();
};

