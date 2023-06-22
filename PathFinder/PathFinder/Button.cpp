#include "Button.h"

void Button::onButtonActivate()
{
	std::cout << " Father Class Button Pressed " << std::endl;
}

void Button::render(sf::RenderWindow* target)
{
	target->draw(shape);
	target->draw(buttonText);
}

bool Button::update(const sf::Vector2f mousePos, sf::Event* currentGameEvent)
{
	bool bButtonActivated = false;

	if (shape.getGlobalBounds().contains(mousePos)) 
	{

		currentButtonState = Hover;
	
		if (currentGameEvent->type == sf::Event::MouseButtonPressed)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				currentButtonState = Pressed;

				if (currentSelectingState == Unselected)
				{
					currentSelectingState = Selected;
					return true;
				}
				else
				{
					currentSelectingState = Unselected;
				}

				
			}
		}
	}
	else
	{
		currentButtonState = Idle;
	}

	updateButtonColor();

	return false;
}

void Button::updateButtonColor()
{
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
	shape.setPosition(pos);
	shape.setSize(sf::Vector2f(width, height));
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(5);

	buttonText.setFont(loadedFont);
	buttonText.setString(buttonString);
	buttonText.setFillColor(sf::Color::Black);
	buttonText.setCharacterSize(12);
	buttonText.setPosition((pos.x + width / 2.0f) - (buttonText.getLocalBounds().width / 2), (pos.y + height / 2.0f) - (buttonText.getLocalBounds().height / 2));


	idleColor = sf::Color(195, 195, 195);
	hoverColor = sf::Color::Green;
	activeColor = sf::Color(255, 127, 0);
	selectedColor = sf::Color::Red;
	
	shape.setFillColor(idleColor);

}

Button::~Button()
{
}
