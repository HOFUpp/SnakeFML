#include <SFML/Graphics.hpp>
#include <string>
#include "game.cpp"

class Window
{
public:
	Window(std::string);
		float wx = 960;
		float wy = 540;
	void windowLoop();
	void gameLoop();
	bool isOpen();

	std::string TITLE;
	bool isFullscreen = false;
	sf::Clock clockFullscreen;
	
private:
	Game game;
	sf::RenderWindow mWindow;
	sf::Event event_list;
	bool isOnFocus = true;

	sf::Texture texture_backgroud;
	sf::Sprite sprite_background;

};

Window::Window(std::string title)
{
	TITLE = title;
	mWindow.create(sf::VideoMode(wx, wy), TITLE, sf::Style::Titlebar | sf::Style::Close /*| sf::Style::Fullscreen*/);	
	game.initGame(wx, wy, ((wx-wy)/2), 0.f);
	
	mWindow.setFramerateLimit(60);
	mWindow.setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width/2) - (wx/2), 
									 (sf::VideoMode::getDesktopMode().height/2) - (wy/2)));
	mWindow.setVerticalSyncEnabled(true);

	texture_backgroud.loadFromFile("../txt/background.png");
	sprite_background.setTexture(texture_backgroud);
	sprite_background.setScale(0.5, 0.5);
}

void Window::windowLoop()
{
	sf::Time elapsedFull = clockFullscreen.getElapsedTime();
	while(mWindow.pollEvent(event_list)) {
		if(event_list.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) 
			mWindow.close();
		if (event_list.type == sf::Event::LostFocus)
			isOnFocus = false;
		if (event_list.type == sf::Event::GainedFocus)
			isOnFocus = true;
	}


	if(isOnFocus) {
		mWindow.clear(sf::Color::Black);
		mWindow.draw(sprite_background);
		gameLoop();
	}
	mWindow.display();
}

void Window::gameLoop()
{
	if(!game.isBodyColliding()) {
		game.snakeInput();
 		game.moveSnake();
		game.checkApple();	
	}
	game.drawWorld(mWindow);
	game.drawSnake(mWindow);
	if(game.isBodyColliding())
		mWindow.close();//game.GameOver();
}

bool Window::isOpen()
{
	return mWindow.isOpen();
}