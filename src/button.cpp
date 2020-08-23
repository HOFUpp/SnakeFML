#include <SFML/Graphics.hpp>
#include <string>

class Button
{
public:
	Button(std::string, int, int, sf::Vector2f);
	void setMessage(std::string);
	void setDimensions(int, int);
	void checkInput();
	void drawButton(sf::RenderWindow&);
	
private:
	sf::Font font;
	std::string message = "error";
	sf::Text text_essage;
	sf::RectangleShape drawingButton;
	
};

Button::Button(std::string str, int x, int y, sf::Vector2f pos)
{
	drawingButton.setSize(sf::Vector2f(x, y));
	drawingBlock.setPosition(pos);
	message = str;
}

void Button::setMessage(std::string str)
{
	message = str;
}

void Button::setDimensions(int x, int y)
{
	drawingBlock.setSize(sf::Vector2f(x, y));
}

void Button::drawButton(sf::RenderWindow& win)
{
	win.draw(RectangleShape);
}