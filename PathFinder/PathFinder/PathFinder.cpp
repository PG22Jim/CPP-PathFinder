// PathFinder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "GridManager.h"


const int WIDTH = 1200;      // Window width
const int HEIGHT = 900;     // Window height


const std::string PATHFOUND_MESSAGE = "Path Found!! ";


sf::Vector2f getPosition(SquareKey key) 
{
    const int x = SQUARE_SIZE * key.getRow();
    const int y = SQUARE_SIZE * key.getColumn();

    return sf::Vector2f(x, y);
}




int main()
{
    // create window and event
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Path Finder");
    sf::Event event;

    // Create grid table

    GridManager* gridManager = new GridManager(GRID_COLUMNS, GRID_ROWS);
    GridTable* gridTable = gridManager->getGridTable();

    //// if path is nullptr, call tryPathFinding function
    //if (!gridTable->getPathToGoal()) 
    //{
    //    ErrorType resultError = gridTable->tryPathFinding();
    //}


    sf::RectangleShape backGround;
    backGround.setPosition(sf::Vector2f(0, 0));
    backGround.setSize(sf::Vector2f(WIDTH, HEIGHT));
    backGround.setFillColor(sf::Color::White);


    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            // Check if the event is a mouse button press
            if (event.type == sf::Event::MouseButtonPressed)
            {
                // Get the mouse position relative to the window
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                mousePosition.x;
                mousePosition.y;

                // Pass mouse position and mouse button event to Grid manager
                gridManager->OnReceiveUserMouseButton(mousePosition, event.mouseButton.button);
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
                {
                    gridManager->currentRequestStatus = SquareStatus::Empty;
                    std::cout << " Change Request to Empty " << std::endl;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
                {
                    gridManager->currentRequestStatus = SquareStatus::Start;
                    std::cout << " Change Request to Start " << std::endl;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
                {
                    gridManager->currentRequestStatus = SquareStatus::Goal;
                    std::cout << " Change Request to Goal " << std::endl;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
                {
                    gridManager->currentRequestStatus = SquareStatus::Wall;
                    std::cout << " Change Request to Wall " << std::endl;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
                {
                    std::cout << " Start Update Path " << std::endl;
                    gridManager->onUpdatePath();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
                {
                    std::cout << " Start Clear Path " << std::endl;
                    gridManager->onClearExistPath();
                }
            }
            
                


            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }
     

        // Rendering part
        // must clear window first
        window.clear();

        // draw background
        window.draw(backGround);

        // Draw grid
        for (const auto& eachMapObject : gridTable->gridData)
        {
            SquareData* eachData = eachMapObject.second->getSquareData();
            SquareKey eachKey = eachMapObject.first;
            SquareStatus eachStatus = eachData->getSquareStatus();


            if (eachStatus == Empty)
                eachData->updateRectShapeColor(sf::Color::Yellow);
            else if (eachStatus == Wall)
                eachData->updateRectShapeColor(sf::Color::Red);
            else if (eachStatus == Start)
                eachData->updateRectShapeColor(sf::Color::Green);
            else if (eachStatus == Goal)
                eachData->updateRectShapeColor(sf::Color::Blue);
            else if (eachStatus == CalculatedPlace)
                eachData->updateRectShapeColor(sf::Color::Cyan);
            else if (eachStatus == CloseSet)
                eachData->updateRectShapeColor(ORANGE);
            else
            {
                eachData->updateRectShapeColor(PURPLE);
            }

            window.draw(eachData->getShape());
        }


        // display window
        window.display();

    }

    return 0;
}
