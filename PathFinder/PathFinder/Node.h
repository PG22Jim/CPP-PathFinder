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
	void setGCost(int newG) { gCost = newG; }

	SquareData* getSquareData() const { return data; }

	void setParentNode(Node* newParentNode) { parentNode = newParentNode; }
	Node* getParentNode() const { return parentNode; }

	Node(SquareData* storingData)
	{
		data = storingData;
		gCost = 0;
		hCost = 0;
		parentNode = nullptr;
	}


	Node(SquareData* storingData, Node* storingPreviousData, int h)
	{
		data = storingData;
		parentNode = storingPreviousData;

		hCost = h;

		if (parentNode) gCost = parentNode->getGCost() + 1;
		else gCost = 0;
;
	}

};

