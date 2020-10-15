#include <SFML/Graphics.hpp>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

class Options
{
public:
	void init(int, int);
	void input();
	void update();
	std::string opt_name(int);
	void save();
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
	sf::Clock clock_change_options;
	sf::Time elapsed_change_options;
	sf::Clock clock_enter;
	sf::Time elapsed_enter;

	sf::RectangleShape background;
	sf::RectangleShape background_rect;
	
	sf::Font font;
	sf::Text message;
	sf::Text options_text;
	
	std::string options_name[10];
	std::string option_list[10];

	std::vector<std::string> background_names;
	std::vector<std::string> song_names;

	bool isExiting = false;
	sf::FloatRect textRect;

	int wx, wy;

};

void Options::init(int x, int y)
{
	wx = x;
	wy = y;
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
	int j = 0;
	for (auto & p : std::filesystem::directory_iterator("../resources/texture/background")) {
		background_names.resize(background_names.size() + 1);
		background_names[i] = p.path();
		background_names[i] = background_names[i].substr(32, background_names[i].size() - 32);
		++i;
	}
	for (auto & p : std::filesystem::directory_iterator("../resources/audio")) {
		song_names.resize(background_names.size() + 1);
		song_names[j] = p.path();
		song_names[j] = song_names[j].substr(19, song_names[j].size() - 19);
		++j;
	}

	option_wide[0] = 4;
	option_wide[1] = 2;
	option_wide[2] = i;
	option_wide[3] = j;
	option_wide[4] = 5;
	option_wide[5] = 2;
	option_wide[6] = 2;
	option_wide[7] = 4;
	option_wide[8] = 3;
	option_wide[9] = 2;

	option_wide_selected[0] = 0;

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
	textRect = message.getLocalBounds();
	message.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
	message.setPosition((backDim[0]/0.8)*0.05 + backDim[0]/5, (backDim[1]) - (backDim[1]/2) + backDim[1]/2);

	options_text.setFont(font);
	options_text.setString(options_name[0]);
	options_text.setCharacterSize(backDim[1]);
	textRect = options_text.getLocalBounds();
	options_text.setOrigin(textRect.left + textRect.width, textRect.top  + textRect.height/2.0f);

	std::ifstream file("../resources/options.conf", std::ios::binary);
	int k = 0;
	std::string string[10];
	while(file) {
		if(k == 10) break;
		std::string opt;
		getline(file, opt);
		string[k] = opt;
		k++;
	}
	file.close();

	if(string[0] == "8") option_wide_selected[0] = 0;
	else if(string[0] == "10") option_wide_selected[0] = 1; 
	else if(string[0] == "12") option_wide_selected[0] = 2; 
	else if(string[0] == "14") option_wide_selected[0] = 3; 
	else option_wide_selected[0] = 0;

	if(string[1] == "no") option_wide_selected[1] = 0;
	else if(string[1] == "yes") option_wide_selected[1] = 1;
	else option_wide_selected[1] = 0;

	option_wide_selected[2] = stoi(string[2]);

	option_wide_selected[3] = stoi(string[3]);

	if(string[4] == "0") option_wide_selected[4] = 0;
	else if(string[4] == "25") option_wide_selected[4] = 1;
	else if(string[4] == "50") option_wide_selected[4] = 2;
	else if(string[4] == "75") option_wide_selected[4] = 3;
	else if(string[4] == "100") option_wide_selected[4] = 4;
	else option_wide_selected[4] = 0;

	if(string[5] == "no") option_wide_selected[5] = 0;
	else if(string[5] == "yes") option_wide_selected[5] = 1;
	else option_wide_selected[5] = 0;

	if(string[6] == "no") option_wide_selected[6] = 0;
	else if(string[6] == "yes") option_wide_selected[6] = 1;
	else option_wide_selected[6] = 0;

	if(string[7] == "540") option_wide_selected[7] = 0;
	else if(string[7] == "720") option_wide_selected[7] = 1;
	else if(string[7] == "768") option_wide_selected[7] = 2; 
	else if(string[7] == "1080") option_wide_selected[7] = 3;
	else option_wide_selected[7] = 0;

	if(string[8] == "no") option_wide_selected[8] = 0;
	else if(string[8] == "30") option_wide_selected[8] = 1;
	else if(string[8] == "60") option_wide_selected[8] = 2;
	else option_wide_selected[8] = 0;

	if(string[9] == "no") option_wide_selected[9] = 0;
	else if(string[9] == "yes") option_wide_selected[9] = 1;
	else option_wide_selected[9] = 0;

	clock_input.restart();
	clock_enter.restart();
}

