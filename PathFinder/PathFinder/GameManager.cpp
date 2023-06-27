// Copyright © 2022 Jim Chen, All Rights Reserved

#include "GameManager.h"

void GameManager::Tick()
{
    pollEvents();
    renderScreen();
}

void GameManager::initializeMessageBoard(sf::Font& loadedFont)
{
    sf::Vector2f messagePos = sf::Vector2f(40.0f, 680.0f);
    messageboardShape.setPosition(messagePos);
    messageboardShape.setSize(sf::Vector2f(600.0f, 160.0f));
    messageboardShape.setFillColor(sf::Color(195, 195, 195));
    messageboardShape.setOutlineColor(sf::Color::Black);
    messageboardShape.setOutlineThickness(5);

    messageText.setFont(loadedFont);
    messageText.setString(WELCOMEMESSAGE);
    messageText.setFillColor(sf::Color::Black);
    messageText.setCharacterSize(16);
    messageText.setPosition(messagePos.x + 10, messagePos.y + 10);
}

void GameManager::initializeButtons(sf::Font& loadedFont)
{
    selectEmptyButton = new Button(680.0f, 40.0f, 200.0f, 120.0f, 16, loadedFont, std::string("SELECT EMPTY"));
    selectStartButton = new Button(680.0f, 200.0f, 200.0f, 120.0f, 16, loadedFont, std::string("SELECT START"));
    selectGoalButton = new Button(920.0f, 200.0f, 200.0f, 120.0f, 16, loadedFont, std::string("SELECT GOAL"));
    selectWallButton = new Button(920.0f, 40.0f, 200.0f, 120.0f, 16, loadedFont, std::string("SELECT WALL"));

    selectDiagonalButton = new Button(920.0f, 360.0f, 200.0f, 120.0f, 16, loadedFont, std::string("DIAGONAL ALSO"));
    selectNeighborOnlyButton = new Button(680.0f, 360.0f, 200.0f, 120.0f, 16, loadedFont, std::string("NEIGHBOR ONLY"));

    selectClassicalFCostButton = new Button(680.0f, 520, 80.0f, 80.0f, 16, loadedFont, std::string("Normal F"));
    selectWeightedFCostButton = new Button(800.0f, 520, 80.0f, 80.0f, 16, loadedFont, std::string("Weighted"));
    selectPiecewiseDownwardCurveButton = new Button(920.0f, 520, 80.0f, 80.0f, 16, loadedFont, std::string("PWDW Curve"));
    selectConvexUpwardParabolaButton = new Button(1040.0f, 520, 80.0f, 80.0f, 16, loadedFont, std::string("XUP"));

    startPathFindButton = new Button(680.0f, 640, 200.0f, 120.0f, 16, loadedFont, std::string("START"));
    cancelPathFindButton = new Button(920.0f, 640.0f, 200.0f, 120.0f, 16, loadedFont, std::string("CANCEL"));
}

void GameManager::updateButton(ButtonType selectedType, Button* selectedButton)
{
    if (selectedType == StatusSelect)
    {
        onCancelButton(selectedButton, selectEmptyButton);
        onCancelButton(selectedButton, selectStartButton);
        onCancelButton(selectedButton, selectGoalButton);
        onCancelButton(selectedButton, selectWallButton);
        return;
    }
    if (selectedType == PathMovement) 
    {
        onCancelButton(selectedButton, selectDiagonalButton);
        onCancelButton(selectedButton, selectNeighborOnlyButton);
        return;
    }
    if (selectedType == FCostCalculation) 
    {
        onCancelButton(selectedButton, selectClassicalFCostButton);
        onCancelButton(selectedButton, selectWeightedFCostButton);
        onCancelButton(selectedButton, selectPiecewiseDownwardCurveButton);
        onCancelButton(selectedButton, selectConvexUpwardParabolaButton);
        return;
    }

    onCancelButton(selectedButton, startPathFindButton);
    onCancelButton(selectedButton, cancelPathFindButton);


}

