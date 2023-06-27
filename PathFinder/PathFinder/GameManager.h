// Copyright © 2022 Jim Chen, All Rights Reserved

#pragma once
#include <iostream>
#include "GridManager.h"
#include "Button.h"



const int APPVERSION = 1;

const sf::String WELCOMEMESSAGE = "Welcome To PathFinder V." + std::to_string(APPVERSION) + ".\nYou can press buttons on right-side to set up a maze. \nYou can pick different heuristic function and different movement";




enum ButtonType
{
    StatusSelect,
    PathMovement,
    FCostCalculation,
    StartCancel,
};


class GameManager
{

private:

    sf::RectangleShape backGround;

    // message board variable
    sf::RectangleShape messageboardShape;
    sf::Text messageText;

    sf::Font buttonFont;

    // buttons
    Button* selectEmptyButton;
    Button* selectStartButton;
    Button* selectGoalButton;
    Button* selectWallButton;

    Button* selectDiagonalButton;
    Button* selectNeighborOnlyButton;

    Button* selectClassicalFCostButton;
    Button* selectWeightedFCostButton;
    Button* selectPiecewiseDownwardCurveButton;
    Button* selectConvexUpwardParabolaButton;

    Button* startPathFindButton;
    Button* cancelPathFindButton;

    sf::Event gameEvent;

    GridManager* gridManager;
    sf::RenderWindow* renderWindow;

    bool leftKeyHolding = false;
    bool rightKeyHolding = false;

    // initialization functions
    void initializeMessageBoard(sf::Font& loadedFont);
    void initializeButtons(sf::Font& loadedFont);


    void updateButton(ButtonType selectedType, Button* selectedButton);
    void onCancelButton(Button* selectedButton, Button* cancelingButton);

    void pollEvents();
    void renderScreen();

public:

    bool isGameRunning() const { return renderWindow->isOpen(); }

    void Tick();

    // constructor, destructor
    GameManager() = delete;
    GameManager(int width, int height);
    ~GameManager();


};

