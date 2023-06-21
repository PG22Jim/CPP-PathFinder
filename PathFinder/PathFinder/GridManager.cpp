#include "GridManager.h"

GridManager::GridManager(int column, int row)
{
	gridTable = new GridTable(column, row);
}

void GridManager::onUpdatePath()
{
    // If there is any showing path
    if (gridTable->getPathToGoal())
    {
        // clear path openset, closeset, and pathNode
        gridTable->eraseAllNode();
        gridTable->clearExistingPath();
    }

    // update Path
    gridTable->tryPathFinding();
}

bool GridManager::isAbleTryPathFind()
{
    return (gridTable->getStartSquareData()) && (gridTable->getGoalSquareData());
}

GridTable* GridManager::getGridTable()
{
	return gridTable;
}

void GridManager::onClearExistPath()
{

    // If there is any showing path
    if (gridTable->getPathToGoal())
    {
        // clear path
        gridTable->clearExistingPath();

    }
}

bool GridManager::OnChangeSquare(int column, int row, SquareStatus requestStatus)
{
    currentError = None;

    // Get suare data from grid table, if result is nullptr, no result
    SquareData* accessingSquareData = gridTable->getSquareData(column, row);
    if (!accessingSquareData) return false;

    SquareStatus accessingSquareStatus = accessingSquareData->getSquareStatus();

    SquareData* startData = gridTable->getStartSquareData();
    SquareData* goalData = gridTable->getGoalSquareData();

    switch (requestStatus)
    {
        // if user request accessing square to be Empty
        case Empty:
            // if accessing square is empty, no need to access it
            if ((accessingSquareStatus == SquareStatus::Empty) || (accessingSquareStatus == SquareStatus::Path) || (accessingSquareStatus == CalculatedPlace)) return false;

            // if accessing square is start or goal, make the reference as nullptr and change status
            if (accessingSquareStatus == Start) 
            {
                gridTable->setStartSquareData(nullptr);
                accessingSquareData->setSquareStatus(Empty);
                return true;
            }
            else if (accessingSquareStatus == Goal)
            {
                gridTable->setGoalSquareData(nullptr);
                accessingSquareData->setSquareStatus(Empty);
                return true;
            }

            // if accessing square is wall, change status and current wall num will decrease
            else if (accessingSquareStatus == Wall)
            {
                accessingSquareData->setSquareStatus(Empty);
                gridTable->currentWallNumDecrement();
                return true;
            }
            break;

        case Wall:
            // if accessing square is wall, no need to access it
            if (accessingSquareStatus == Wall) return false;
            
            if (gridTable->isAbleToAddMoreWall())
            {
                // if accessing square is empty, adding wall and current wall num increment
                if (accessingSquareStatus == Empty || accessingSquareStatus == CalculatedPlace)
                {
                    accessingSquareData->setSquareStatus(Wall);
                    gridTable->currentWallNumDecrement();
                    return true;
                }
                // if accessing square is start or goal, make the reference as nullptr and change status
                else if (accessingSquareStatus == Start)
                {
                    gridTable->setStartSquareData(nullptr);
                    accessingSquareData->setSquareStatus(Wall);
                    return true;
                }
                else if (accessingSquareStatus == Goal)
                {
                    gridTable->setGoalSquareData(nullptr);
                    accessingSquareData->setSquareStatus(Wall);
                    return true;
                }
                else if (accessingSquareStatus == Path)
                {
                    accessingSquareData->setSquareStatus(Wall);
                    return true;
                }
            }
            else 
            {
                currentError = NoMoreWallToAdd;
                return false;
            }
            break;

        case Start:
            // if accessing square is start, no need to access it
            if (accessingSquareStatus == Start)
                return false;

            // if accessing square is empty, goal or wall, check if there already has startSquare
            if (accessingSquareStatus == Empty || accessingSquareStatus == CalculatedPlace || accessingSquareStatus == Path)
            {
                if (startData)
                {
                    // swap status and set the reference as the new one
                    gridTable->swapSquareStatus(accessingSquareData, startData);
                    gridTable->setStartSquareData(accessingSquareData);
                    return true;
                }
                else
                {
                    // set accessing data as reference and change status
                    gridTable->setStartSquareData(accessingSquareData);
                    accessingSquareData->setSquareStatus(Start);
                    return true;
                }
            }
            else if (accessingSquareStatus == Goal)
            {
                if (startData)
                {
                    // dereference it to make it empty and swap status and set the reference as start
                    gridTable->setGoalSquareData(nullptr);
                    accessingSquareData->setSquareStatus(Empty);
                    gridTable->swapSquareStatus(accessingSquareData, startData);
                    gridTable->setStartSquareData(accessingSquareData);
                    return true;
                }
                else
                {
                    // make the reference as nullptr, set accessing data as reference change status
                    gridTable->setStartSquareData(accessingSquareData);
                    gridTable->setGoalSquareData(nullptr);
                    accessingSquareData->setSquareStatus(Start);
                    return true;
                }
            }
            else if (accessingSquareStatus == Wall)
            {
                if (startData)
                {
                    // Make it empty and swap status and set the reference as start, current wall num decrement
                    accessingSquareData->setSquareStatus(Empty);
                    gridTable->swapSquareStatus(accessingSquareData, startData);
                    gridTable->setStartSquareData(accessingSquareData);
                    gridTable->currentWallNumDecrement();
                    return true;
                }
                else
                {
                    // overwrite status, make it as reference and current wall status decrement
                    accessingSquareData->setSquareStatus(Start);
                    gridTable->setStartSquareData(accessingSquareData);
                    gridTable->currentWallNumDecrement();
                    return true;
                }
            }

            break;
        case Goal:
            // if accessing square is goal, no need to access it
            if (accessingSquareStatus == Goal)
                return false;

            // if accessing square is empty, goal or wall, check if there already has goalSquare
            if (accessingSquareStatus == Empty || accessingSquareStatus == CalculatedPlace || accessingSquareStatus == Path)
            {
                if (goalData)
                {
                    // swap status and set the reference as the new one
                    gridTable->swapSquareStatus(accessingSquareData, goalData);
                    gridTable->setGoalSquareData(accessingSquareData);
                    return true;
                }
                else
                {
                    // set accessing data as reference and change status
                    gridTable->setGoalSquareData(accessingSquareData);
                    accessingSquareData->setSquareStatus(Goal);
                    return true;
                }
            }
            else if (accessingSquareStatus == Start)
            {
                if (goalData)
                {
                    // dereference it to make it empty and swap status and set the reference as start
                    gridTable->setStartSquareData(nullptr);
                    accessingSquareData->setSquareStatus(Empty);
                    gridTable->swapSquareStatus(accessingSquareData, goalData);
                    gridTable->setGoalSquareData(accessingSquareData);
                    return true;
                }
                else
                {
                    // make the reference as nullptr, set accessing data as reference change status
                    gridTable->setGoalSquareData(accessingSquareData);
                    gridTable->setStartSquareData(nullptr);
                    accessingSquareData->setSquareStatus(Goal);
                    return true;
                }
            }
            else if (accessingSquareStatus == Wall)
            {
                if (goalData)
                {
                    // Make it empty and swap status and set the reference as start, current wall num decrement
                    accessingSquareData->setSquareStatus(Empty);
                    gridTable->swapSquareStatus(accessingSquareData, goalData);
                    gridTable->setGoalSquareData(accessingSquareData);
                    gridTable->currentWallNumDecrement();
                    return true;
                }
                else
                {
                    // overwrite status, make it as reference and current wall status decrement
                    accessingSquareData->setSquareStatus(Goal);
                    gridTable->setGoalSquareData(accessingSquareData);
                    gridTable->currentWallNumDecrement();
                    return true;
                }
            }

            break;
        case Path:
            return false;
            break;
        default:
            return false;
            break;
    }
    return false;
}



