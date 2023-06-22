// PathFinder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "GameManager.h"


const int WIDTH = 1200;      // Window width
const int HEIGHT = 900;     // Window height


const std::string PATHFOUND_MESSAGE = "Path Found!!";

int main()
{
    GameManager* game = new GameManager(WIDTH, HEIGHT);

    while (game->isGameRunning())
    {
        game->Tick();
    }

    return 0;
}

