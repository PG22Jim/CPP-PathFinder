#include "SquareData.h"

const int SquareData::getPositionValue(bool isColumn)
{
	if (isColumn)
		return squareKey.getColumn();

	return squareKey.getRow();
}

SquareData::SquareData()
{
	squareKey = SquareKey();
	squareStatus = ::Empty;
}

SquareData::SquareData(int column, int row, SquareStatus status)
{
	squareKey = SquareKey();
	squareKey.setColumn(column);
	squareKey.setRow(row);
	squareStatus = status;
}

SquareData::SquareData(const SquareData& other)
{
	squareKey = other.getKey();
	squareStatus = other.squareStatus;
}

SquareData::~SquareData()
{
}
