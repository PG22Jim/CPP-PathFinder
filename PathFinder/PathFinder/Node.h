#pragma once

#include "SquareData.h"

class Node
{
private:

	int gCost;
	int hCost;



public:

	SquareData* data;
	Node* previousData;

	

	int getSteps();
	int getSum() const { return gCost + hCost; }


	Node(SquareData* storingData)
	{
		data = storingData;
		gCost = 0;
		hCost = 0;
		previousData = nullptr;
	}


	Node(SquareData* storingData, Node* storingPreviousData, int g, int h)
	{
		data = storingData;
		previousData = storingPreviousData;
		gCost = g;
		hCost = h;
	}

};

