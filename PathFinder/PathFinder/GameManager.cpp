#include "GameManager.h"

void GameManager::Tick()
{
    pollEvents();
    renderScreen();
}

void GameManager::pollEvents()
{
	while (renderWindow->pollEvent(gameEvent))
	{
        // Get the mouse position relative to the window
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*renderWindow);
        mousePosition.x;
        mousePosition.y;

        // Check if the event is a mouse button press
        if (gameEvent.type == sf::Event::MouseButtonPressed)
        {
            // Pass mouse position and mouse button event to Grid manager
            gridManager->OnReceiveUserMouseButton(mousePosition, event.mouseButton.button);
        }

        if (startPathFindButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            std::cout << " Start Update Path " << std::endl;
            gridManager->onUpdatePath();
        }

        if (cancelPathFindButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            std::cout << " Start Clear Path " << std::endl;
            gridManager->onClearExistPath();
        }

        if (selectEmptyButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            gridManager->currentRequestStatus = SquareStatus::Empty;
            std::cout << " Change Request to Empty " << std::endl;
        }
        if (selectStartButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            gridManager->currentRequestStatus = SquareStatus::Start;
            std::cout << " Change Request to Start " << std::endl;
        }

        if (selectGoalButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            gridManager->currentRequestStatus = SquareStatus::Goal;
            std::cout << " Change Request to Goal " << std::endl;
        }

        if (selectWallButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            gridManager->currentRequestStatus = SquareStatus::Wall;
            std::cout << " Change Request to Wall " << std::endl;
        }

        if (selectDiagonalButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            const bool bSuccessChange = gridManager->setGridMovement(DiagonalAlso);
            if (bSuccessChange && gridManager->getGridTable()->getPathToGoal()) gridManager->onUpdatePath();
        }

        if (selectNeighborOnlyButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            const bool bSuccessChange = gridManager->setGridMovement(NeighborOnly);
            if (bSuccessChange && gridManager->getGridTable()->getPathToGoal()) gridManager->onUpdatePath();
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) renderWindow->close();
	}
}

void GameManager::renderScreen()
{
    // Rendering part
    // must clear window first
    renderWindow->clear();

    // draw background
    renderWindow->draw(backGround);
    

    gridManager->renderGrid(renderWindow);


    startPathFindButton->render(renderWindow);
    cancelPathFindButton->render(renderWindow);
    selectEmptyButton->render(renderWindow);
    selectStartButton->render(renderWindow);
    selectGoalButton->render(renderWindow);
    selectWallButton->render(renderWindow);
    selectDiagonalButton->render(renderWindow);
    selectNeighborOnlyButton->render(renderWindow);



    // display window
    renderWindow->display();
}

GameManager::GameManager(int width, int height)
{
	renderWindow = new sf::RenderWindow(sf::VideoMode(width, height), "Path Finder");
	gridManager = new GridManager(GRID_COLUMNS, GRID_ROWS);

    // Create background image
    backGround.setPosition(sf::Vector2f(0, 0));
    backGround.setSize(sf::Vector2f(width, height));
    backGround.setFillColor(sf::Color::White);

    // Create Buttons
    buttonFont.loadFromFile("Font/Franklin.otf");
    // float x, float y, float width, float height, sf::Font , std::string buttonText

    selectEmptyButton = new Button(680.0f, 40.0f, 200.0f, 120.0f, 16, buttonFont, std::string("SELECT EMPTY"));

    selectStartButton = new Button(680.0f, 200.0f, 200.0f, 120.0f, 16, buttonFont, std::string("SELECT START"));

    selectGoalButton = new Button(920.0f, 200.0f, 200.0f, 120.0f, 16, buttonFont, std::string("SELECT GOAL"));

    selectWallButton = new Button(920.0f, 40.0f, 200.0f, 120.0f, 16, buttonFont, std::string("SELECT WALL"));


    selectDiagonalButton = new Button(920.0f, 360.0f, 200.0f, 120.0f, 16, buttonFont, std::string("DIAGONAL ALSO"));

    selectNeighborOnlyButton = new Button(680.0f, 360.0f, 200.0f, 120.0f, 16, buttonFont, std::string("NEIGHBOR ONLY"));


    startPathFindButton = new Button(680.0f, 520, 200.0f, 120.0f, 16, buttonFont, std::string("START"));


    cancelPathFindButton = new Button(920.0f, 520.0f, 200.0f, 120.0f, 16, buttonFont, std::string("CANCEL"));

}

GameManager::~GameManager()
{
	delete renderWindow;
	delete gridManager;
}
