// Copyright © 2022 Jim Chen, All Rights Reserved

#pragma once

#include "GridTable.h"
#include "SFML/Graphics.hpp"






class GridManager
{
private:


	GridTable* gridTable;
	ErrorType currentError = ErrorType::None;

	GridMovement currentGridMovement = NeighborOnly;
	FCostType currentFCostType = Classic;



public:


	GridTable* getGridTable();

	sf::String onUpdatePath();
	bool setGridMovement(GridMovement newMovement);

	bool setFCostType(FCostType newCostType);


	void OnReceiveUserMouseButton(sf::Vector2i mousePos, sf::Mouse::Button mouseButtonType, bool isLeftKeyHolding, bool isRightKeyHolding);

	bool OnChangeSquare(int column, int row, SquareStatus requestStatus);


	void tick();
	void renderGrid(sf::RenderWindow* renderWindow);

	SquareStatus currentRequestStatus = SquareStatus::Empty;
	

	GridManager();
	~GridManager();


};

