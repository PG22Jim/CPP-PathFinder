#pragma once
#include <iostream>
#include "SFML\System.hpp"
#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp"
#include "SFML\Audio.hpp"


enum ButtonState
{
	Idle,
	Hover,
	Pressed,
};

enum ButtonSelectState 
{
	Unselected,
	Selected
};


class Button
{
private:

	ButtonState currentButtonState = Idle;
	ButtonSelectState currentSelectingState = Unselected;

	sf::RectangleShape shape;
	sf::Font* font;
	sf::Text buttonText;
	
	sf::Color idleColor;
	sf::Color hoverColor;
	sf::Color activeColor;
	sf::Color selectedColor;


protected:



public:

	void onButtonActivate();




	void render(sf::RenderWindow* target);
	bool update(const sf::Vector2f mousePos, sf::Event* currentGameEvent);
	void updateButtonColor();


	Button() = delete;
	Button(float x, float y, float width, float height, int fontSize, sf::Font &loadedFont, std::string buttonString);
	~Button();

};

