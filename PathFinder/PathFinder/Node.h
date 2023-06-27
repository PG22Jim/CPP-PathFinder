// Copyright © 2022 Jim Chen, All Rights Reserved

#pragma once

#include "SquareData.h"

// Weight variable to calculate Fcost
const float WEIGHT = 2;


// Fcost calculation type
enum FCostType
{
	Classic,
	DynamicWeighted,
	PiecewiseDownwardCurve,
	ConvexUpwardParabola,
};


class Node
{
private:

	// actual cost from start to current Node
	float gCost;

	// estimated cost from current node to goal
	float hCost;

	SquareData* data;
	Node* parentNode;



public:

	


	float getSum(FCostType calculationType) const;

	// Get and set functions
	float getGCost() const { return gCost; }
	float getHCost() const { return hCost; }
	SquareData* getSquareData() const { return data; }
	Node* getParentNode() const { return parentNode; }

	void setGCost(float newG) { gCost = newG; }
	void setHCost(float newH) { hCost = newH; }
	void setParentNode(Node* newParentNode) { parentNode = newParentNode; }
	
	// reseting node function
	void resetNode();


	// constructor
	Node(SquareData* storingData)
	{
		data = storingData;
		gCost = 99;
		hCost = 99;
		parentNode = nullptr;
	}

	// constructor
	Node(SquareData* storingData, Node* storingPreviousData, int h, GridMovement requestingGridMovement)
	{
		data = storingData;
		parentNode = storingPreviousData;

		hCost = h;

		if (parentNode) gCost = parentNode->getGCost() + storingData->getKey().findDistance(parentNode->getSquareData()->getKey(), requestingGridMovement);
		else gCost = 0;
	}

	~Node();

};

