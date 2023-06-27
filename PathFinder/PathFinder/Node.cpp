// Copyright © 2022 Jim Chen, All Rights Reserved

#include "Node.h"
#include <iostream>

void Node::resetNode()
{
    gCost = 99;
    hCost = 99;
    parentNode = nullptr;
}

float Node::getSum(FCostType calculationType) const
{
	// Normal A* F Cost Calculation
	if (calculationType == Classic) return gCost + hCost;

	// adding weight when calculate F cost will make A* faster (Weighted A*)
	if (calculationType == DynamicWeighted) return gCost + (WEIGHT * hCost);

	// Weighted A* (XUP) Convex Upward Parabola / increase the weight as we are near the goal
	if (calculationType == ConvexUpwardParabola) return (1.0f / 2.0f * WEIGHT) * (gCost + hCost + std::sqrt(std::powf((gCost + hCost), 2.0f) + 4.0f * WEIGHT * (WEIGHT - 1) * std::powf(hCost, 2.0f)));

	// Weighted A* (pwXD) Piecewise Downward curve / decreases the weight as we are near the goal
	return hCost > gCost ? gCost + hCost : (gCost + (2.0f * WEIGHT - 1.0f) * hCost) / WEIGHT;
}

Node::~Node()
{

}

