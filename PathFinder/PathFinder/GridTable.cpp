#include "GridTable.h"


void GridTable::initializeGridTable()
{
    // Nested for loop to initialize square data
    int iterationIndex = 0;
    for (int i = 0; i < maxRow; i++)
    {
        for (int j = 0; j < maxColumn; j++)
        {
            SquareData* newData = new SquareData(j, i, Empty);
            
            Node* newNode = new Node(newData);
            SquareKey newDataKey = newData->getKey();

            gridData.insert({ newDataKey , newNode });

            iterationIndex++;
        }
    }

}

void GridTable::initializeWalls(int wallNum)
{
    for (int i = 0; i < wallNum; i++)
    {
        bool bIsWallDecided = false;

        while (!bIsWallDecided)
        {
            // Providing a seed value
            srand((unsigned)time(NULL));

            //
            int rndColumn = rand() % 10;
            int rndRow = rand() % 10;

            SquareData* returnData = getSquareData(rndColumn, rndRow);
            if (!returnData) continue;

            SquareStatus returnStatus = returnData->getSquareStatus();
            if (returnStatus != Empty) continue;

            returnData->setSquareStatus(SquareStatus::Wall);
            bIsWallDecided = true;
        }
    }
}

SquareData* GridTable::generateRandomSquare(int maxColumn, int maxRow, SquareStatus requestStatus)
{
    // Providing a seed value
    srand((unsigned)time(NULL));

    //
    int rndColumn = rand() % maxColumn;
    int rndRow = rand() % maxRow;

    return new SquareData(rndColumn, rndRow, requestStatus);
}

bool GridTable::squareCompare_IsSamePos(SquareData* dataA, SquareData* dataB)
{
    bool isKeySame = dataA->getKey().operator==(dataB->getKey());
    return isKeySame;
}

