// Copyright © 2022 Jim Chen, All Rights Reserved

#pragma once

#include <map>
#include <stdlib.h>
#include "SFML/Graphics.hpp"
#include <iostream>


const int GRID_LENGTH = 600;     // Grid Length
const int GRID_ROWS = 20;   // Number of grid rows
const int GRID_COLUMNS = 20; // Number of grid columns
const float DISTANCE_DIAGONAL = 14; // cost to diagonal movement
const float DISTANCE_STRAIGHT = 10; // cost to straight movement
const int PADDING = 40; // padding of square

const int SQUARE_SIZE = GRID_LENGTH / GRID_ROWS; // each grid square size
const float SQUARE_OUTLINE_THICKNESS = 3.0f;


const sf::Color ORANGE = sf::Color(255, 127, 0);
const sf::Color PURPLE = sf::Color(127, 0, 255);



enum GridMovement 
{
	NeighborOnly,
	DiagonalAlso
};

enum SquareStatus
{
	Empty,
	Wall,
	Start,
	Goal,
	Path,
	CalculatedPlace,
};

// Custom key type made by column and row, it is for hash table
struct SquareKey
{
private:
	int column;
	int row;

public:

	// constructor and destructor
	SquareKey() { column = 0; row = 0;}
	SquareKey(int newColumn, int newRow) { column = newColumn; row = newRow; }
	~SquareKey() {}

	// get and set function
	int getColumn() const { return column; }
	int getRow() const { return row; }

	void setColumn(int newColumn) { column = newColumn; }
	void setRow(int newRow) { row = newRow; }

	// operator == and < 
	bool operator==(const SquareKey& other) const
	{
		return column == other.column && row == other.row;
	}

	bool operator<(const SquareKey& other) const
	{
		if (column < other.column)
			return true;
		else if (column == other.column)
			return row < other.row;
		else
			return false;
	}

	// distance calculation
	float findDistance(const SquareKey& targetKey, GridMovement requestMovement) const
	{
		// OCTILE DISTANCE
		if (requestMovement == DiagonalAlso)
		{
			float dx = abs(row - targetKey.getRow());
			float dy = abs(column - targetKey.getColumn());
			return DISTANCE_STRAIGHT * (dx + dy) + (DISTANCE_DIAGONAL - 2.0f * DISTANCE_STRAIGHT) * std::fmin(dx, dy);
		}

		// MANHATTAN DISTANCE
		float columnDis = targetKey.getColumn() - column;
		float rowDis = targetKey.getRow() - row;
		float returnSum = DISTANCE_STRAIGHT * (abs(columnDis) + abs(rowDis));
		return returnSum;
	}
};


class SquareData
{
private:
	SquareKey squareKey;
	SquareStatus squareStatus;

	sf::RectangleShape squareShape;

	void initializeShape(SquareKey key);


public:
	
	// Getters and setters
	const SquareKey& getKey() { return squareKey; }
	const SquareKey& getKey_const() const { return squareKey; }
	SquareStatus getSquareStatus() const { return squareStatus; }

	void setKey(const SquareKey& newKey) { squareKey = newKey; }
	void setSquareStatus(SquareStatus newStatus) { squareStatus = newStatus; }


	// Public function to be called from other class
	void updateRectShapeColor(sf::Color color);

	sf::RectangleShape getShape() { return squareShape; }


	// Constructors and Destructor
	SquareData();
	SquareData(int column, int row, SquareStatus status);
	SquareData(const SquareData& other);
	~SquareData();
};