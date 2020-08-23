#include <SFML/Graphics.hpp>
#include <vector> 				
#include <ctime>                //for "srand(time(NULL))""
#include <cstdlib>              //for "rand()
#include "button.cpp"

class Game
{
public:
	void initGame(float, float, float, float);
	void drawWorld(sf::RenderWindow&);
	sf::Vector2f newApplePosition();
	void checkApple(); 							// checks if the apple has been eated
	void drawSnake(sf::RenderWindow&); 			// draws the head and the body of the snake
	void snakeInput(); 						
	void moveSnake();
	void wallCollisionResponse(); 				// old aarcade style wall teleportation
	bool isBodyColliding(); 					// checks if the head is colliding with the body 
	int GameOver(); 							// displays the game over screen and (depending on user input) 
												// redirects the user to another screen
			
private:
	float offset_x = 0;
	float offset_y = 0;

	sf::RectangleShape drawingBlock; 			// this sf::RectangleShape is used to draw almost anything to the screen
	bool alive = true; 							// is the snake alive?
	bool eaten = true; 							// has the apple been eaten?
	bool has_game_start = false;
	bool isSnakeColliding = false; 				// whith it self or (depanding with the user options) the wall
	float table_dimensions = 10; 					// dimensions of the game world (they have to be even)
	sf::Vector2f apple_pos; 					// stores the apple position

	sf::Clock clock_input; 						// sf::Clock used for user inputs 
	//sf::Clock clock_for_background;
	sf::Time elapsed_input; 					// used to time user inputs and the snake movement
	sf::Time elapsed_background;
	bool hasStarted = true; 					// this variable is only used to make the snake move before tot. seconds 
												// have passed at the start of the game

	//Button buttons[3];
	//std::string message[] = {"retry", "main menu", "exit"};

	int x_vel = 0;  							// X and Y velocity of the snake head
	int y_vel = 0;
	std::vector<sf::Vector2f> snakePosition; 	// vector that stores the position oof the snake head 
												// (e.g. snakePosition[0]) and body (e.g. snakePosition[1, 2 ...])
};

void Game::initGame(float tx, float ty, float ofsx, float ofsy)
{
	offset_x = ofsx;
	offset_y = ofsy;
	drawingBlock.setSize(sf::Vector2f(ty/table_dimensions, ty/table_dimensions));
	srand(time(0));
	// here we are putting the head at the center of the table
	snakePosition = {sf::Vector2f((table_dimensions/2 - 1) * drawingBlock.getSize().x, 
					 (table_dimensions/2 - 1) * drawingBlock.getSize().y)};
}

void Game::drawWorld(sf::RenderWindow& win)
{
	// drawing background 
	bool turn = 1;
	for(int i = 0; i < table_dimensions; ++i) {
		for(int j = 0; j < table_dimensions; ++j) {
			drawingBlock.setPosition((drawingBlock.getSize().x) * j + offset_x, (drawingBlock.getSize().y) * i + offset_y);
			if(turn) {
				drawingBlock.setFillColor(sf::Color(5, 5, 5));
				turn = 0;
			} else {
				drawingBlock.setFillColor(sf::Color(12, 12, 12));
				turn = 1;
			}
			win.draw(drawingBlock);
		}
		if(static_cast<int>(table_dimensions) % 2 == 0)
			if(turn) turn = 0;
			else turn = 1;
	}

	// drawing apple
	drawingBlock.setFillColor(sf::Color(255, 25, 25));
	if(eaten) {
		apple_pos = newApplePosition();
		eaten = false;
	}	
	drawingBlock.setPosition(apple_pos + sf::Vector2f(offset_x, offset_y));
	drawingBlock.setScale(0.6, 0.6);
	drawingBlock.move(drawingBlock.getSize().x * 0.2, drawingBlock.getSize().y * 0.2);
	win.draw(drawingBlock);
	drawingBlock.setScale(1, 1);
}

