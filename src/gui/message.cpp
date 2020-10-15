#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Message
{
public:
	Message();
	Message(std::string, float);
	void initMessage(std::string, float);
	void setCharacterSize(int);
	void setString(std::string);
	void setPosition(float, float);
	void setPosition(sf::Vector2f);
	void move(float, float);
	void setOriginCenter();
	void changeFont();
	sf::FloatRect getLocalBounds(); 
	void draw(sf::RenderWindow&);

private:
	sf::Font font;
	sf::Text text_message;
	
};

Message::Message()
{
	font.loadFromFile("font/8-bitArcadeIn.ttf");
}

Message::Message(std::string text, float size)
{
	font.loadFromFile("font/8-bitArcadeIn.ttf");
	text_message.setFont(font);
	text_message.setString(text);
	text_message.setCharacterSize(size);
}

void Message::initMessage(std::string text, float size)
{
	text_message.setFont(font);
	text_message.setString(text);
	text_message.setCharacterSize(size);
}

void Message::changeFont()
{
	font.loadFromFile("font/04B_30__.TTF");
	text_message.setFont(font);
}

void Message::setCharacterSize(int size)
{
	text_message.setCharacterSize(size);
}

void Message::setString(std::string text)
{
	text_message.setString(text);
}

void Message::setPosition(float x, float y)
{
	text_message.setPosition(x, y);
}

void Message::move(float x, float y)
{
	text_message.move(x, y);
}

void Message::setOriginCenter()
{
	sf::FloatRect textRect;
	textRect = text_message.getLocalBounds();
	text_message.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
}

sf::FloatRect Message::getLocalBounds()
{
	return text_message.getLocalBounds();
}

void Message::draw(sf::RenderWindow& win)
{
	win.draw(text_message);
}
