#include "GridManager.h"

GridManager::GridManager(int column, int row)
{
	gridTable = new GridTable(column, row);
}

GridTable* GridManager::getGridTable()
{
	return gridTable;
}

GridManager::~GridManager()
{
}
