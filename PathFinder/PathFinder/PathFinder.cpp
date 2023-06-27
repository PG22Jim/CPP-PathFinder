// PathFinder.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Copyright © 2022 Jim Chen, All Rights Reserved

#include <iostream>
#include "GameManager.h"

const int WIDTH = 1200;      // Window width
const int HEIGHT = 900;     // Window height

int main()
{
    GameManager* game = new GameManager(WIDTH, HEIGHT);

    while (game->isGameRunning())
    {
        game->Tick();
    }

    return 0;
}

