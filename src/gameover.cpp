#include <SFML/Graphics.hpp>
#include <string>
#include "gui/button.cpp"

class GameOverMenu
{
public:
	void init(int, int);
	void input();
	void update();
	void draw(sf::RenderWindow&);
	int response();
	
private:
	int dim_x, dim_y;
	Button retryButton;
	Button menuBotton;
	Button exitButton;

	sf::Clock clock_input;
	sf::Time elapsed_input;

	int button_num = 1;
	bool selected = false;

	sf::RectangleShape buttonHighlight;
	sf::RectangleShape background;
	
};

void GameOverMenu::init(int wx, int wy)
{
	dim_x = wx/3;
	dim_y = wy/7;
	retryButton.init("retry", dim_x, dim_y, sf::Vector2f(dim_x, dim_y), wy/9);
	menuBotton.init("menu", dim_x, dim_y, sf::Vector2f(dim_x, dim_y*3), wy/9);
	exitButton.init("exit", dim_x, dim_y, sf::Vector2f(dim_x, dim_y*5), wy/9);

	button_num = 1;
	selected = false;

	buttonHighlight.setSize(sf::Vector2f(dim_x + (dim_y/4), dim_y + (dim_y/4)));
	buttonHighlight.setPosition(sf::Vector2f(dim_x - (dim_y/8), dim_y - (dim_y/8)));
	buttonHighlight.setFillColor(sf::Color(220, 220, 220));

	background.setSize(sf::Vector2f(wx, wy));
	background.setFillColor(sf::Color(30, 30, 30, 215));
}

void GameOverMenu::input()
{
	elapsed_input = clock_input.getElapsedTime();
	if(elapsed_input.asSeconds() > 0.15) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			button_num -= 1;
			if(button_num == 0) button_num = 1;
			else clock_input.restart();
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			button_num += 1;
			if(button_num == 4) button_num = 3;
			else clock_input.restart();
		}	
	}

	selected = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		selected = true;
}

void GameOverMenu::update()
{
	switch(button_num) {
		case 1:
			buttonHighlight.setPosition(sf::Vector2f(dim_x - (dim_y/8), dim_y - (dim_y/8)));
			break;
		case 2:
			buttonHighlight.setPosition(sf::Vector2f(dim_x - (dim_y/8), dim_y - (dim_y/8) + (dim_y*2)));
			break;
		case 3:
			buttonHighlight.setPosition(sf::Vector2f(dim_x - (dim_y/8), dim_y - (dim_y/8) + (dim_y*4)));
			break;
		default:
			buttonHighlight.setPosition(sf::Vector2f(dim_x - (dim_y/8), dim_y - (dim_y/8)));
			break;
	}  
}

void GameOverMenu::draw(sf::RenderWindow& win)
{
	win.draw(background);
	win.draw(buttonHighlight);
	retryButton.drawButton(win);
	menuBotton.drawButton(win);
	exitButton.drawButton(win);
}

int GameOverMenu::response()
{
	if(button_num == 1 && selected) return 1;
	if(button_num == 2 && selected) return 2;
	if(button_num == 3 && selected) return 3;
	return 0;
}
