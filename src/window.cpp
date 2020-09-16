#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <fstream>
#include <string>

#include "game.cpp"
#include "gui/message.cpp"
#include "gameover.cpp" 
#include "mainmenu.cpp"
#include "options.cpp"

enum {Menu = 1, mainGame = 2, gameOver = 3, options = 4, records = 5};
int game_state = 1;

class Window
{
public:
	Window(std::string);
		float wx;
		float wy;
		//float wx = 1920;
		//float wy = 1080;
		//float wx = 960;
		//float wy = 540;
	void init();
	void windowLoop();
	void gameLoop();
	void overLoop();
	void menuLoop();
	void optionsLoop();
	bool isOpen();
	void saveScore();

	std::string TITLE;
	
private:
	Game game;
	GameOverMenu overMenu;
	mainMenu main_menu;
	Options menu_options;
		std::string opt[10];
	
	sf::RenderWindow mWindow;
	sf::Event event_list;
	bool isOnFocus;
	bool isFullscreen = false;

	Message text_points[2];

	sf::Texture texture_backgroud;
	sf::Sprite sprite_background;
	sf::RectangleShape points_background;

	std::vector<std::string> background_paths;
	int background_num = 0;
	bool uphud = false;

	//std::vector<sf::Music> songs;
	sf::Music songs;
	std::vector<std::string> song_paths;
	int volume = 0;
	int songs_num = 2;
	bool songs_start = false;
	bool songs_stoped = false;

	Message fps_counter;
	sf::Clock FPS;
	sf::RectangleShape FPSbackground;
	int fps_count = 0;
	bool show_fps = true;
	
};

Window::Window(std::string title)
{
	TITLE = title;
	std::ifstream file("../resources/options.conf", std::ios::binary);
	int i = 0;
	while(file) {
		if(i == 10) break;
		std::string string;
		getline(file, string);
		opt[i] = string;
		i++;
	}
	file.close();

	wy = stoi(opt[7]);
	wx = wy/9*16;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	isFullscreen = (opt[6] == "yes") ? true : false;
	if(isFullscreen) {
		mWindow.create(sf::VideoMode(wx, wy), TITLE, sf::Style::Titlebar | sf::Style::Close | sf::Style::Fullscreen, settings);
	} else {
		mWindow.create(sf::VideoMode(wx, wy), TITLE, sf::Style::Titlebar | sf::Style::Close, settings);
	}
	
	mWindow.setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width/2) - (wx/2), 
									 (sf::VideoMode::getDesktopMode().height/2) - (wy/2)));

	FPSbackground.setSize(sf::Vector2f(wx/15, wy/19));
	FPSbackground.setFillColor(sf::Color::Black);
	FPSbackground.setOutlineThickness(wx/200);
	FPSbackground.setOutlineColor(sf::Color(60, 60, 60));
	fps_counter.initMessage("", wy/14);
	fps_counter.setPosition(wy/50, -wy/36);
	
	
	init();
}

void Window::init()
{
	game.table_dimensions = stoi(opt[0]);

	if(opt[8] != "no") mWindow.setFramerateLimit(stoi(opt[8]));
	mWindow.setVerticalSyncEnabled((opt[9] == "yes") ? true : false);
	//mWindow.setKeyRepeatEnabled(false);
	mWindow.setMouseCursorVisible(false);

	if(isFullscreen) game.initGame(wy, wy,  wy/10,  wy/10);
	else game.initGame(wy /*- (wy/2)*/, wy /*- (wy/2)*/, wy/10, wy/10);
	game.setWallCollidable((opt[1] == "yes") ? true : false);
	overMenu.init(wx, wy);
	main_menu.init(wx, wy);
	menu_options.init(wx, wy);

	uphud = (opt[5] == "up") ? true : false;
	
	text_points[0].initMessage("", wy/10);
	if(uphud) text_points[0].setPosition(wy*0.95, (wy*0.75) - ((wy-(wy/5))/20)*17);
	else text_points[0].setPosition(wy*0.95, (wy*0.75) - ((wy-(wy/5))/20));
	text_points[1].initMessage("", wy/10);
	if(uphud) text_points[1].setPosition(wy*0.95, (wy*0.8) - ((wy-(wy/5))/20)*17);
	else text_points[1].setPosition(wy*0.95, (wy*0.8) - ((wy-(wy/5))/20));

	//sf::FloatRect textRect = text_points[0].getLocalBounds();
	points_background.setSize(sf::Vector2f(((wy*0.8)/10)*12, ((wy*0.8)/10)*2));
	points_background.setFillColor(sf::Color(0, 0, 0, 216));
	if(uphud) points_background.setPosition(wy*0.925, wy/10);
	else points_background.setPosition(wy*0.925, (wy/10) + ((wy-(wy/5))/10)*8);

	int i = 0;
	for (auto & p : std::filesystem::directory_iterator("../resources/texture/background")) {
		background_paths.resize(background_paths.size() + 1);
		background_paths[i] = p.path();
		i++;
	}

	int j = 0;
	for (auto & p : std::filesystem::directory_iterator("../resources/audio")) {
		song_paths.resize(song_paths.size() + 1);
		song_paths[j] = p.path();
		j++;
	}

	if(background_paths.size() == 0) 
		{sf::Image image; image.create(wx, wy, sf::Color(48, 98, 48)); texture_backgroud.loadFromImage(image);}
	else 
		if(!texture_backgroud.loadFromFile(background_paths[stoi(opt[2])]))
			{sf::Image image; image.create(wx, wy, sf::Color(48, 98, 48)); texture_backgroud.loadFromImage(image);}
	sprite_background.setTexture(texture_backgroud);
	if(!(wx == texture_backgroud.getSize().x)) sprite_background.setScale(wx/texture_backgroud.getSize().x, wy/texture_backgroud.getSize().y);

	songs_start = false;
	songs.openFromFile(song_paths[stoi(opt[3])]);
	songs.setLoop(true);
	songs.setVolume(stoi(opt[4]));
}

