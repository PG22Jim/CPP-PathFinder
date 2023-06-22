#pragma once

#include "SquareData.h"

class Node
{
private:

	int gCost;
	int hCost;

	SquareData* data;
	Node* parentNode;

public:



	int calculatePathSteps();

	int getSum() const { return gCost + hCost; }
	int getGCost() const { return gCost; }
	int getHCost() const { return hCost; }
	void setGCost(int newG) { gCost = newG; }
	void setHCost(int newH) { hCost = newH; }
	void resetNode();


	SquareData* getSquareData() const { return data; }

	void setParentNode(Node* newParentNode) { parentNode = newParentNode; }
	Node* getParentNode() const { return parentNode; }

	Node(SquareData* storingData)
	{
		data = storingData;
		gCost = 99;
		hCost = 99;
		parentNode = nullptr;
	}


	Node(SquareData* storingData, Node* storingPreviousData, int h, GridMovement requestingGridMovement)
	{
		data = storingData;
		parentNode = storingPreviousData;

		hCost = h;

		if (parentNode) gCost = parentNode->getGCost() + storingData->getKey().findDistance(parentNode->getSquareData()->getKey(), requestingGridMovement);
		else gCost = 0;
;
	}

};

