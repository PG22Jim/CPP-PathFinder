// Copyright © 2022 Jim Chen, All Rights Reserved

#include "GridTable.h"


void GridTable::initializeGridTable()
{
    // Nested for loop to initialize every node in grid
    for (int i = 0; i < GRID_ROWS; i++)
    {
        for (int j = 0; j < GRID_COLUMNS; j++)
        {
            SquareData* newData = new SquareData(j, i, Empty);
            SquareKey newDataKey = newData->getKey();
            
            Node* newNode = new Node(newData);

            // use square key as key of map
            gridData.insert({ newDataKey , newNode });

        }
    }

}

bool GridTable::NodeCompare(const Node* leftNode, const Node* rightNode, FCostType costType) const
{
    // if left and right node's Fcost are same
    const float leftFCost = leftNode->getSum(costType);
    const float rightFCost = rightNode->getSum(costType);
    if (leftFCost == rightFCost)
    {
        // judge with their HCost
        const float leftHCost = leftNode->getHCost();
        const float rightHCost = rightNode->getHCost();

        return leftHCost > rightHCost;
    }

    return leftFCost > rightFCost;
};


bool GridTable::squareCompare_IsSamePos(SquareData* dataA, SquareData* dataB)
{
    bool isKeySame = dataA->getKey().operator==(dataB->getKey());
    return isKeySame;
}

void GridTable::clearExistingPath()
{
    // if there is no existing path, return
    if (!pathToGoal) return;

    // for each grid data, reset node if it is not start, goal, or wall
    for (auto item : gridData)
    {
        SquareStatus status = item.second->getSquareData()->getSquareStatus();
        if (status != Start && status != Goal && status != Wall)
        {
            item.second->getSquareData()->setSquareStatus(Empty);
            item.second->resetNode();
        }
    }


}

bool GridTable::isAblePathFind()
{
    // only try path find if start and goal exist
    return startSquareData && goalSquareData;
}

SquareData* GridTable::getSquareData(int requestColumn, int requestRow)
{
    // return square data if found object in grid data, else return nullptr
    SquareKey findingKey{ requestColumn, requestRow };
    auto foundObject = gridData.find(findingKey);

    if (foundObject != gridData.end())
        return foundObject->second->getSquareData();
    else
        return nullptr;
}

Node* GridTable::tryGetGridFromTable(int requestColumn, int requestRow)
{
    // return node if found object in grid data, else return nullptr
    SquareKey findingKey{ requestColumn, requestRow };
    auto foundObject = gridData.find(findingKey);

    if (foundObject != gridData.end())
        return foundObject->second;
    else
        return nullptr;
}

void GridTable::swapSquareStatus(SquareData* squareA, SquareData* squareB)
{
    SquareStatus statusA = squareA->getSquareStatus();
    SquareStatus statusB = squareB->getSquareStatus();

    squareA->setSquareStatus(statusB);
    squareB->setSquareStatus(statusA);
}