void Window::saveScore() {
	std::ofstream file("../resources/score.bin", std::ios::out | std::ios::app | std::ios::binary);
    file << game.points << std::endl;
    file.close();
}

void Window::windowLoop()
{
	while(mWindow.pollEvent(event_list)) {
		if(event_list.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) 
			mWindow.close();
		if(event_list.type == sf::Event::LostFocus)
			isOnFocus = false;
		if(event_list.type == sf::Event::GainedFocus)
			isOnFocus = true;
		if(event_list.type == sf::Event::KeyReleased) {
			/* if(event_list.key.code == sf::Keyboard::W) {
				if(background_num < background_paths.size()-1) background_num += 1;
				else background_num = 0;
				texture_backgroud.loadFromFile(background_paths[background_num]);
				sprite_background.setTexture(texture_backgroud);
			}*/

			if(event_list.key.code == sf::Keyboard::H) {
				if(uphud) {
					uphud = false;
					text_points[0].setPosition(wy*0.95, (wy*0.75) - ((wy-(wy/5))/20));
					text_points[1].setPosition(wy*0.95, (wy*0.8) - ((wy-(wy/5))/20));
					points_background.setPosition(wy*0.925, (wy/10) + ((wy-(wy/5))/10)*8);
				}else {
					uphud = true;
					text_points[0].setPosition(wy*0.95, (wy*0.75) - ((wy-(wy/5))/20)*17);
					text_points[1].setPosition(wy*0.95, (wy*0.8) - ((wy-(wy/5))/20)*17);
					points_background.setPosition(wy*0.925, wy/10);
				}
			}

			if(event_list.key.code == sf::Keyboard::K) {
				if(!songs_stoped) {songs.pause(); songs_stoped = true;}
				else {songs.play(); songs_stoped = false;}
			}

			if(event_list.key.code == sf::Keyboard::L) {
				if(volume != 100) volume += 10;
				songs.setVolume(volume);
			}

			if(event_list.key.code == sf::Keyboard::J) {
				if(volume != 0) volume -= 10;
				songs.setVolume(volume);
			}
			
		}
	}

	text_points[0].setString("points  " + std::to_string(game.points));
	text_points[1].setString("eaten apple  " + std::to_string(game.apple_eaten));

	if(show_fps) {
		sf::Time elapsed_fps = FPS.getElapsedTime();
		fps_count += 1;
		if(elapsed_fps.asSeconds() >= 1) {
			fps_counter.setString(std::to_string(fps_count));
			fps_count = 0;	
			FPS.restart();
		}
	}

	if(isOnFocus) {
		mWindow.clear(sf::Color::Black);
		switch(game_state)
		{
			case Menu:
				menuLoop();
				break;
			case mainGame:
				gameLoop();
				break;
			case gameOver:
				overLoop();
				break;
			case options:
				optionsLoop();
				break;
			default:
				break;
		}
		if(show_fps) {
			mWindow.draw(FPSbackground);
			fps_counter.draw(mWindow);
		}
	}
	mWindow.display();
}

void Window::menuLoop()
{
	main_menu.input();
	main_menu.update();
	main_menu.draw(mWindow);
	if(main_menu.response() == 2) game_state = mainGame;
	if(main_menu.response() == 4) game_state = options;
	//if(main_menu.response() == 5) game_state = records;
	if(main_menu.response() == -1) mWindow.close();
}

void Window::gameLoop()
{
	if(!songs_start) {songs.play(); songs_start = true;}
	mWindow.draw(sprite_background);
	mWindow.draw(points_background);
		text_points[0].draw(mWindow);
		text_points[1].draw(mWindow);
	game.snakeInput();
 	game.moveSnake();
	game.checkApple();	
	game.drawWorld(mWindow);
	game.drawSnake(mWindow);
	if(game.isSnakeColliding) {game_state = gameOver; songs.stop(); saveScore();}
}

void Window::overLoop()
{
	mWindow.draw(sprite_background);
	mWindow.draw(points_background);
	text_points[0].draw(mWindow);
	text_points[1].draw(mWindow);
	game.drawWorld(mWindow);
	game.drawSnake(mWindow);
	overMenu.input();
	overMenu.update();
	overMenu.draw(mWindow);
	if(overMenu.response() == 1) {init(); game_state = mainGame;}
	if(overMenu.response() == 2) {init(); game_state = Menu;}
	if(overMenu.response() == 3) mWindow.close();
}

void Window::optionsLoop()
{
	menu_options.draw(mWindow);
	menu_options.input();
	if(menu_options.response() == 1) {init(); game_state = Menu;}
}

bool Window::isOpen()
{
	return mWindow.isOpen();
}
