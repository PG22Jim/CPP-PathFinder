#include "GridTable.h"


void GridTable::initializeGridTable()
{
    //// Set start and finish with random generator
    //SquareData* startData = generateRandomSquare(maxColumn, maxRow, SquareStatus::Start);
    //SquareData* goalData = generateRandomSquare(maxColumn, maxRow, SquareStatus::Goal);

    //bool isGoalDecided = false;

    //while (!isGoalDecided)
    //{
    //    // if start position is different from goal position, set bool to true to stop while loop and continue
    //    if (!squareCompare_IsSamePos(goalData, startData))
    //    {
    //        isGoalDecided = true;
    //        continue;
    //    }

    //    goalData = generateRandomSquare(maxColumn, maxRow, SquareStatus::Goal);
    //}

    // Nested for loop to initialize square data
    int iterationIndex = 0;
    for (int i = 0; i < maxRow; i++)
    {
        for (int j = 0; j < maxColumn; j++)
        {
            SquareData* newData = new SquareData(j, i, Empty);

            //if (squareCompare_IsSamePos(newData, startData))
            //{
            //    startSquareData = newData;
            //    startSquareData->setSquareStatus(SquareStatus::Start);
            //}
            //else if (squareCompare_IsSamePos(newData, goalData))
            //{
            //    goalSquareData = newData;
            //    goalSquareData->setSquareStatus(SquareStatus::Goal);
            //}
            
            //gridData[newData->getKey()] = newData;
            SquareKey newDataKey = newData->getKey();
            gridData.insert({ newDataKey , newData});
            // SquareList.push_back(newData);

            iterationIndex++;
        }
    }

    //initializeWalls((maxColumn * maxRow)/10);
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


    Node* iterateNode = pathToGoal;
    Node* clearingNode = nullptr;
    while (iterateNode)
    {
        clearingNode = iterateNode;

        // while loop to clear path
        SquareData* currentData = iterateNode->data;
        if (currentData->getSquareStatus() == Path) currentData->setSquareStatus(Empty);

        iterateNode = iterateNode->previousData;
        clearingNode = nullptr;
    }
}

SquareData* GridTable::getSquareData(int requestColumn, int requestRow)
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


    openSet.clear();
    closeSet.clear();

    SquareKey currentStartSquareKey = startSquareData->getKey();
    int startHCost = currentStartSquareKey.findDistance(goalSquareData->getKey());

    Node* currentNode = new Node(startSquareData, nullptr, 0, startHCost);
    addNewOpenSet(currentNode);

    while (!openSet.empty())
    {
        // Explore iterating node and update other possible exploring node into open set
        std::vector<Node*> newExplorableNodes = getValidNeighborNodes(currentNode);

        if(newExplorableNodes.size() > 0)
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
                    allocatePathToGoal();
                    // TODO: clear all nodes that are not goal
                    eraseAllNodeNotInPath();
                    return ErrorType::None;
                }
            }

        // get next exploring node from openset
        Node* nextExploreNode = findNextExploreNode();
        if (nextExploreNode) 
        {
            // if nextExploreNode valid, continue to explore
            currentNode = nextExploreNode;
        }
    }


    // if openset is empty, all possible squares are explored. return Error NoValidPath
    eraseAllNodeNotInPath();
    return ErrorType::NoValidPath;
}

std::vector<Node*> GridTable::getValidNeighborNodes(Node* exploringNode)
{
    std::vector<Node*> returnVector;

    SquareData* exploringSquareData = exploringNode->data;
    SquareKey exploringKey = exploringSquareData->getKey();
    int exploreColumn = exploringKey.getColumn();
    int exploreRow = exploringKey.getRow();


    // Try explore up down left right square of exploringNode, if function return valid node it means it is explorable node, add to returnVector
    Node* upNode = tryExploreNode((exploreColumn - 1), exploreRow, exploringNode);
    if (upNode) returnVector.push_back(upNode);
    
    Node* downNode = tryExploreNode((exploreColumn + 1), exploreRow, exploringNode);
    if (downNode) returnVector.push_back(downNode);

    Node* leftNode = tryExploreNode(exploreColumn, exploreRow - 1, exploringNode);
    if (leftNode) returnVector.push_back(leftNode);

    Node* rightNode = tryExploreNode(exploreColumn, exploreRow + 1, exploringNode);
    if (rightNode) returnVector.push_back(rightNode);


    // move exploring Node from openSet to closeSet
    removeOpenSet(exploringNode);
    addNewCloseSet(exploringNode);

    return returnVector;
}

Node* GridTable::tryExploreNode(int requestColumn, int requestRow, Node* originalNode)
{
    Node* returnNode = nullptr;

    // if squareData is in the gridData, keep checking
    SquareData* checkingSquare = getSquareData(requestColumn, requestRow);
    if (checkingSquare)
    {
        const bool bCanExplore = canExploreThisSquare(checkingSquare);
        //const bool bIsExploringSquare = squareCompare_IsSamePos(checkingSquare, exploringSquareData);

        // if square is explorable and is not same square to exploring node
        if (bCanExplore)
        {
            // make it as new node with calculate g and h cost, then add to the openSet

            int checkingGCost = checkingSquare->getKey().findDistance(getStartSquareData()->getKey());
            int checkingHCost = checkingSquare->getKey().findDistance(getGoalSquareData()->getKey());

            // TODO: Probably should have a way to get other close path exploringNode
            returnNode = new Node(checkingSquare, originalNode, checkingGCost, checkingHCost);
            openSet.push_back(returnNode);
        }
    }

    return returnNode;
}


