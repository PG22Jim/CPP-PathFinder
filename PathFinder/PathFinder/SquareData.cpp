#include "SquareData.h"

void SquareData::initializeShape(SquareKey key)
{
	const int x = (SQUARE_SIZE * key.getRow());// +PADDING;
	const int y = (SQUARE_SIZE * key.getColumn());// +PADDING;

    squareShape.setPosition(sf::Vector2f(x, y));
    squareShape.setSize(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));

	squareShape.setOutlineColor(sf::Color::Black);
	squareShape.setOutlineThickness(SQUARE_OUTLINE_THICKNESS);
}

//const int SquareData::getPositionValue(bool isColumn)
//{
//	if (isColumn)
//		return squareKey.getColumn();
//
//	return squareKey.getRow();
//}

void SquareData::updateRectShapeColor(sf::Color color)
{
	squareShape.setFillColor(color);
}

SquareData::SquareData()
{
	squareKey = SquareKey();
	initializeShape(squareKey);
	squareStatus = ::Empty;
}

SquareData::SquareData(int column, int row, SquareStatus status)
{
	squareKey = SquareKey(column, row);
	initializeShape(squareKey);
	squareStatus = status;
}

SquareData::SquareData(const SquareData& other)
{
	squareKey = other.getKey_const();
	initializeShape(squareKey);
	squareStatus = other.squareStatus;
}

SquareData::~SquareData()
{
}
