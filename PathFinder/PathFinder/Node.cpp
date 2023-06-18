#include "Node.h"
#include <iostream>

int Node::calculatePathSteps()
{
    int iterateNodeStep = 0;

    Node* stepCountingNode = this;
    while (stepCountingNode)
    {
        stepCountingNode = stepCountingNode->getParentNode();
        iterateNodeStep++;
    }
    return iterateNodeStep;
}