sf::String GridTable::tryPathFinding(GridMovement requestingMovement, FCostType costType)
{
    // early return if player haven't put start and goal
    if (!startSquareData || !goalSquareData) return MISSSTARTENDMESSAGE;
    
    pathToGoal = nullptr;

    // initialize openset, closeset, and pathNode
    openSet.clear();
    closeSet.clear();
    clearExistingPath();


    // add new created start Node into openset
    SquareKey currentStartSquareKey = startSquareData->getKey();
    int startHCost = currentStartSquareKey.findDistance(goalSquareData->getKey(), requestingMovement);
    addNewOpenSet(new Node(startSquareData, nullptr, startHCost, requestingMovement));

    // start timer
    auto start = std::chrono::system_clock::now();
    
    // iteration begin
    while (!openSet.empty())
    {
        // get next explore node
        Node* nextExploreNode = findNextExploreNode(costType);

        if (nextExploreNode)
        {
            // remove exploring node from openset and move to close set
            removeOpenSet(nextExploreNode);
            addNewCloseSet(nextExploreNode);

            // If status is goal
            SquareStatus exploreStatus = nextExploreNode->getSquareData()->getSquareStatus();
            if (exploreStatus == SquareStatus::Goal)
            {
                // assign it as pathToGoal, and allocate it
                pathToGoal = nextExploreNode;
                removeOpenSet(pathToGoal);
                allocatePathToGoal();

                // return found message with elapsed time
                auto end = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = end - start;
                
                return getPathFindMessage(elapsed_seconds.count(), requestingMovement, costType);
            }

            // find all walkable neighbors around nextExploreNode
            std::vector<Node*> successorNodes = getValidNeighborNodes(nextExploreNode, requestingMovement);

            // for each walkable neighbors
            for (Node* eachNeighbor : successorNodes)
            {
                // calcualte tentative GCost
                SquareKey neighborKey = eachNeighbor->getSquareData()->getKey();
                float tentativeGCost = nextExploreNode->getGCost() + neighborKey.findDistance(nextExploreNode->getSquareData()->getKey(), requestingMovement);

                // check if neighbor is in openset
                auto foundItem = openSet.find(neighborKey);
                const bool bItemFound = foundItem != openSet.end();

                // if tentative GCost is lower than neighbor's written GCost or it is not in openset
                if (tentativeGCost < eachNeighbor->getGCost() || !bItemFound)
                {
                    // update Gcost, HCost and parent node
                    eachNeighbor->setGCost(tentativeGCost);
                    eachNeighbor->setHCost(eachNeighbor->getSquareData()->getKey().findDistance(goalSquareData->getKey(), requestingMovement));
                    eachNeighbor->setParentNode(nextExploreNode);

                    // if it is not found, put it into openset
                    if (!bItemFound) 
                    {
                        addNewOpenSet(eachNeighbor);
                    }
                }
            }
        }
    }

    // if openset is empty, all possible squares are explored. return NoValidPath error message
    return NOVALIDPATHMESSAGE;
}

std::vector<Node*> GridTable::getValidNeighborNodes(Node* exploringNode, GridMovement requestingGridMovement)
{
    // empty vector
    std::vector<Node*> returnVector;

    // get column and row of exploring node
    SquareData* exploringSquareData = exploringNode->getSquareData();
    SquareKey exploringKey = exploringSquareData->getKey();
    int exploreColumn = exploringKey.getColumn();
    int exploreRow = exploringKey.getRow();

    // try to check if upNode is exist in grid
    Node* upNode = tryGetGridFromTable((exploreColumn - 1), exploreRow);
    if (upNode) 
    {
        // check if we can explore this node
        if (canExploreThisSquare(upNode->getSquareData()))
        {
            // if true, add into vector
            returnVector.push_back(upNode);
        }
    }

    Node* leftNode = tryGetGridFromTable(exploreColumn, exploreRow - 1);
    if (leftNode)
    {
        if (canExploreThisSquare(leftNode->getSquareData()))
        {
            returnVector.push_back(leftNode);
        }
    }

    Node* rightNode = tryGetGridFromTable(exploreColumn, exploreRow + 1);
    if (rightNode)
    {
        if (canExploreThisSquare(rightNode->getSquareData()))
        {
            returnVector.push_back(rightNode);
        }
    }

    Node* downNode = tryGetGridFromTable((exploreColumn + 1), exploreRow);
    if (downNode)
    {
        if (canExploreThisSquare(downNode->getSquareData()))
        {
            returnVector.push_back(downNode);
        }
    }

    // check diagonal nodes if movement is including diagonal
    if (requestingGridMovement == DiagonalAlso)
    {
        Node* upleftNode = tryGetGridFromTable((exploreColumn - 1), exploreRow -1);
        if (upleftNode)
        {
            if (canExploreThisSquare(upleftNode->getSquareData()))
            {
                returnVector.push_back(upleftNode);
            }
        }

        Node* uprightNode = tryGetGridFromTable((exploreColumn - 1), exploreRow + 1);
        if (uprightNode)
        {
            if (canExploreThisSquare(uprightNode->getSquareData()))
            {
                returnVector.push_back(uprightNode);
            }
        }
    
        Node* downleftNode = tryGetGridFromTable((exploreColumn + 1), exploreRow -1);
        if (downleftNode)
        {
            if (canExploreThisSquare(downleftNode->getSquareData()))
            {
                returnVector.push_back(downleftNode);
            }
        }

        Node* downrightNode = tryGetGridFromTable((exploreColumn + 1), exploreRow + 1);
        if (downrightNode)
        {
            if (canExploreThisSquare(downrightNode->getSquareData()))
            {
                returnVector.push_back(downrightNode);
            }
        }    
    }



    return returnVector;
}


