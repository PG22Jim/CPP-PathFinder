// Copyright © 2022 Jim Chen, All Rights Reserved

#include "Button.h"


void Button::deactivateButton()
{
	// reset button states and update its color
	currentButtonState == Idle;
	currentSelectingState = Unselected;
	updateButtonColor();
}

void Button::render(sf::RenderWindow* target)
{
	target->draw(shape);
	target->draw(buttonText);
}

bool Button::update(const sf::Vector2f mousePos, sf::Event* currentGameEvent)
{
	// if mouse is on button
	if (shape.getGlobalBounds().contains(mousePos)) 
	{
		// hover state
		currentButtonState = Hover;
	
		// check if mouse left button is clicked
		if (currentGameEvent->type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			// enter presssed state
			currentButtonState = Pressed;
			// if another state is unselected, enter selected state and return true
			if (currentSelectingState == Unselected)
			{
				currentSelectingState = Selected;
				return true;
			}
			else // if left key pressed on activated button, make it unselected
			{
				currentSelectingState = Unselected;
			}
		}
	}

	// if button is not hovered, button state is idle
	else currentButtonState = Idle;
	
	// update color and return false
	updateButtonColor();
	return false;
}

void Button::updateButtonColor()
{
	// switch case to update color on button states
	switch (currentButtonState)
	{
		case Idle:
			if (currentSelectingState == Unselected) shape.setFillColor(idleColor);
			else shape.setFillColor(selectedColor);

			break;
		case Hover:
			shape.setFillColor(hoverColor);
			break;
		case Pressed:
			shape.setFillColor(activeColor);
			break;
		default:
			break;
	}
}

Button::Button(float x, float y, float width, float height, int fontSize, sf::Font &loadedFont, std::string buttonString)
{
	sf::Vector2f pos = sf::Vector2f(x, y);

	// colors initialization
	idleColor = sf::Color(195, 195, 195);
	hoverColor = sf::Color::Green;
	activeColor = sf::Color(255, 127, 0);
	selectedColor = sf::Color::Red;

	// button shape initialization
	shape.setPosition(pos);
	shape.setSize(sf::Vector2f(width, height));
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(5);
	shape.setFillColor(idleColor);

	// button text initialization
	buttonText.setFont(loadedFont);
	buttonText.setString(buttonString);
	buttonText.setFillColor(sf::Color::Black);
	buttonText.setCharacterSize(12);
	buttonText.setPosition((pos.x + width / 2.0f) - (buttonText.getLocalBounds().width / 2), (pos.y + height / 2.0f) - (buttonText.getLocalBounds().height / 2));

	

}

Button::~Button()
{
}
