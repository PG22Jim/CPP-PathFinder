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
            
            SquareKey newDataKey = newData->getKey();
            gridData.insert({ newDataKey , newData});

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


    Node* iterateNode = pathToGoal;
    Node* clearingNode = nullptr;
    while (iterateNode)
    {
        clearingNode = iterateNode;

        // while loop to clear path
        SquareData* currentData = iterateNode->getSquareData();
        if (currentData->getSquareStatus() == Path) currentData->setSquareStatus(Empty);

        iterateNode = iterateNode->getParentNode();
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
        // get exploring node from openset
        // this will be iteration node
        Node* nextExploreNode = findNextExploreNode();
        if (nextExploreNode) 
        {
            removeOpenSet(nextExploreNode);
            addNewCloseSet(nextExploreNode);


            // find all walkable neighbors around nextExploreNode
            std::vector<Node*> newExplorableNodes = getValidNeighborNodes(nextExploreNode);

            // if there are neighbors
            if (newExplorableNodes.size() > 0)
            {
                // Check if new return new Explorable Nodes has node that is goal
                for (Node* eachNewNode : newExplorableNodes)
                {
                    // If status is goal
                    SquareStatus eachStatus = eachNewNode->getSquareData()->getSquareStatus();
                    if (eachStatus == SquareStatus::Goal) 
                    {
                        // assign it as pathToGoal, remove from openSet, then clear openSet
                        pathToGoal = eachNewNode;
                        removeOpenSet(pathToGoal);
                        allocatePathToGoal();
                        return ErrorType::None;
                    }

                    // since it is not goal, add into openset
                    addNewOpenSet(eachNewNode);
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

        // if square is explorable and is not same square to exploring node
        if (bCanExplore)
        {
            // make it as new node with calculate h cost, then add to the openSet

            int checkingHCost = checkingSquare->getKey().findDistance(getGoalSquareData()->getKey());

            returnNode = getUpdatedCostNode(checkingSquare, originalNode);
        }
    }

    return returnNode;
}

Node* GridTable::getUpdatedCostNode(SquareData* checkingSquareData, Node* currentExploredNode)
{
    // check if neighbor square is already in open set
    SquareKey checkingKey = checkingSquareData->getKey();
    auto foundItem = openSet.find(checkingKey);
    if (foundItem != openSet.end())
    {
        // if neighbor is in openset
        Node* neighborInOpenSet = foundItem->second;
        int neighborInOpenSetGCost = neighborInOpenSet->getGCost();

        // check if current neighbor node in openset has smaller G value to G value from path of currentExploredNode
        int tentativeGCost = currentExploredNode->getGCost() + 1;
        if (neighborInOpenSetGCost < tentativeGCost) 
        {
            // If true, it mean current neighbor's path is shortest path
            return neighborInOpenSet;
        }

        // if tentative G cost is smaller, it mean currentExplored path is shorter path, overwrite G, H, and parentNode of it
        neighborInOpenSet->setGCost(tentativeGCost);
        neighborInOpenSet->setHCost(checkingKey.findDistance(goalSquareData->getKey()));
        neighborInOpenSet->setParentNode(currentExploredNode);
        return neighborInOpenSet;
        
    }

    // if neighbor is not in openset, create it and add into openset
    const int neighborHcost = checkingKey.findDistance(currentExploredNode->getSquareData()->getKey());
    return new Node(checkingSquareData, currentExploredNode, neighborHcost);
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
        for (auto eachCloseSetItem : closeSet)
        {
            // If key is same, squareData cannot be explore
            SquareKey closeSetKey = eachCloseSetItem.second->getSquareData()->getKey();
            bool isKeySame = checkingSquareKey.operator==(closeSetKey);
            if (isKeySame) 
            {
                return false;
            }            
        }
    }


    // if checking square is not in closeSet, not in openSet, and is not start, wall, or goal, it means it is exploreable
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
            if (eachNodeStatus != OpenSet) continue;

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
            if (eachNodeStatus != OpenSet) continue;

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
            iterateData->setSquareStatus(SquareStatus::OpenSet);
        }
    }

    for (auto itemInCloseSet : closeSet)
    {
        Node* eachCloseNode = itemInCloseSet.second;
        SquareData* iterateData = eachCloseNode->getSquareData();
        SquareStatus iterateStatus = iterateData->getSquareStatus();
        if (iterateStatus == SquareStatus::Empty)
        {
            iterateData->setSquareStatus(SquareStatus::OpenSet);
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

    // If there is only one node in openset, just return that
    if (openSetSize == 1)
    {
        return openSet.begin()->second;
    }


    // if there are more than 1 node in openSet

    // TODO: Use quickSort instead
    SquareKey startKey = startSquareData->getKey();
    SquareKey goalKey = goalSquareData->getKey();


    Node* bestNode = nullptr;
    // G: actual distance from Node to the Start
    int bestNodeG;
    // H: distance from Node to the Goal
    int bestNodeH;
    int bestNodeSum;

    for(auto itemInOpenSet : openSet )
    {
        Node* iteratingNode = itemInOpenSet.second;

        if (!bestNode) 
        {
            bestNode = iteratingNode;
            SquareKey bestNodeKey = bestNode->getSquareData()->getKey();
            // G: actual distance from Node to the Start
            bestNodeG = bestNodeKey.findDistance(startKey);
            // H: distance from Node to the Goal
            bestNodeH = bestNodeKey.findDistance(goalKey);
            bestNodeSum = bestNodeG + bestNodeH;
        
            continue;
        }

        SquareKey iterateKey = iteratingNode->getSquareData()->getKey();

        // G: distance from Node to the Start
        const int iterateG = iterateKey.findDistance(startKey);
        // H: distance from Node to the Goal
        const int iterateH = iterateKey.findDistance(goalKey);
        const int iterateSum = iterateG + iterateH;


        

        // if sum is bigger than best sum, continue to next iteration
        if (iterateSum > bestNodeSum)
        {
            continue;
        }

        // if sums are the same
        else if (iterateSum == bestNodeSum)
        {

            // check if H value is smaller, if small, iterate node becomes the best node
            if (iterateH < bestNodeH)
            {

                bestNode = iteratingNode;
                bestNodeH = iterateH;
                bestNodeSum = iterateSum;
            }
            // if H value is same, compare both Node's path steps
            else if (iterateH == bestNodeH)
            {
                int iterateNodeStep = iteratingNode->getGCost();
                int bestNodeStep = bestNode->getGCost();

                if (iterateNodeStep < bestNodeStep)
                {
                    bestNode = iteratingNode;
                    bestNodeH = iterateH;
                    bestNodeSum = iterateSum;
                }
            }
        }
        else
        {
            // if iterate sum is smaller than best sum, iterate node becomes the best node
            bestNode = iteratingNode;
            bestNodeH = iterateH;
            bestNodeSum = iterateSum;
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
