#include "GridTable.h"


void GridTable::initializeGridTable()
{
    const int maxColumn = 10;
    const int maxRow = 10;

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

            SquareList.push_back(newData);

            iterationIndex++;
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
    return dataA->getColumnIndex() == dataB->getColumnIndex() && dataA->getRowIndex() == dataB->getRowIndex();
}

SquareData* GridTable::GetSquareData(int requestColumn, int requestRow)
{

    for (int i = 0; i < squareListSize; ++i)
    {
        int column = SquareList.at(i)->getColumnIndex();
        int row = SquareList.at(i)->getRowIndex();

        if (column == requestColumn && row == requestRow)
        {
            return SquareList[i];
        }
    }

    return nullptr;
}

void GridTable::getPathToGoal()
{
    openSet.clear();
    closeSet.clear();

    Node* currentNode = new Node(startSquareData);
    addNewOpenSet(currentNode);

    while (!openSet.empty())
    {
        // Get list of valid point to explore
        std::vector<Node*> exploringNode = exploreNode(currentNode);

        // If return empty list, means this explore is not valid
        //if(exploringNode )

        for (Node* eachNode : exploringNode)
        {
            openSet.push_back(eachNode);
            removeOpenSet(currentNode);
            addNewCloseSet(currentNode);
        }

    }


}

std::vector<Node*> GridTable::exploreNode(Node* exploringNode)
{
    std::vector<Node*> returnVector;

    SquareData* exploringSquareData = exploringNode->data;
    int exploreColumn = exploringSquareData->getColumnIndex();
    int exploreRow = exploringSquareData->getRowIndex();

    Node* upNode;
    if ((exploreColumn - 1) >= 0 && (exploreColumn - 1) <= 9)
    {


    }



    return returnVector;
}

bool GridTable::canExploreThisNode(Node* checkingNode)
{
    return false;
}

Node* GridTable::findNextExploreNode()
{
    return nullptr;
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


GridTable::~GridTable()
{
    SquareList.clear();
}