void GameManager::onCancelButton(Button* selectedButton, Button* cancelingButton)
{
    if (selectedButton == cancelingButton) return;
    cancelingButton->deactivateButton();
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
            // if button is left, left key holdiing will become true
            if (gameEvent.mouseButton.button == sf::Mouse::Left)
            {
                leftKeyHolding = true;
                rightKeyHolding = false;
            }
            if (gameEvent.mouseButton.button == sf::Mouse::Right)
            {
                leftKeyHolding = false;
                rightKeyHolding = true;
            }
        }

        // Pass mouse position and mouse button event to Grid manager if right or left mouse key is holding
        if(leftKeyHolding || rightKeyHolding) gridManager->OnReceiveUserMouseButton(mousePosition, gameEvent.mouseButton.button, leftKeyHolding, rightKeyHolding);

        // if button left or right is released, make bool false
        if (gameEvent.type == sf::Event::MouseButtonReleased)
        {
            if (gameEvent.mouseButton.button == sf::Mouse::Left) leftKeyHolding = false;
            if (gameEvent.mouseButton.button == sf::Mouse::Right) rightKeyHolding = false;
        }

        // if start or cancel path finding button is pressed, update button and change message
        if (startPathFindButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            updateButton(StartCancel, startPathFindButton);
            messageText.setString(gridManager->onUpdatePath());
        }

        if (cancelPathFindButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            updateButton(StartCancel, cancelPathFindButton);
            gridManager->getGridTable()->clearExistingPath();
        }

        // update other button if specific button is pressed and execute its function
        if (selectEmptyButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            gridManager->currentRequestStatus = SquareStatus::Empty;
            updateButton(StatusSelect, selectEmptyButton);
        }
        if (selectStartButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            gridManager->currentRequestStatus = SquareStatus::Start;
            updateButton(StatusSelect, selectStartButton);
        }

        if (selectGoalButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            gridManager->currentRequestStatus = SquareStatus::Goal;
            updateButton(StatusSelect, selectGoalButton);
        }

        if (selectWallButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            gridManager->currentRequestStatus = SquareStatus::Wall;
            updateButton(StatusSelect, selectWallButton);
        }

        if (selectClassicalFCostButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            const bool bSuccessChange = gridManager->setFCostType(Classic);
            if (bSuccessChange)
            {
                updateButton(FCostCalculation, selectClassicalFCostButton);
                if(gridManager->getGridTable()->getPathToGoal()) messageText.setString(gridManager->onUpdatePath());
            }
        }

        if (selectWeightedFCostButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            const bool bSuccessChange = gridManager->setFCostType(DynamicWeighted);
            if (bSuccessChange)
            {
                updateButton(FCostCalculation, selectWeightedFCostButton);
                if (gridManager->getGridTable()->getPathToGoal()) messageText.setString(gridManager->onUpdatePath());
            }
        }

        if (selectPiecewiseDownwardCurveButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            const bool bSuccessChange = gridManager->setFCostType(PiecewiseDownwardCurve);
            if (bSuccessChange)
            {
                updateButton(FCostCalculation, selectPiecewiseDownwardCurveButton);
                if (gridManager->getGridTable()->getPathToGoal()) messageText.setString(gridManager->onUpdatePath());
            }
        }

        if (selectConvexUpwardParabolaButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            const bool bSuccessChange = gridManager->setFCostType(ConvexUpwardParabola);
            if (bSuccessChange)
            {
                updateButton(FCostCalculation, selectConvexUpwardParabolaButton);
                if (gridManager->getGridTable()->getPathToGoal()) messageText.setString(gridManager->onUpdatePath());
            }
        }

        if (selectDiagonalButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            const bool bSuccessChange = gridManager->setGridMovement(DiagonalAlso);
            if (bSuccessChange)
            {
                updateButton(PathMovement, selectDiagonalButton);
                if (gridManager->getGridTable()->getPathToGoal()) messageText.setString(gridManager->onUpdatePath());
            }
        }

        if (selectNeighborOnlyButton->update(sf::Vector2f(mousePosition.x, mousePosition.y), &gameEvent))
        {
            const bool bSuccessChange = gridManager->setGridMovement(NeighborOnly);
            if (bSuccessChange)
            {
                updateButton(PathMovement, selectNeighborOnlyButton);
                if (gridManager->getGridTable()->getPathToGoal()) messageText.setString(gridManager->onUpdatePath());
            }
        }

        // if player press esc, close window
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
    selectWallButton->render(renderWindow);
    selectDiagonalButton->render(renderWindow);
    selectNeighborOnlyButton->render(renderWindow);

    selectClassicalFCostButton->render(renderWindow);
    selectWeightedFCostButton->render(renderWindow);
    selectPiecewiseDownwardCurveButton->render(renderWindow);
    selectConvexUpwardParabolaButton->render(renderWindow);

    selectStartButton->render(renderWindow);
    selectGoalButton->render(renderWindow);


    renderWindow->draw(messageboardShape);
    renderWindow->draw(messageText);

    // display window
    renderWindow->display();
}

GameManager::GameManager(int width, int height)
{
	renderWindow = new sf::RenderWindow(sf::VideoMode(width, height), "Path Finder");
	gridManager = new GridManager();

    // Create background image
    backGround.setPosition(sf::Vector2f(0, 0));
    backGround.setSize(sf::Vector2f(width, height));
    backGround.setFillColor(sf::Color::White);

    // Create Buttons
    buttonFont.loadFromFile("Font/Franklin.otf");


    initializeMessageBoard(buttonFont);
    initializeButtons(buttonFont);
}

GameManager::~GameManager()
{
	delete renderWindow;
	delete gridManager;
}
