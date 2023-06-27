// Copyright © 2022 Jim Chen, All Rights Reserved

#include "GridManager.h"

GridManager::GridManager()
{
	gridTable = new GridTable();
}

sf::String GridManager::onUpdatePath()
{
    // update Path
    return gridTable->tryPathFinding(currentGridMovement, currentFCostType);
}


bool GridManager::setGridMovement(GridMovement newMovement)
{
    if (currentGridMovement != newMovement) 
    {
        currentGridMovement = newMovement;
        return true;
    }
    return false;
}

bool GridManager::setFCostType(FCostType newCostType)
{
    if (currentFCostType != newCostType)
    {
        currentFCostType = newCostType;
        return true;
    }
    return false;
}

GridTable* GridManager::getGridTable()
{
	return gridTable;
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



void GridManager::OnReceiveUserMouseButton(sf::Vector2i mousePos, sf::Mouse::Button mouseButtonType, bool isLeftKeyHolding, bool isRightKeyHolding)
{

    // Print mouse position if mouse cursor position is in grid
    if (GRID_LENGTH + PADDING > mousePos.x && GRID_LENGTH + PADDING > mousePos.y)
    {
        // convert vector 2D mouse to row and column
        const int mouseRow = (mousePos.x - PADDING) / SQUARE_SIZE;
        const int mouseColumn = (mousePos.y - PADDING) / SQUARE_SIZE;

        bool bIsSquareChanged = false;

        // If the left mouse button is pressed
        if (mouseButtonType == sf::Mouse::Left || isLeftKeyHolding)
            bIsSquareChanged = OnChangeSquare(mouseColumn, mouseRow, currentRequestStatus);
        
        // If the right mouse button is pressed
        else if (mouseButtonType == sf::Mouse::Right || isRightKeyHolding)
            bIsSquareChanged = OnChangeSquare(mouseColumn, mouseRow, Empty);
        
        if (bIsSquareChanged && gridTable->isAblePathFind() && gridTable->getPathToGoal())
        {
            onUpdatePath();
        }
    }
}

GridManager::~GridManager()
{
}

void GridManager::tick()
{
}

void GridManager::renderGrid(sf::RenderWindow* renderWindow)
{
    // Draw grid
    for (const auto& eachMapObject : gridTable->gridData)
    {
        SquareData* eachData = eachMapObject.second->getSquareData();
        SquareKey eachKey = eachMapObject.first;
        SquareStatus eachStatus = eachData->getSquareStatus();


        if (eachStatus == Empty)
            eachData->updateRectShapeColor(sf::Color::Yellow);
        else if (eachStatus == Wall)
            eachData->updateRectShapeColor(sf::Color::Red);
        else if (eachStatus == Start)
            eachData->updateRectShapeColor(sf::Color::Green);
        else if (eachStatus == Goal)
            eachData->updateRectShapeColor(sf::Color::Blue);
        else if (eachStatus == CalculatedPlace)
            eachData->updateRectShapeColor(sf::Color::Cyan);
        else
        {
            eachData->updateRectShapeColor(PURPLE);
        }

        renderWindow->draw(eachData->getShape());
    }

}
