// PathFinder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include <GLFW/glfw3.h>
#include "GridTable.h"

#include "SFML/Graphics.hpp"

const int WIDTH = 1200;      // Window width
const int HEIGHT = 900;     // Window height
const int GRID_LENGTH = 800;     // Grid Length
const int GRID_ROWS = 10;   // Number of grid rows
const int GRID_COLUMNS = 10; // Number of grid columns
const int SQUARE_SIZE = GRID_LENGTH / GRID_ROWS;
const float SQUARE_OUTLINE_THICKNESS = 3.0f;

const std::string PATHFOUND_MESSAGE = " Path Found!! ";


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
    GridTable* gridTable = new GridTable(GRID_COLUMNS, GRID_ROWS);

    // if path is nullptr, call tryPathFinding function
    if (!gridTable->getPathToGoal()) 
    {
        gridTable->tryPathFinding();
    }


    sf::RectangleShape rects[GRID_ROWS * GRID_COLUMNS];
    int index = 0;
    for(const auto& eachMapObject : gridTable->gridData)
    {
        SquareKey eachKey = eachMapObject.first;
        SquareStatus eachStatus = eachMapObject.second->getSquareStatus();
        sf::Vector2f rectanglePos(getPosition(eachKey));

        rects[index].setPosition(rectanglePos);
        rects[index].setSize(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));

        if(eachStatus == Empty)
            rects[index].setFillColor(sf::Color::Yellow);
        else if (eachStatus == Wall)
            rects[index].setFillColor(sf::Color::Red);
        else if (eachStatus == Start)
            rects[index].setFillColor(sf::Color::Green);
        else if (eachStatus == Goal)
            rects[index].setFillColor(sf::Color::Blue);
        else
            rects[index].setFillColor(sf::Color::Cyan);

        rects[index].setOutlineColor(sf::Color::Black);
        rects[index].setOutlineThickness(SQUARE_OUTLINE_THICKNESS);

        index++;
    }

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
                // Check if the left mouse button is pressed
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // Get the mouse position relative to the window
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                    // Print mouse position if mouse cursor position is in grid
                    if (GRID_LENGTH > mousePosition.x && GRID_LENGTH > mousePosition.y)
                    {

                        std::cout << "Mouse Position: " << mousePosition.x << ", " << mousePosition.y << std::endl;
                    }
                }
            }


            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }
     

        // Rendering part
        // must clear window first
        window.clear();

        // draw background
        window.draw(backGround);

        for (sf::RectangleShape eachRect : rects)
        {
            window.draw(eachRect);
        }

        // display window
        window.display();

    }


    return 0;


    //if (!glfwInit())
    //    return -1;

    //GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Path Finder", NULL, NULL);
    //if (!window)
    //{
    //    glfwTerminate();
    //    return -1;
    //}

    //glfwMakeContextCurrent(window);

    //GridTable* gridTable = new GridTable(GRID_COLUMNS, GRID_ROWS);

    //// if path is nullptr, call tryPathFinding function
    //if (!gridTable->getPathToGoal()) 
    //{
    //    gridTable->tryPathFinding();
    //}


    //glViewport(0, 0, WIDTH, HEIGHT);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    //while (!glfwWindowShouldClose(window))
    //{
    //    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white
    //    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer


    //    drawGrid(gridTable);

    //    glfwSwapBuffers(window);
    //    glfwPollEvents();
    //}

    //glfwTerminate();
    //return 0;
}