void Game::drawSnake(sf::RenderWindow& win)
{
	for(auto pos = 0; pos < snakePosition.size(); ++pos) {
		drawingBlock.setFillColor(sf::Color(25 + (pos * 1), 140 + (pos * 2), 35 + (pos * 2)));
		drawingBlock.setPosition(snakePosition[pos] + sf::Vector2f(offset_x, offset_y));
		win.draw(drawingBlock);
	}
}

void Game::snakeInput()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {x_vel = -1; y_vel = 0;}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {x_vel = 1; y_vel = 0;}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {x_vel = 0; y_vel = -1;}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {x_vel = 0; y_vel = 1;}	
}

void Game::wallCollisionResponse()
{
	if((snakePosition[0].y / drawingBlock.getSize().x) > table_dimensions-1) 
		snakePosition[0] = sf::Vector2f(snakePosition[0].x, 0);
	else if((snakePosition[0].y / drawingBlock.getSize().x) < 0) 
		snakePosition[0] = sf::Vector2f(snakePosition[0].x, (table_dimensions-1) * drawingBlock.getSize().y);	
	else if((snakePosition[0].x / drawingBlock.getSize().x) > table_dimensions-1) 
		snakePosition[0] = sf::Vector2f(0, snakePosition[0].y);
	else if((snakePosition[0].x / drawingBlock.getSize().x) < 0) 
		snakePosition[0] = sf::Vector2f((table_dimensions-1) * drawingBlock.getSize().x, snakePosition[0].y);
}

bool Game::isBodyColliding()
{
	for(auto i = 3; i < snakePosition.size(); ++i)
		if(snakePosition[0].x == snakePosition[i].x && snakePosition[0].y == snakePosition[i].y)
			return true; 
	return false;
}

sf::Vector2f Game::newApplePosition()
{
	sf::Vector2f newPos;
	bool isOk = false;
	while(!isOk) {
		newPos = sf::Vector2f((rand() % static_cast<int>(table_dimensions)) * drawingBlock.getSize().x, 
				   			  (rand() % static_cast<int>(table_dimensions)) * drawingBlock.getSize().y);
		isOk = true;	   			  
		for(auto i = 0; i < snakePosition.size(); ++i) {
			if(newPos.x == snakePosition[i].x && newPos.y == snakePosition[i].y) {isOk = false; break;}
		} 
	} 
	
	return newPos;
}

void Game::checkApple()
{
	// checks if the apple has been eaten
	if(snakePosition[0].x == apple_pos.x && snakePosition[0].y == apple_pos.y) {
		eaten = true;
		// here we are extending the body of the snake by one block
		snakePosition.resize(snakePosition.size() + 1);
		snakePosition[snakePosition.size()-1] = snakePosition[snakePosition.size()-2];
	}
}

void Game::moveSnake()
{
	elapsed_input = clock_input.getElapsedTime();
	if(elapsed_input.asSeconds() > 0.15 || !hasStarted) {
		hasStarted = true;

		if(snakePosition[0].x + (x_vel*drawingBlock.getSize().x) == snakePosition[1].x && 
		   snakePosition[0].y + (y_vel*drawingBlock.getSize().y) == snakePosition[1].y)
		{x_vel *= -1; y_vel *= -1;}

		sf::Vector2f last_pos[snakePosition.size()];
		for(auto i = 0; i < snakePosition.size(); ++i)
			last_pos[i] = snakePosition[i];
		for(auto i = 1; i < snakePosition.size(); ++i)
			snakePosition[i] = last_pos[i-1];

		snakePosition[0] = sf::Vector2f((snakePosition[0].x) + (x_vel * drawingBlock.getSize().x), 
									   	(snakePosition[0].y) + (y_vel * drawingBlock.getSize().y));					
		
		clock_input.restart();
	}
	wallCollisionResponse();	
}

int Game::GameOver()
{
	
	return 1;	
}