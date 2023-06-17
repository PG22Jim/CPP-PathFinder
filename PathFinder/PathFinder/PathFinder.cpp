// PathFinder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <GLFW/glfw3.h>
#include "GridTable.h"

const int WIDTH = 800;      // Window width
const int HEIGHT = 600;     // Window height
const int GRID_ROWS = 10;   // Number of grid rows
const int GRID_COLUMNS = 10; // Number of grid columns

void drawGrid(GridTable* gridTable)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

    float squareSizeX = static_cast<float>(WIDTH) / GRID_COLUMNS;
    float squareSizeY = static_cast<float>(HEIGHT) / GRID_ROWS;


    int listSize = GRID_COLUMNS * GRID_ROWS;

    for (const auto& eachMapObject : gridTable->gridData)
    {
        SquareData* iteratingSquare = eachMapObject.second;

        int column = iteratingSquare->getPositionValue(true);
        int row = iteratingSquare->getPositionValue(false);
        SquareStatus status = iteratingSquare->getSquareStatus();

        float x = row * squareSizeX;
        float y = column * squareSizeY;



        if (status == SquareStatus::Empty)
            glColor3f(1.0f, 1.0f, 1.0f); // draw white as empty space
        else if (status == SquareStatus::Start)
        {
            glColor3f(0.0f, 1.0f, 0.0f); // draw green square as start
            std::cout << " column: " << column << "  row: " << row << std::endl;
        }
        else if (status == SquareStatus::Goal)
        {
            glColor3f(0.0f, 0.0f, 1.0f); // draw blue square as goal
            std::cout << " column: " << column << "  row: " << row << std::endl;
        }
        else
            glColor3f(1.0f, 0.0f, 0.0f); // draw red square as wall


        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + squareSizeX, y);
        glVertex2f(x + squareSizeX, y + squareSizeY);
        glVertex2f(x, y + squareSizeY);
        glEnd();

    }




    //for (int a = 0; a < listSize; a++)
    //{
    //    SquareData* iteratingSquare = dataList.at(a);
    //    Key iteratingKey = iteratingSquare->getKey();
    //    int column = iteratingKey.getColumn();
    //    int row = iteratingKey.getRow();
    //    SquareStatus status = iteratingSquare->getSquareStatus();

    //    float x = row * squareSizeX;
    //    float y = column * squareSizeY;



    //    if (status == SquareStatus::Empty)
    //        glColor3f(1.0f, 1.0f, 1.0f); // draw white as empty space
    //    else if (status == SquareStatus::Start)
    //    {
    //        glColor3f(0.0f, 1.0f, 0.0f); // draw green square as start
    //        std::cout << " column: " << column << "  row: " << row << std::endl;
    //    }
    //    else if (status == SquareStatus::Goal)
    //    {
    //        glColor3f(0.0f, 0.0f, 1.0f); // draw blue square as goal
    //        std::cout << " column: " << column << "  row: " << row << std::endl;
    //    }
    //    else
    //        glColor3f(1.0f, 0.0f, 0.0f); // draw red square as wall


    //    glBegin(GL_QUADS);
    //    glVertex2f(x, y);
    //    glVertex2f(x + squareSizeX, y);
    //    glVertex2f(x + squareSizeX, y + squareSizeY);
    //    glVertex2f(x, y + squareSizeY);
    //    glEnd();
    //}
}

int main()
{
    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Path Finder", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GridTable* gridTable = new GridTable();

    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white
        glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer


        drawGrid(gridTable);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