void Options::input()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) isExiting = true;

	elapsed_input = clock_input.getElapsedTime();
	elapsed_change_options = clock_change_options.getElapsedTime();
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
		} 
	}
	
	elapsed_enter = clock_enter.getElapsedTime();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && elapsed_enter.asSeconds() > 3) {
		save();
		isExiting = true;
	}
	
	if(elapsed_change_options.asSeconds() > 0.15) {
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			move_option_right = true;
			clock_change_options.restart();
		} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			move_option_left = true;
			clock_change_options.restart();
		}
	}
}

void Options::update()
{
	if(move_option_right) {
		if(option_wide_selected[option_num] < option_wide[option_num]-1) option_wide_selected[option_num]++;
		else option_wide_selected[option_num] = 0;
	} else if(move_option_left) {
		if(option_wide_selected[option_num] > 0) option_wide_selected[option_num]--;
		else option_wide_selected[option_num] = option_wide[option_num]-1;
	}
	move_option_right = false;
	move_option_left = false;
}

void Options::save()
{
	std::ofstream file("../resources/options.conf", std::ios::binary);
	std::string save_opt;

	for(int i = 0; i < 10; i++) {
		switch(i) {
			case 0:
				if(option_wide_selected[0] == 0) save_opt = "8";
				if(option_wide_selected[0] == 1) save_opt = "10";
				if(option_wide_selected[0] == 2) save_opt = "12";
				if(option_wide_selected[0] == 3) save_opt = "14";
				break;
			case 1:
				if(option_wide_selected[1] == 0) save_opt = "no";
				if(option_wide_selected[1] == 1) save_opt = "yes";
				break;
			case 2:
				save_opt = std::to_string(option_wide_selected[2]);				
				break;
			case 3:
				save_opt = std::to_string(option_wide_selected[3]);
				break;
			case 4:
				if(option_wide_selected[4] == 0) save_opt = "0";
				if(option_wide_selected[4] == 1) save_opt = "25";
				if(option_wide_selected[4] == 2) save_opt = "50";
				if(option_wide_selected[4] == 3) save_opt = "75";
				if(option_wide_selected[4] == 4) save_opt = "100";
				break;
			case 5:
				if(option_wide_selected[5] == 0) save_opt = "down";
				if(option_wide_selected[5] == 1) save_opt = "up";
				break;
			case 6:
				if(option_wide_selected[6] == 0) save_opt = "no";
				if(option_wide_selected[6] == 1) save_opt = "yes";
				break;
			case 7:
				if(option_wide_selected[7] == 0) save_opt = "540";
				if(option_wide_selected[7] == 1) save_opt = "720";
				if(option_wide_selected[7] == 2) save_opt = "768";
				if(option_wide_selected[7] == 3) save_opt = "1080";
				break;
			case 8:
				if(option_wide_selected[8] == 0) save_opt = "no";
				if(option_wide_selected[8] == 1) save_opt = "30";
				if(option_wide_selected[8] == 2) save_opt = "60";
				break;
			case 9:
				if(option_wide_selected[9] == 0) save_opt = "no";
				if(option_wide_selected[9] == 1) save_opt = "yes";
				break;
			default:
				break;
		}
		file << save_opt << std::endl;
	}
}

