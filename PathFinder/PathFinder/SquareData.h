#pragma once

enum SquareStatus
{
	Empty,
	Wall,
	Start,
	Goal,
	Path
};


class SquareData
{
private:

	// Private variables
	int columnIndex;
	int rowIndex;
	SquareStatus squareStatus;

public:

	// Get set functions
	int getColumnIndex() const { return columnIndex; }
	void setColumnIndex(int newIndex) { columnIndex = newIndex; }

	int getRowIndex() const { return rowIndex; }
	void setRowIndex(int newIndex) { rowIndex = newIndex; }

	SquareStatus getSquareStatus() const { return squareStatus; }
	void setSquareStatus(SquareStatus newStatus) { squareStatus = newStatus; }



	SquareData();
	SquareData(int column, int row, SquareStatus status);
	SquareData(const SquareData& other);
	~SquareData();
};