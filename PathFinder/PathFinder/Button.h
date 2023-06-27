// Copyright © 2022 Jim Chen, All Rights Reserved

#pragma once
#include <iostream>
#include "SFML\System.hpp"
#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp"
#include "SFML\Audio.hpp"

// Button enum to determine button color
enum ButtonState
{
	Idle,
	Hover,
	Pressed,
};

// Button enum to determine button color
enum ButtonSelectState 
{
	Unselected,
	Selected
};


class Button
{
private:

	// Two states to determine button's color
	ButtonState currentButtonState = Idle;
	ButtonSelectState currentSelectingState = Unselected;

	// Shape and text variable
	sf::RectangleShape shape;
	sf::Text buttonText;
	
	// colors
	sf::Color idleColor;
	sf::Color hoverColor;
	sf::Color activeColor;
	sf::Color selectedColor;


protected:



public:

	// button public functions
	void deactivateButton();
	void updateButtonColor();

	// render and update function
	void render(sf::RenderWindow* target);
	bool update(const sf::Vector2f mousePos, sf::Event* currentGameEvent);

	// deleted default construcotr
	Button() = delete;

	// constructor with necessary variables
	Button(float x, float y, float width, float height, int fontSize, sf::Font &loadedFont, std::string buttonString);
	~Button();

};

