#pragma once

#include "GridTable.h"
#include "SFML/Graphics.hpp"







class GridManager
{
private:


	GridTable* gridTable;
	sf::Text MessageBoard;
	ErrorType currentError = ErrorType::None;

	GridMovement currentGridMovement = NeighborOnly;


	bool isAbleTryPathFind();



public:


	GridTable* getGridTable();

	void onUpdatePath();
	void onClearExistPath();
	bool setGridMovement(GridMovement newMovement);

	void OnReceiveUserMouseButton(sf::Vector2i mousePos, sf::Mouse::Button mouseButtonType);

	bool OnChangeSquare(int column, int row, SquareStatus requestStatus);


	void tick();
	void renderGrid(sf::RenderWindow* renderWindow);

	SquareStatus currentRequestStatus = SquareStatus::Empty;
	

	GridManager() = delete;
	GridManager(int column, int row);
	~GridManager();


};

