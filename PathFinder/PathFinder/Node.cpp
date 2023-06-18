#include "Node.h"
#include <iostream>

int Node::getSteps()
{
    int iterateNodeStep = 0;

    Node* stepCountingNode = this;
    while (stepCountingNode)
    {
        stepCountingNode = stepCountingNode->previousData;
        iterateNodeStep++;
    }
    
    std::cout << "Steps: " << iterateNodeStep << std::endl;

    return iterateNodeStep;
}
