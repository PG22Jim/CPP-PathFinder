#pragma once

#include "SquareData.h"

class Node
{
public:

	SquareData* data;
	Node* previousData;


	Node(SquareData* storingData)
	{
		data = storingData;
		previousData = nullptr;
	}


	Node(SquareData* storingData, Node* storingPreviousData)
	{
		data = storingData;
		previousData = storingPreviousData;
	}

};

