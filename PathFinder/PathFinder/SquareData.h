#pragma once

#include <map>
#include <stdlib.h>

enum SquareStatus
{
	Empty,
	Wall,
	Start,
	Goal,
	Path
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

	int findDistance(const SquareKey& targetKey) 
	{
		int columnDis = column - targetKey.getColumn();
		int rowDis = row - targetKey.getRow();
		return columnDis + rowDis;
	}
};


class SquareData
{
private:
	SquareKey squareKey;
	SquareStatus squareStatus;

public:
	// Getters and setters for Key
	const SquareKey& getKey() const { return squareKey; }
	void setKey(const SquareKey& newKey) { squareKey = newKey; }

	const int getPositionValue(bool isColumn);


	// Getters and setters for SquareStatus
	SquareStatus getSquareStatus() const { return squareStatus; }
	void setSquareStatus(SquareStatus newStatus) { squareStatus = newStatus; }



	SquareData();
	SquareData(int column, int row, SquareStatus status);
	SquareData(const SquareData& other);
	~SquareData();
};