bool GridTable::isInGridRange(int requestColumn, int requestRow)
{
    return false;
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

void GridTable::eraseAllNodeNotInPath()
{
    // if there is no existing path
    if (!pathToGoal) 
    {
        // clear every node in openSet and closeSet, then clear them
        if (closeSet.size() > 0)
            for (Node* eachOpenNode : openSet)
            {
                eachOpenNode = nullptr;
            }

        if (closeSet.size() > 0)
            for (Node* eachCloseNode : closeSet)
            {
                eachCloseNode = nullptr;
            }

        openSet.clear();
        closeSet.clear();
        return;
    }

    // For each node in openSet and closeSet
    if (openSet.size() > 0)
        for (Node* eachOpenNode : openSet) 
        {
            // For each open node, check if it is path, start, or goal, if true, no need to erase pointer
            SquareStatus eachNodeStatus = eachOpenNode->data->getSquareStatus();
            if (eachNodeStatus == Start || eachNodeStatus == Goal || eachNodeStatus == Path) continue;

            eachOpenNode = nullptr;
        }

    if(closeSet.size() > 0)
        for (Node* eachOpenNode : closeSet)
        {
            // For each open node, check if it is path, start, or goal, if true, no need to erase pointer
            SquareStatus eachNodeStatus = eachOpenNode->data->getSquareStatus();
            if (eachNodeStatus == Start || eachNodeStatus == Goal || eachNodeStatus == Path) continue;

            eachOpenNode = nullptr;
        }

    openSet.clear();
    closeSet.clear();
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

    // if there is no node in openset
    if (openSetSize == 0)
    {
        std::cout << "OpenSet Empty " << std::endl;
        return nullptr;
    }

    // If there is only one node in openset, just return that
    if (openSetSize == 1)
    {
        std::cout << "OpenSet Only Has One " << std::endl;
        return openSet.at(0);
    }
    std::cout << "============================== " << std::endl;
    std::cout << "============================== " << std::endl;
    std::cout << "============================== " << std::endl;
    std::cout << "OpenSet Has More Than One Node " << std::endl;

    // if there are more than 1 node in openSet
    SquareKey startKey = startSquareData->getKey();
    SquareKey goalKey = goalSquareData->getKey();

    Node* bestNode = openSet.at(0);
    SquareKey bestNodeKey = bestNode->data->getKey();
    // G: distance from Node to the Start
    int bestNodeG = bestNodeKey.findDistance(startKey);
    // H: distance from Node to the Goal
    int bestNodeH = bestNodeKey.findDistance(goalKey);

    int bestNodeSum = bestNodeG + bestNodeH;


    std::cout << "Start to find best node " << std::endl;
    std::cout << "============================== " << std::endl;
    for (int i = 1; i < openSetSize; i++)
    {
        std::cout << "Iteration: " << i << std::endl;
        std::cout << "============================== " << std::endl;
        std::cout << "Current BestNode: X: [" << bestNode->data->getKey().getRow() << "]  Y: [" << bestNode->data->getKey().getColumn()<< "]  H: [" << bestNodeH << "]  Sum: [" << bestNodeSum << "]" << std::endl;
        Node* iteratingNode = openSet.at(i);
        SquareKey iterateKey = iteratingNode->data->getKey();

        // G: distance from Node to the Start
        const int iterateG = iterateKey.findDistance(startKey);
        // H: distance from Node to the Goal
        const int iterateH = iterateKey.findDistance(goalKey);
        const int iterateSum = iterateG + iterateH;

        std::cout << "Current IterNode: X: [" << iterateKey.getRow() << "]  Y: [" << iterateKey.getColumn() << "]  H: [" << iterateH << "]  Sum: [" << iterateSum << "]" << std::endl;

        

        // if sum is bigger than best sum, continue to next iteration
        if (iterateSum > bestNodeSum)
        {
            std::cout << "Not BestNode" << std::endl;

            continue;
        }

        // if sums are the same
        else if (iterateSum == bestNodeSum)
        {
            std::cout << "Sum Same" << std::endl;

            // check if H value is smaller, if small, iterate node becomes the best node
            if (iterateH < bestNodeH)
            {
                std::cout << "Become BestNode Due to smaller H" << std::endl;

                bestNode = openSet.at(i);
                bestNodeH = iterateH;
                bestNodeSum = iterateSum;
            }
            // if H value is same, compare both Node's path steps
            else if (iterateH == bestNodeH)
            {
                int iterateNodeStep = iteratingNode->getSteps();
                int bestNodeStep = bestNode->getSteps();

                if (iterateNodeStep < bestNodeStep)
                {
                    std::cout << "Become BestNode Due to smaller steps" << std::endl;
                    bestNode = openSet.at(i);
                    bestNodeH = iterateH;
                    bestNodeSum = iterateSum;
                }
            }
        }
        else
        {
            std::cout << "Become BestNode Due to smaller sum" << std::endl;
            // if iterate sum is smaller than best sum, iterate node becomes the best node
            bestNode = openSet.at(i);
            bestNodeH = iterateH;
            bestNodeSum = iterateSum;
        }
    }

    std::cout << "Iteration Finish " << std::endl;
    std::cout << "============================== " << std::endl;
    std::cout << "Current BestNode: X: [" << bestNode->data->getKey().getRow() << "]  Y: [" << bestNode->data->getKey().getColumn() << "]  H: [" << bestNodeH << "]  Sum: [" << bestNodeSum << "]" << std::endl;

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
    maxWallNum = (maxColumn * maxRow) / 5;
    currentWallNum = 0;
    initializeGridTable();
}


GridTable::~GridTable()
{
    //gridData.clear();
}
