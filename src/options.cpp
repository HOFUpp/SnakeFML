#include <SFML/Graphics.hpp>
#include <filesystem>
#include <fstream>
#include <string>

class Options
{
public:
	void init(int, int);
	void input();
	void update();
	void draw(sf::RenderWindow&);
	int response();

private:	
	float name_dim[2];
	float list_dim[2];

	float backDim[2];

	int option_num = 0;
	int option_wide[10];
	int option_wide_selected[10];
	bool move_option_right = false;
	bool move_option_left = false;

	sf::Clock clock_input;
	sf::Time elapsed_input;
	sf::Clock clock_move_options;
	sf::Time elapsed_move_options;

	sf::RectangleShape background;
	sf::RectangleShape background_rect;
	
	sf::Font font;
	sf::Text message;
	sf::Text options_text;
	
	std::string options_name[10];
	std::string option_list[10];

	bool isExiting = false;

};

void Options::init(int wx, int wy)
{
	options_name[0] = "map dimensions";
	options_name[1] = "collidable wall";
	options_name[2] = "background"; 		// custom
	options_name[3] = "music"; 				// custom
	options_name[4] = "volume"; 			// custom
	options_name[5] = "show point";
	options_name[6] = "fullscreen";
	options_name[7] = "resolution";
	options_name[8]	= "FPS limit";
	options_name[9] = "VSYNC";

	int i = 0;
	for (auto & p : std::filesystem::directory_iterator("../resources/texture/background")) ++i;
	int j = 0;
	for (auto & p : std::filesystem::directory_iterator("../resources/audio")) ++j;

	option_wide[0] = 4;
	option_wide[1] = 2;
	option_wide[2] = i;
	option_wide[3] = j;
	option_wide[4] = 5;
	option_wide[5] = 2;
	option_wide[6] = 2;
	option_wide[7] = 3;
	option_wide[8] = 3;
	option_wide[9] = 2;

	backDim[0] = wx*0.8;
	backDim[1] = wy/12;

	option_num = 0;
	isExiting = false;

	background.setSize(sf::Vector2f(wx, wy));
	background.setFillColor(sf::Color(48, 98, 48));

	background_rect.setSize(sf::Vector2f(backDim[0], backDim[1]));
	background_rect.setFillColor(sf::Color(200, 200, 200));

	font.loadFromFile("font/8-bitArcadeIn.ttf");
	message.setFont(font);
	message.setString(options_name[0]);
	message.setCharacterSize(backDim[1]);
	sf::FloatRect textRect;
	textRect = message.getLocalBounds();
	message.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
	message.setPosition((backDim[0]/0.8)*0.05 + backDim[0]/5, (backDim[1]) - (backDim[1]/2) + backDim[1]/2);
}

void Options::input()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) isExiting = true;

	elapsed_input = clock_input.getElapsedTime();
	if(elapsed_input.asSeconds() > 0.15) {
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			option_num -= 1;
			if(option_num < 0) {
				option_num = 0;
			} else {
				clock_input.restart();
				clock_move_options.restart();
			}
		} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			option_num += 1;
			if(option_num > 9) {
				option_num = 9;
			} else {
				clock_input.restart();
				clock_move_options.restart();	
			}
		} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			if(1) move_option_right = true;
		} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			if(1) move_option_left = true;
		}
	}
}

void Options::update()
{
	switch(option_num) {
		case 0:
			
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		default:
			break;
	}
}

void Options::draw(sf::RenderWindow& win)
{
	elapsed_move_options = clock_move_options.getElapsedTime();
	win.draw(background);
	for(int i = 0; i < 10; ++i) {
		message.setString(options_name[i]);
		message.setPosition((backDim[0]/0.8)*0.05 + backDim[0]/5, (backDim[1]*(i+1)) - (backDim[1]/2) + backDim[1]/2);
		background_rect.setPosition((backDim[0]/0.8)*0.05, (backDim[1]*(i+1)) - (backDim[1]/2));
		if(option_num == i) background_rect.setFillColor(sf::Color(31, 31, 31));
		else background_rect.setFillColor(sf::Color(21, 21, 21));
		if(option_num == i) {
			if(elapsed_move_options.asSeconds() <= 0.1) {
				background_rect.move((backDim[0]/0.8*0.1)/0.1 * elapsed_move_options.asSeconds(), 0.f);
				message.move((backDim[0]/0.8*0.1)/0.1 * elapsed_move_options.asSeconds(), 0.f);
			} else {
				background_rect.move(backDim[0]/0.8*0.1, 0.f);
				message.move(backDim[0]/0.8*0.1, 0.f);
			}
		}
		win.draw(background_rect);
		win.draw(message);
	}
}

int Options::response()
{
	if(isExiting) return 1;
	return 0;
}
