#include "GridTable.h"


void GridTable::initializeGridTable()
{

    // Set start and finish with random generator
    SquareData* startData = generateRandomSquare(maxColumn, maxRow, SquareStatus::Start);
    SquareData* goalData = generateRandomSquare(maxColumn, maxRow, SquareStatus::Goal);

    bool isGoalDecided = false;

    while (!isGoalDecided)
    {
        // if start position is different from goal position, set bool to true to stop while loop and continue
        if (!squareCompare_IsSamePos(goalData, startData))
        {
            isGoalDecided = true;
            continue;
        }

        goalData = generateRandomSquare(maxColumn, maxRow, SquareStatus::Goal);
    }

    // Nested for loop to initialize square data
    int iterationIndex = 0;
    for (int i = 0; i < maxRow; i++)
    {
        for (int j = 0; j < maxColumn; j++)
        {
            SquareData* newData = new SquareData(j, i, Empty);

            if (squareCompare_IsSamePos(newData, startData))
            {
                startSquareData = newData;
                startSquareData->setSquareStatus(SquareStatus::Start);
            }
            else if (squareCompare_IsSamePos(newData, goalData))
            {
                goalSquareData = newData;
                goalSquareData->setSquareStatus(SquareStatus::Goal);
            }
            
            //gridData[newData->getKey()] = newData;
            SquareKey newDataKey = newData->getKey();
            gridData.insert({ newDataKey , newData});
            // SquareList.push_back(newData);

            iterationIndex++;
        }
    }

    initializeWalls((maxColumn * maxRow)/10);
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

SquareData* GridTable::getSquareData(int requestColumn, int requestRow)
{
    SquareKey findingKey{ requestColumn, requestRow };
    auto foundObject = gridData.find(findingKey);

    if (foundObject != gridData.end())
        return foundObject->second;
    else
        return nullptr;

    //for (int i = 0; i < squareListSize; ++i)
    //{
    //    Key iteratingKey = SquareList.at(i)->getKey();
    //    int column = iteratingKey.getColumn();
    //    int row = iteratingKey.getRow();

    //    if (column == requestColumn && row == requestRow)
    //    {
    //        return SquareList[i];
    //    }
    //}

    //return nullptr;
}

void GridTable::tryPathFinding()
{
    openSet.clear();
    closeSet.clear();

    Node* currentNode = new Node(startSquareData);
    addNewOpenSet(currentNode);

    while (!openSet.empty())
    {
        // Explore iterating node and update other possible exploring node into open set
        std::vector<Node*> newExplorableNodes = tryExploreNode(currentNode);

        // Check if new return new Explorable Nodes has node that is goal
        for (Node* eachNewNode : newExplorableNodes)
        {
            // If status is goal
            SquareStatus eachStatus = eachNewNode->data->getSquareStatus();
            if (eachStatus == SquareStatus::Goal) 
            {
                // assign it as pathToGoal, remove from openSet, then clear openSet
                pathToGoal = eachNewNode;
                removeOpenSet(pathToGoal);
                // TODO: clear all nodes that are not goal

                openSet.clear();
                continue;
            }
        }


        Node* nextExploreNode = findNextExploreNode();
        if (nextExploreNode) 
        {
            currentNode = nextExploreNode;
        }

    }


    allocatePathToGoal();
}

std::vector<Node*> GridTable::tryExploreNode(Node* exploringNode)
{
    std::vector<Node*> returnVector;

    SquareData* exploringSquareData = exploringNode->data;
    SquareKey exploringKey = exploringSquareData->getKey();
    int exploreColumn = exploringKey.getColumn();
    int exploreRow = exploringKey.getRow();

    
    // Up square from the exploring square, if position is valid, keep checking
    if ((exploreColumn - 1) >= 0 && (exploreColumn - 1) <= 9)
    {
        // if squareData is in the gridData, keep checking
        SquareData* checkingSquare = getSquareData((exploreColumn - 1), exploreRow);
        if (checkingSquare) 
        {
            const bool bCanExplore = canExploreThisSquare(checkingSquare);
            //const bool bIsExploringSquare = squareCompare_IsSamePos(checkingSquare, exploringSquareData);

            // if square is explorable and is not same square to exploring node
            if (bCanExplore)
            {
                // make it as new node and add it to the openSet
                Node* newExplorableNode = new Node(checkingSquare, exploringNode);
                openSet.push_back(newExplorableNode);
                returnVector.push_back(newExplorableNode);
            }
        }
    }

    // Down square from the exploring square, if position is valid, keep checking
    if ((exploreColumn + 1) >= 0 && (exploreColumn + 1) <= 9)
    {
        // if squareData is in the gridData, keep checking
        SquareData* checkingSquare = getSquareData((exploreColumn + 1), exploreRow);
        if (checkingSquare)
        {
            const bool bCanExplore = canExploreThisSquare(checkingSquare);
            //const bool bIsExploringSquare = squareCompare_IsSamePos(checkingSquare, exploringSquareData);

            // if square is explorable and is not same square to exploring node
            if (bCanExplore)
            {
                // make it as new node and add it to the openSet
                Node* newExplorableNode = new Node(checkingSquare, exploringNode);
                openSet.push_back(newExplorableNode);
                returnVector.push_back(newExplorableNode);
            }
        }
    }

    // Left square from the exploring square, if position is valid, keep checking
    if ((exploreRow - 1) >= 0 && (exploreRow - 1) <= 9)
    {
        // if squareData is in the gridData, keep checking
        SquareData* checkingSquare = getSquareData(exploreColumn, exploreRow - 1);
        if (checkingSquare)
        {
            const bool bCanExplore = canExploreThisSquare(checkingSquare);
            //const bool bIsExploringSquare = squareCompare_IsSamePos(checkingSquare, exploringSquareData);

            // if square is explorable and is not same square to exploring node
            if (bCanExplore)
            {
                // make it as new node and add it to the openSet
                Node* newExplorableNode = new Node(checkingSquare, exploringNode);
                openSet.push_back(newExplorableNode);
                returnVector.push_back(newExplorableNode);
            }
        }
    }

    // Right square from the exploring square, if position is valid, keep checking
    if ((exploreRow + 1) >= 0 && (exploreRow + 1) <= 9)
    {
        // if squareData is in the gridData, keep checking
        SquareData* checkingSquare = getSquareData(exploreColumn, exploreRow + 1);
        if (checkingSquare)
        {
            const bool bCanExplore = canExploreThisSquare(checkingSquare);
            //const bool bIsExploringSquare = squareCompare_IsSamePos(checkingSquare, exploringSquareData);

            // if square is explorable and is not same square to exploring node
            if (bCanExplore)
            {
                // make it as new node and add it to the openSet
                Node* newExplorableNode = new Node(checkingSquare, exploringNode);
                openSet.push_back(newExplorableNode);
                returnVector.push_back(newExplorableNode);
            }
        }
    }


    // move exploring Node from openSet to closeSet
    removeOpenSet(exploringNode);
    addNewCloseSet(exploringNode);

    return returnVector;
}

bool GridTable::canExploreThisSquare(SquareData* checkingData)
{
    // get status and key of the checking data
    SquareStatus checkingSquareStatus = checkingData->getSquareStatus();
    SquareKey checkingSquareKey = checkingData->getKey();

    if(checkingSquareStatus == ::Wall) return false;
    if(checkingSquareStatus == ::Start) return false;


    int currentCloseSetSize = closeSet.size();
    if (currentCloseSetSize > 0)
    {
        // check if checking data is in closeSet
        for (int i = 0; i < currentCloseSetSize; i++)
        {
            // If key is same, squareData cannot be explore
            SquareKey closeSetKey = closeSet.at(i)->data->getKey();
            bool isKeySame = checkingSquareKey.operator==(closeSetKey);
            if (isKeySame) 
            {
                return false;
            }
        }
    }

    int currentOpenSetSize = openSet.size();
    if (currentOpenSetSize > 0) 
    {
        // check if checking data is in openSet
        for (int i = 0; i < currentOpenSetSize; i++) 
        {
            // if key is same, squareData cannot be explore
            SquareKey openSetKey = openSet.at(i)->data->getKey();
            bool isKeySame = checkingSquareKey.operator==(openSetKey);
            if (isKeySame)
            {
                return false;
            }
        }
    }

    // if checking square is not in closeSet, not in openSet, and is not start, wall, or goal, it means it is exploreable
    return true;
}

void GridTable::eraseAllNodeNotInPath(Node* pathNode)
{
    if (!pathNode) return;

    // For each node in open set
    for (Node* eachOpenNode : openSet) 
    {
        bool bIsPath = false;
        bool bIterateFinish = false;

        Node* iterateNode = pathNode;
        SquareKey eachOpenKey = eachOpenNode->data->getKey();
        while (!bIterateFinish)
        {
            SquareKey iterateKey = iterateNode->data->getKey();
            if (eachOpenKey.operator==(iterateKey))
            {
                bIsPath = true;
                bIterateFinish = true;
            }

            iterateNode = iterateNode->previousData;
            if (iterateNode != nullptr)
            {
                bIterateFinish = true;
            }
        }
        
        if (!bIsPath) 
        {
            eachOpenNode == nullptr;
        }
    }

    Node* secondIterateNode = pathNode;
    for (Node* eachCloseNode : closeSet)
    {
        SquareKey eachCloseKey = eachCloseNode->data->getKey();
        while (secondIterateNode->previousData != nullptr)
        {




            secondIterateNode = secondIterateNode->previousData;
        }
    }







}

void GridTable::allocatePathToGoal()
{
    if (pathToGoal) 
    {
        Node* iterateNode = pathToGoal;

        while (iterateNode)
        {
            SquareData* iterateData = iterateNode->data;
            SquareStatus iterateStatus = iterateData->getSquareStatus();
            if (iterateStatus == SquareStatus::Empty) 
            {
                iterateData->setSquareStatus(SquareStatus::Path);
            }

            iterateNode = iterateNode->previousData;
        }
    }
}

Node* GridTable::findNextExploreNode()
{
    int openSetSize = openSet.size();

    // if there is no node in the list
    if (openSetSize == 0) 
    {
        return nullptr;
    }

    // If there is only one node in the list
    if (openSetSize == 1) 
    {
        return openSet.at(0);
    }


    // if there are more than 1 node in openSet
    SquareKey startKey = startSquareData->getKey();
    SquareKey goalKey = goalSquareData->getKey();
    
    Node* bestNode = openSet.at(0);
    SquareKey bestNodeKey = bestNode->data->getKey();
    // G: distance from Node to the Start
    int bestG = bestNodeKey.findDistance(startKey);
    // H: distance from Node to the Goal
    int bestH = bestNodeKey.findDistance(goalKey);

    int bestSum = bestG + bestH;



    for (Node* eachNode : openSet) 
    {
        SquareKey iterateKey = eachNode->data->getKey();

        // G: distance from Node to the Start
        const int iterateG = iterateKey.findDistance(startKey);
        // H: distance from Node to the Goal
        const int iterateH = iterateKey.findDistance(goalKey);
        const int iterateSum = iterateG + iterateH;

        // if sum is bigger than best sum, next iteration
        if (iterateSum > bestSum) continue;

        // if sums are same
        else if (iterateSum == bestSum) 
        {
            // check if H value is smaller or not, if true, iterate node become best node
            if (iterateH < iterateH)
            {
                bestNode = eachNode;
                bestG = iterateG;
                bestH = iterateH;
                bestSum = iterateSum;
            }
        }
        else
        {
            // if iterate sum is smaller than best sum, iterate node become best node
            bestNode = eachNode;
            bestG = iterateG;
            bestH = iterateH;
            bestSum = iterateSum;
        }

    }


    return bestNode;

}

int GridTable::getGValue(Node* targetNode)
{
    return 0;
}

int GridTable::getHValue(Node* targetNode)
{
    SquareKey targetKey = targetNode->data->getKey();



    return 0;
}

void GridTable::addNewOpenSet(Node* addingNode)
{
    openSet.push_back(addingNode);
}

void GridTable::removeOpenSet(Node* removingNode)
{
    openSet.erase(std::remove(openSet.begin(), openSet.end(), removingNode), openSet.end());
}

void GridTable::addNewCloseSet(Node* addingNode)
{
    closeSet.push_back(addingNode);
}

GridTable::GridTable()
{
    initializeGridTable();
}

GridTable::GridTable(int column, int row)
{
    maxColumn = column;
    maxRow = row;
    initializeGridTable();
}


GridTable::~GridTable()
{
    //gridData.clear();
}
