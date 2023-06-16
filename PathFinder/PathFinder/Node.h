#pragma once

#include "SquareData.h"

class Node
{
public:

	SquareData* data;
	Node* nextData;
	Node* previousData;


	Node(SquareData* storingData)
	{
		data = storingData;
		nextData = nullptr;
		previousData = nullptr;
	}

	Node(SquareData* storingData, Node* storingNextData)
	{
		data = storingData;
		nextData = storingNextData;
		previousData = nullptr;
	}

	Node(SquareData* storingData, Node* storingNextData, Node* storingPreviousData)
	{
		data = storingData;
		nextData = storingNextData;
		previousData = storingPreviousData;
	}

};

