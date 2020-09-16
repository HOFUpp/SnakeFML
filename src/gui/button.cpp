#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "message.cpp"

class Button
{
public:
	Button() {
		message = "error";
	}
	
	Button(std::string, int, int, sf::Vector2f);
	void init(std::string, int, int, sf::Vector2f, float);
	void setMessage(std::string);
	void move(float, float);
	void setPosition(float, float);
	void setDimensions(int, int);
	void setFillColor(sf::Color);
	void checkInput();
	void drawButton(sf::RenderWindow&);

	sf::Vector2f getPosition();
	
private:
	std::string message;
	Message text_message;
	sf::RectangleShape drawingButton;
	
};

Button::Button(std::string string, int x, int y, sf::Vector2f pos)
{
	drawingButton.setSize(sf::Vector2f(x, y));
	drawingButton.setPosition(pos);
	drawingButton.setFillColor(sf::Color(0, 0, 0));
	message = string;
	text_message.initMessage(message, 0);
}

void Button::init(std::string str, int x, int y, sf::Vector2f pos, float text_dim)
{
	drawingButton.setSize(sf::Vector2f(x, y));
	drawingButton.setPosition(pos);
	drawingButton.setFillColor(sf::Color(21, 21, 21));
	message = str;
	text_message.initMessage(message, text_dim);
	text_message.setOriginCenter();
	text_message.setPosition(pos.x + (x/2), pos.y + (y/2));
}

sf::Vector2f Button::getPosition()
{
	return sf::Vector2f(drawingButton.getPosition());
}

void Button::setMessage(std::string string)
{
	message = string;
}

void Button::move(float x, float y)
{
	drawingButton.move(x, y);
	text_message.move(x, y);
}

void Button::setPosition(float x, float y)
{
	drawingButton.setPosition(x, y);
	text_message.setPosition(x + (drawingButton.getSize().x/2), y + (drawingButton.getSize().y/2));
}

void Button::setDimensions(int x, int y)
{
	drawingButton.setSize(sf::Vector2f(x, y));
}

void Button::setFillColor(sf::Color col)
{
	drawingButton.setFillColor(col);
}

void Button::drawButton(sf::RenderWindow& win)
{
	win.draw(drawingButton);
	text_message.draw(win);
}
