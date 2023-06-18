#pragma once

#include <map>
#include <stdlib.h>
#include "SFML/Graphics.hpp"
#include <iostream>


const int GRID_LENGTH = 800;     // Grid Length
const int GRID_ROWS = 10;   // Number of grid rows
const int GRID_COLUMNS = 10; // Number of grid columns
const int PADDING = 40;
const int SQUARE_SIZE = GRID_LENGTH / GRID_ROWS;
const float SQUARE_OUTLINE_THICKNESS = 3.0f;


const sf::Color ORANGE = sf::Color(255, 127, 0);
const sf::Color PURPLE = sf::Color(127, 0, 255);

enum SquareStatus
{
	Empty,
	Wall,
	Start,
	Goal,
	Path,
	OpenSet,
	CloseSet,
};

// Custom key type made by column and row, it is for hash table
struct SquareKey
{
private:
	int column;
	int row;

public:

	SquareKey() { column = 0; row = 0;}
	SquareKey(int newColumn, int newRow) { column = newColumn; row = newRow; }


	int getColumn() const { return column; }
	void setColumn(int newColumn) { column = newColumn; }
	int getRow() const { return row; }
	void setRow(int newRow) { row = newRow; }


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

	int findDistance(const SquareKey& targetKey) const
	{
		int columnDis = targetKey.getColumn() - column;
		int rowDis = targetKey.getRow() - row;
		int returnSum = abs(columnDis) + abs(rowDis);
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
	//const int getPositionValue(bool isColumn);
	
	// Getters and setters
	const SquareKey& getKey() { return squareKey; }
	const SquareKey& getKey() const { return squareKey; }
	void setKey(const SquareKey& newKey) { squareKey = newKey; }

	SquareStatus getSquareStatus() const { return squareStatus; }
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