#include <SFML/Graphics.hpp>
#include <string>
#include "gui/button.cpp"
#include "gui/message.cpp"

class mainMenu
{
public:
	void init(int, int);
	void input();
	void update();
	void draw(sf::RenderWindow&);
	int response();
	
private:
	int dim_x, dim_y;
	int wx, wy;
	Button start;
	Button options;
	Button records;
	Button exit;

	Message title;

	sf::Clock clock_input;
	sf::Time elapsed_input;
	sf::Clock clock_move_button;
	sf::Time elapsed_move_Button;
	
	int button_num;
	bool selected;
	float y_offset;

	sf::RectangleShape highlightLine;
	sf::RectangleShape fakeButton;
	sf::RectangleShape background;

	bool changedPos;
};

void mainMenu::init(int window_x, int window_y)
{
	dim_x = window_x/4;
	dim_y = window_y/7;

	wx = window_x;
	wy = window_y;
	
	start.init("start", dim_x, dim_y, sf::Vector2f(0, wy - dim_y), wy/9);
	options.init("options", dim_x, dim_y, sf::Vector2f(dim_x, wy - dim_y), wy/9);
	records.init("records", dim_x, dim_y, sf::Vector2f(dim_x*2, wy - dim_y), wy/9);
	exit.init("exit", dim_x, dim_y, sf::Vector2f(dim_x*3, wy - dim_y), wy/9);

	changedPos = true;
	
	title.initMessage("Snake", wy/2.5);
	title.changeFont();
	title.setOriginCenter();
	title.setPosition(wx/2, wy/3);

	button_num = 1;
	selected = false;

	fakeButton.setSize(sf::Vector2f(wx, dim_y));
	fakeButton.setPosition(sf::Vector2f(0.f, wy-dim_y));
	fakeButton.setFillColor(sf::Color(41, 41, 41));

	highlightLine.setSize(sf::Vector2f(dim_x, dim_y/8));
	highlightLine.setPosition(0.f, wy - dim_y*2 - (dim_y/8));
	highlightLine.setFillColor(sf::Color(220, 220, 220));

	background.setSize(sf::Vector2f(wx, wy));
	background.setFillColor(sf::Color(48, 98, 48));
	clock_input.restart();

	start.setPosition(0.f, wy - (dim_y*2));
}

void mainMenu::input()
{
	elapsed_input = clock_input.getElapsedTime();
	if(elapsed_input.asSeconds() > 0.15) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			button_num -= 1;
			if(button_num == 0) button_num = 1;
			else {clock_input.restart(); clock_move_button.restart();}
			changedPos = false;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			button_num += 1;
			if(button_num == 5) button_num = 4;
			else {clock_input.restart(); clock_move_button.restart();}
			changedPos = false;
		}

		selected = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			selected = true;	
	}	
}

void mainMenu::update()
{
	start.setFillColor(sf::Color(21, 21, 21));
	options.setFillColor(sf::Color(21, 21, 21));
	records.setFillColor(sf::Color(21, 21, 21));
	exit.setFillColor(sf::Color(21, 21, 21));
	
	elapsed_move_Button = clock_move_button.getElapsedTime();
	switch(button_num) {
		case 1:
			start.setFillColor(sf::Color(41, 41, 41));
			if(elapsed_move_Button.asSeconds() <= 0.1) {
				start.setPosition(0.f, wy - (dim_y + ((dim_y/0.1)*elapsed_move_Button.asSeconds())));
				highlightLine.setPosition(0.f, wy - (dim_y + ((dim_y/0.1)*elapsed_move_Button.asSeconds())) - dim_y/8);
			} else {
				start.setPosition(0.f, wy - dim_y*2);
				highlightLine.setPosition(0.f, wy - dim_y*2 - dim_y/8);
			} 
			if(!changedPos) {
				options.setPosition(dim_x, wy - (dim_y));;
				changedPos = true;	
			}
			break;
		case 2:
			options.setFillColor(sf::Color(41, 41, 41));
			if(elapsed_move_Button.asSeconds() <= 0.1) {
				options.setPosition(dim_x, wy - (dim_y + ((dim_y/0.1)*elapsed_move_Button.asSeconds())));
				highlightLine.setPosition(dim_x, wy - (dim_y + ((dim_y/0.1)*elapsed_move_Button.asSeconds())) - dim_y/8);
			} else { 
				options.setPosition(dim_x, wy - dim_y*2);
				highlightLine.setPosition(dim_x, wy - dim_y*2 - dim_y/8);
			}
			if(!changedPos) {
				start.setPosition(0.f, wy - (dim_y));
				records.setPosition(dim_x*2, wy - (dim_y));
				changedPos = true;	
			}
			break;
		case 3:
			records.setFillColor(sf::Color(41, 41, 41));
			if(elapsed_move_Button.asSeconds() <= 0.1) {
				records.setPosition(dim_x*2, wy - (dim_y + ((dim_y/0.1)*elapsed_move_Button.asSeconds())));
				highlightLine.setPosition(dim_x*2, wy - (dim_y + ((dim_y/0.1)*elapsed_move_Button.asSeconds())) - dim_y/8);
			} else { 
				records.setPosition(dim_x*2, wy - dim_y*2);
				highlightLine.setPosition(dim_x*2, wy - dim_y*2 - dim_y/8);
			}
			if(!changedPos) {
				options.setPosition(dim_x, wy - (dim_y));
				exit.setPosition(dim_x*3, wy - (dim_y));
				changedPos = true;	
			}
			break;
		case 4:
			exit.setFillColor(sf::Color(41, 41, 41));
			if(elapsed_move_Button.asSeconds() <= 0.1) { 
				exit.setPosition(dim_x*3, wy - (dim_y + ((dim_y/0.1)*elapsed_move_Button.asSeconds())));
				highlightLine.setPosition(dim_x*3, wy - (dim_y + ((dim_y/0.1)*elapsed_move_Button.asSeconds())) - dim_y/8);
			} else { 
				exit.setPosition(dim_x*3, wy - dim_y*2);
				highlightLine.setPosition(dim_x*3, wy - dim_y*2 - dim_y/8);
			}
			if(!changedPos) {
				records.setPosition(dim_x*2, wy - (dim_y));
				changedPos = true;	
			}
			break;
	}
}

void mainMenu::draw(sf::RenderWindow& win)
{
	win.draw(background);
	win.draw(highlightLine);
	win.draw(fakeButton);
	title.draw(win);
	start.drawButton(win);
	options.drawButton(win);
	records.drawButton(win);
	exit.drawButton(win);
}

int mainMenu::response()
{
	if(button_num == 1 && selected) return 2;
	if(button_num == 2 && selected) return 4;
	if(button_num == 3 && selected) return 5;
	if(button_num == 4 && selected) return -1;
	return 0;
}