void GridTable::clearExistingPath()
{
    if (!pathToGoal) return;


    //Node* iterateNode = pathToGoal;
    ////Node* clearingNode = nullptr;
    //while (iterateNode)
    //{
    //    //clearingNode = iterateNode;

    //    // while loop to clear path
    //    SquareData* currentData = iterateNode->getSquareData();
    //    if (currentData->getSquareStatus() == Path) currentData->setSquareStatus(Empty);

    //    iterateNode = iterateNode->getParentNode();
    //    //clearingNode = nullptr;
    //}

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

SquareData* GridTable::getSquareData(int requestColumn, int requestRow)
{
    SquareKey findingKey{ requestColumn, requestRow };
    auto foundObject = gridData.find(findingKey);

    if (foundObject != gridData.end())
        return foundObject->second->getSquareData();
    else
        return nullptr;
}

Node* GridTable::tryGetGridFromTable(int requestColumn, int requestRow)
{
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

ErrorType GridTable::tryPathFinding()
{
    if (!startSquareData || !goalSquareData) return ErrorType::MissingStartGoal;
    pathToGoal = nullptr;

    // initialize openset, closeset, and pathNode
    eraseAllNode();
    clearExistingPath();

    // add new created start Node into openset
    SquareKey currentStartSquareKey = startSquareData->getKey();
    int startHCost = currentStartSquareKey.findDistance(goalSquareData->getKey());
    addNewOpenSet(new Node(startSquareData, nullptr, startHCost));

    // iteration begin
    while (!openSet.empty())
    {

        Node* nextExploreNode = findNextExploreNode();
        //Node* nextExploreNode = openSet.begin()->second;
        if (nextExploreNode)
        {
            removeOpenSet(nextExploreNode);
            addNewCloseSet(nextExploreNode);

            // If status is goal
            SquareStatus exploreStatus = nextExploreNode->getSquareData()->getSquareStatus();
            if (exploreStatus == SquareStatus::Goal)
            {
                // assign it as pathToGoal, remove from openSet, then clear openSet
                pathToGoal = nextExploreNode;
                removeOpenSet(pathToGoal);
                allocatePathToGoal();
                return ErrorType::None;
            }

            // find all walkable neighbors around nextExploreNode
            std::vector<Node*> successorNodes = getValidNeighborNodes(nextExploreNode);
            //std::vector<SquareData*> validNeighbors = getValidNeighborNodes(nextExploreNode);

            for (Node* eachNeighbor : successorNodes)
            {
                SquareKey neighborKey = eachNeighbor->getSquareData()->getKey();
                int tentativeGCost = nextExploreNode->getGCost() + neighborKey.findDistance(nextExploreNode->getSquareData()->getKey());
                auto foundItem = openSet.find(neighborKey);
                const bool bItemFound = foundItem != openSet.end();

                if (tentativeGCost < eachNeighbor->getGCost() || !bItemFound)
                {
                    eachNeighbor->setGCost(tentativeGCost);
                    eachNeighbor->setHCost(eachNeighbor->getSquareData()->getKey().findDistance(goalSquareData->getKey()));
                    eachNeighbor->setParentNode(nextExploreNode);

                    if (!bItemFound) 
                    {
                        addNewOpenSet(eachNeighbor);
                    }
                }
            }
        }
    }

    // if openset is empty, all possible squares are explored. return Error NoValidPath
    eraseAllNode();
    return ErrorType::NoValidPath;
}

std::vector<Node*> GridTable::getValidNeighborNodes(Node* exploringNode)
{
    std::vector<Node*> returnVector;

    SquareData* exploringSquareData = exploringNode->getSquareData();
    SquareKey exploringKey = exploringSquareData->getKey();
    int exploreColumn = exploringKey.getColumn();
    int exploreRow = exploringKey.getRow();






    Node* upNode = tryGetGridFromTable((exploreColumn - 1), exploreRow);
    if (upNode) 
    {
        if (canExploreThisSquare(upNode->getSquareData()))
        {
            returnVector.push_back(upNode);
        }
    }

    //Node* upleftNode = tryGetGridFromTable((exploreColumn - 1), exploreRow -1);
    //if (upleftNode)
    //{
    //    if (canExploreThisSquare(upleftNode->getSquareData()))
    //    {
    //        returnVector.push_back(upleftNode);
    //    }
    //}

    //Node* uprightNode = tryGetGridFromTable((exploreColumn - 1), exploreRow + 1);
    //if (uprightNode)
    //{
    //    if (canExploreThisSquare(uprightNode->getSquareData()))
    //    {
    //        returnVector.push_back(uprightNode);
    //    }
    //}

    Node* downNode = tryGetGridFromTable((exploreColumn + 1), exploreRow);
    if (downNode)
    {
        if (canExploreThisSquare(downNode->getSquareData()))
        {
            returnVector.push_back(downNode);
        }
    }

    //Node* downleftNode = tryGetGridFromTable((exploreColumn + 1), exploreRow -1);
    //if (downleftNode)
    //{
    //    if (canExploreThisSquare(downleftNode->getSquareData()))
    //    {
    //        returnVector.push_back(downleftNode);
    //    }
    //}

    //Node* downrightNode = tryGetGridFromTable((exploreColumn + 1), exploreRow + 1);
    //if (downleftNode)
    //{
    //    if (canExploreThisSquare(downrightNode->getSquareData()))
    //    {
    //        returnVector.push_back(downrightNode);
    //    }
    //}

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

    return returnVector;
}


bool GridTable::canExploreThisSquare(SquareData* checkingData)
{
    // get status and key of the checking data
    SquareStatus checkingSquareStatus = checkingData->getSquareStatus();
    SquareKey checkingSquareKey = checkingData->getKey();

    if(checkingSquareStatus == ::Wall) return false;
    if(checkingSquareStatus == ::Start) return false;

    if (!closeSet.empty())
    {
        auto findingItem = closeSet.find(checkingSquareKey);
        if (findingItem != closeSet.end())
        {
            return false;
        }
    }


    // if checking square is not start, wall, or goal, it means it is exploreable
    return true;
}

void GridTable::eraseAllNode()
{
    clearCloseSet();
    clearOpenSet();
}

void GridTable::clearOpenSet()
{
    if (openSet.size() > 0) 
    {
        for (auto itemInOpenSet : openSet)
        {
            Node* eachOpenNode = itemInOpenSet.second;
            // For each open node, check if it is path, start, or goal, if true, no need to erase pointer
            SquareStatus eachNodeStatus = eachOpenNode->getSquareData()->getSquareStatus();
            if (eachNodeStatus != CalculatedPlace) continue;

            eachOpenNode->getSquareData()->setSquareStatus(Empty);
            eachOpenNode = nullptr;
        }
    }

    openSet.clear();
}

void GridTable::clearCloseSet()
{
    if (closeSet.size() > 0)
    {
        for (auto itemInCloseSet : closeSet)
        {
            Node* eachcloseNode = itemInCloseSet.second;
            // For each open node, check if it is path, start, or goal, if true, no need to erase pointer
            SquareStatus eachNodeStatus = eachcloseNode->getSquareData()->getSquareStatus();
            if (eachNodeStatus != CalculatedPlace) continue;

            eachcloseNode->getSquareData()->setSquareStatus(Empty);
            eachcloseNode = nullptr;
        }
    }

    closeSet.clear();
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
            iterateData->setSquareStatus(SquareStatus::CloseSet);
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

Node* GridTable::findNextExploreNode()
{
    int openSetSize = openSet.size();

    // if there is no node in openset
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


    // if there are more than 1 node in openSet
    for (auto item : openSet)
    {
        Node* iterateNode = item.second;
        const int iterateFCost = iterateNode->getSum();
        const int bestNodeFCost = bestNode->getSum();

        if (iterateFCost < bestNodeFCost) 
        {
            bestNode = iterateNode;
        }
        else if (iterateFCost == bestNodeFCost) 
        {
            if (iterateNode->getHCost() < bestNode->getHCost())
            {
                bestNode = iterateNode;
            }
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

GridTable::GridTable()
{
    initializeGridTable();
}

GridTable::GridTable(int column, int row)
{
    maxColumn = column;
    maxRow = row;
    maxWallNum = (maxColumn * maxRow) / 5;
    currentWallNum = 0;
    initializeGridTable();
}


GridTable::~GridTable()
{
    //gridData.clear();
}
