#pragma once
#include <iostream>
#include "GridManager.h"
#include "Button.h"





class GameManager
{

private:

    sf::RectangleShape backGround;


    sf::Font buttonFont;

    Button* startPathFindButton;
    Button* cancelPathFindButton;
    Button* selectEmptyButton;
    Button* selectStartButton;
    Button* selectGoalButton;
    Button* selectWallButton;
    Button* selectDiagonalButton;
    Button* selectNeighborOnlyButton;


    sf::Event gameEvent;

    GridManager* gridManager;
    sf::RenderWindow* renderWindow;
    sf::Event event;


    void pollEvents();
    void renderScreen();

public:

    bool isGameRunning() const { return renderWindow->isOpen(); }

    void Tick();

    GameManager() = delete;
    GameManager(int width, int height);
    ~GameManager();


};