void GridManager::OnReceiveUserMouseButton(sf::Vector2i mousePos, sf::Mouse::Button mouseButtonType)
{
    // If the left mouse button is pressed
    if (mouseButtonType == sf::Mouse::Left)
    {
        // Print mouse position if mouse cursor position is in grid
        if (GRID_LENGTH > mousePos.x && GRID_LENGTH > mousePos.y)
        {
            // convert vector 2D mouse to row and column
            const int mouseRow = mousePos.x / SQUARE_SIZE;
            const int mouseColumn = mousePos.y / SQUARE_SIZE;


            if (OnChangeSquare(mouseColumn, mouseRow, currentRequestStatus)) 
            {
                if (isAbleTryPathFind() && gridTable->getPathToGoal())
                {
                    onUpdatePath();
                }
            }


        }
    }

    // If the right mouse button is pressed
    else if (mouseButtonType == sf::Mouse::Right)
    {
        // Print mouse position if mouse cursor position is in grid
        if (GRID_LENGTH > mousePos.x && GRID_LENGTH > mousePos.y)
        {
            // convert vector 2D mouse to row and column
            const int mouseRow = mousePos.x / SQUARE_SIZE;
            const int mouseColumn = mousePos.y / SQUARE_SIZE;

            // Right click means removing square
            if (OnChangeSquare(mouseColumn, mouseRow, Empty))
            {
                if (isAbleTryPathFind() && gridTable->getPathToGoal())
                {
                    onUpdatePath();
                }
            }

        }
    }
}

GridManager::~GridManager()
{
}