std::string Options::opt_name(int num)
{
	switch(num) {
		case 0:
			if(option_wide_selected[0] == 0) return "8";
			if(option_wide_selected[0] == 1) return "10";
			if(option_wide_selected[0] == 2) return "12";
			if(option_wide_selected[0] == 3) return "14";
			break;
		case 1:
			if(option_wide_selected[1] == 0) return "no";
			if(option_wide_selected[1] == 1) return "yes";
			break;
		case 2:
			{
			std::string str = background_names[option_wide_selected[2]];
			if(str.length() > 10) str.resize(10);
			return str;
			}
			break;
		case 3:
			{
			std::string str = song_names[option_wide_selected[3]];
			if(str.length() > 10) str.resize(10);
			return str;
			}
			break;
		case 4:
			if(option_wide_selected[4] == 0) return "0";
			if(option_wide_selected[4] == 1) return "25";
			if(option_wide_selected[4] == 2) return "50";
			if(option_wide_selected[4] == 3) return "75";
			if(option_wide_selected[4] == 4) return "100";
			break;
		case 5:
			if(option_wide_selected[5] == 0) return "down";
			if(option_wide_selected[5] == 1) return "up";
			break;
		case 6:
			if(option_wide_selected[6] == 0) return "no";
			if(option_wide_selected[6] == 1) return "yes";
			break;
		case 7:
			if(option_wide_selected[7] == 0) return "540p";
			if(option_wide_selected[7] == 1) return "720p";
			if(option_wide_selected[7] == 2) return "768p";
			if(option_wide_selected[7] == 3) return "1080p";
			break;
		case 8:
			if(option_wide_selected[8] == 0) return "no";
			if(option_wide_selected[8] == 1) return "30FPS";
			if(option_wide_selected[8] == 2) return "60FPS";
			break;
		case 9:
			if(option_wide_selected[9] == 0) return "no";
			if(option_wide_selected[9] == 1) return "yes";
			break;
		default:
			break;
	}

	return "ERROR";
}

void Options::draw(sf::RenderWindow& win)
{
	elapsed_move_options = clock_move_options.getElapsedTime();
	win.draw(background);
	message.setString("press ESC to exit");
	message.setPosition(wx/5, wy-wx/32);
	win.draw(message);
	for(int i = 0; i < 10; ++i) {
		message.setString(options_name[i]);
		message.setPosition((backDim[0]/0.8)*0.05 + backDim[0]/5, (backDim[1]*(i+1)) - (backDim[1]/2) + backDim[1]/2);
		options_text.setString(opt_name(i));
		options_text.setPosition(backDim[0]*1.1, (backDim[1]*(i+1)) - (backDim[1]/2.5) + backDim[1]/2.4);
		background_rect.setPosition((backDim[0]/0.8)*0.05, (backDim[1]*(i+1)) - (backDim[1]/2));
		if(option_num == i) background_rect.setFillColor(sf::Color(31, 31, 31));
		else background_rect.setFillColor(sf::Color(21, 21, 21));
		if(option_num == i) {
			if(elapsed_move_options.asSeconds() <= 0.1) {
				background_rect.move((backDim[0]/0.8*0.1)/0.1 * elapsed_move_options.asSeconds(), 0.f);
				message.move((backDim[0]/0.8*0.1)/0.1 * elapsed_move_options.asSeconds(), 0.f);
				options_text.move((backDim[0]/0.8*0.1)/0.1 * elapsed_move_options.asSeconds(), 0.f);
			} else {
				background_rect.move(backDim[0]/0.8*0.1, 0.f);
				message.move(backDim[0]/0.8*0.1, 0.f);
				options_text.move(backDim[0]/0.8*0.1, 0.f);
			}
		}
		win.draw(background_rect);
		win.draw(message);
		win.draw(options_text);	
	}
}

int Options::response()
{
	if(isExiting) return 1;
	return 0;
}