bool GridTable::canExploreThisSquare(SquareData* checkingData)
{
    // get status and key of the checking data
    SquareStatus checkingSquareStatus = checkingData->getSquareStatus();
    SquareKey checkingSquareKey = checkingData->getKey();

    // no need to explore wall or start square
    if(checkingSquareStatus == ::Wall) return false;
    if(checkingSquareStatus == ::Start) return false;

    if (!closeSet.empty())
    {
        // return false if square is in closeset
        auto findingItem = closeSet.find(checkingSquareKey);
        if (findingItem != closeSet.end())
        {
            return false;
        }
    }


    // if checking square is not start, wall, or in closeset, it means it is exploreable
    return true;
}

void GridTable::allocatePathToGoal()
{
    for (auto itemInOpenSet : openSet)
    {
        Node* eachOpenNode = itemInOpenSet.second;
        SquareData* iterateData = eachOpenNode->getSquareData();
        SquareStatus iterateStatus = iterateData->getSquareStatus();
        if (iterateStatus == SquareStatus::Empty)
        {
            iterateData->setSquareStatus(SquareStatus::CalculatedPlace);
        }
    }

    for (auto itemInCloseSet : closeSet)
    {
        Node* eachCloseNode = itemInCloseSet.second;
        SquareData* iterateData = eachCloseNode->getSquareData();
        SquareStatus iterateStatus = iterateData->getSquareStatus();
        if (iterateStatus == SquareStatus::Empty)
        {
            iterateData->setSquareStatus(SquareStatus::CalculatedPlace);
        }
    }

    if (pathToGoal) 
    {
        Node* iterateNode = pathToGoal;

        while (iterateNode)
        {
            SquareData* iterateData = iterateNode->getSquareData();
            SquareStatus iterateStatus = iterateData->getSquareStatus();
            if (iterateStatus != SquareStatus::Start && iterateStatus != SquareStatus::Goal)
            {
                iterateData->setSquareStatus(SquareStatus::Path);
            }

            iterateNode = iterateNode->getParentNode();
        }
    }
}

Node* GridTable::findNextExploreNode(FCostType costType)
{

    // return nullptr if there is no node in openset
    int openSetSize = openSet.size();
    if (openSetSize == 0)
    {
        return nullptr;
    }

    Node* bestNode = openSet.begin()->second;
    
    // If there is only one node in openset, just return that
    if (openSetSize == 1)
    {
        return bestNode;
    }

    // for each node in openset, compare node and replace the best node if current node is better exploring point
    for (auto item : openSet)
    {
        if (NodeCompare(bestNode, item.second, costType))
        {
            bestNode = item.second;
        }
    }

    return bestNode;
}


void GridTable::addNewOpenSet(Node* addingNode)
{
    openSet.insert({ addingNode->getSquareData()->getKey(), addingNode });
}

void GridTable::removeOpenSet(Node* removingNode)
{
    openSet.erase(removingNode->getSquareData()->getKey());
}

void GridTable::addNewCloseSet(Node* addingNode)
{
    closeSet.insert({ addingNode->getSquareData()->getKey(), addingNode });
}

std::string GridTable::getPathFindMessage(float elapsedTime, GridMovement requestingMovement, FCostType costType) const
{
    std::string movementString;

    if (requestingMovement == DiagonalAlso) movementString = "Diagonal Also";
    else movementString = "Orthogonal Only";

    std::string costTypeString;

    if (costType == Classic) costTypeString = "Classic";
    else if (costType == ConvexUpwardParabola) costTypeString = "Convex Upward Parabola";
    else if (costType == DynamicWeighted) costTypeString = "Dynamic Weighted";
    else costTypeString = "Piecewise Downward Curve";


    return "Path Found! \nUsing Grid Method: " + movementString + "\nUsing Heuristic Function: " + costTypeString + "\nElapsed Time is: " + std::to_string(elapsedTime) + "s";
}

GridTable::GridTable()
{
    maxWallNum = (GRID_COLUMNS * GRID_ROWS) / 5;
    currentWallNum = 0;
    initializeGridTable();
}

GridTable::~GridTable()
{
    //gridData.clear();
}
