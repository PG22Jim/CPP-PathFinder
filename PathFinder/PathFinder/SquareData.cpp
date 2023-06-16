#include "SquareData.h"

SquareData::SquareData()
{
	columnIndex = 0;
	rowIndex = 0;
	squareStatus = ::Empty;
}

SquareData::SquareData(int column, int row, SquareStatus status)
{
	columnIndex = column;
	rowIndex = row;
	squareStatus = status;
}

SquareData::SquareData(const SquareData& other)
{
	columnIndex = other.columnIndex;
	rowIndex = other.rowIndex;
	squareStatus = other.squareStatus;
}

SquareData::~SquareData()
{
}
