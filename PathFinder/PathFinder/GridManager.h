#pragma once

#include "GridTable.h"







class GridManager
{
private:

	GridTable* gridTable;
	

public:


	GridTable* getGridTable();


	GridManager() = delete;
	GridManager(int column, int row);
	~GridManager();


};

