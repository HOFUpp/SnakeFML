#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>

class Records
{
public:
	void init(int, int);
	void input();
	int response();
	void draw(sf::RenderWindow&);

private:
	std::string titles[6] = {
		"wall", "no wall", "x8  ", "x10 ", "x12 ", "x14 "
	};
	std::string points[8];

	float backDim[2];
	float wx, wy;
	
	sf::RectangleShape background;
	sf::RectangleShape background_rect_category;
	sf::RectangleShape background_rect_points;

	sf::Font font;
	sf::Text header_text;
	sf::Text point_text;
	sf::FloatRect textRect;

	bool isExiting = false;
	
};

void Records::init(int x, int y)
{
	isExiting = false;

	wx = x;
	wy = y;

	backDim[0] = wx*0.38;
	backDim[1] = wy/8;
	
	background.setSize(sf::Vector2f(wx, wy));
	background.setFillColor(sf::Color(48, 98, 48));

	background_rect_category.setSize(sf::Vector2f(backDim[0], backDim[1]));
	background_rect_category.setFillColor(sf::Color(21, 21, 21));

	background_rect_points.setSize(sf::Vector2f(backDim[0], backDim[1]));
	background_rect_points.setFillColor(sf::Color(31, 31, 31));

	std::ifstream file("../resources/score.bin", std::ios::binary);
	int i = 0;
	while(file) {
		if(i == 8) break;
		std::string string;
		getline(file, string);
		points[i] = string;
		i++;
	}
	file.close();

	font.loadFromFile("font/8-bitArcadeIn.ttf");

	header_text.setFont(font);
	header_text.setString("000000000");
	header_text.setCharacterSize(backDim[1]);
	textRect = header_text.getLocalBounds();
	header_text.setOrigin(textRect.left + textRect.width/2, textRect.top  + textRect.height/2.0f);	

	point_text.setFont(font);
	point_text.setString("000000000");
	point_text.setCharacterSize(backDim[1]/1.5);
	textRect = point_text.getLocalBounds();
	point_text.setOrigin(textRect.left, textRect.top  + textRect.height/2.0f);
}

void Records::input()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		isExiting = true;
}

void Records::draw(sf::RenderWindow& win)
{
	int j = 0;
	int k = 0;
	win.draw(background);
	point_text.setCharacterSize(backDim[1]/1.40);
	point_text.setString("press ESC to exit");
	point_text.setPosition(wx/12, wy-wx/28);
	win.draw(point_text);
	point_text.setCharacterSize(backDim[1]/1.5);
	for(int i = 0; i < 8; ++i) {
		if(i == 0) {
			background_rect_category.setPosition(wx*0.08, wy/8);
			win.draw(background_rect_category);
			header_text.setPosition(background_rect_category.getPosition() + sf::Vector2f(backDim[0]/2, backDim[1]/2));
			header_text.setString(titles[1]);
			win.draw(header_text);
		}else if(i == 3){
			background_rect_category.setPosition(wx*0.08*2 + backDim[0], wy/8);
			win.draw(background_rect_category);
			header_text.setPosition(background_rect_category.getPosition() + sf::Vector2f(backDim[0]/2, backDim[1]/2));
			header_text.setString(titles[0]);
			win.draw(header_text);
		}

		if(i < 4) {
			background_rect_points.setPosition(wx*0.08, backDim[1]*3 + backDim[1]*j);
			point_text.setPosition(background_rect_points.getPosition() + sf::Vector2f(backDim[0]/30, backDim[1]/2));
			point_text.setString(titles[2+k] + points[k]);
		}else {
			background_rect_points.setPosition(wx*0.08*2 + backDim[0], -backDim[1] + backDim[1]*j);
			point_text.setPosition(background_rect_points.getPosition() + sf::Vector2f(backDim[0]/30, backDim[1]/2));
			point_text.setString(titles[2+k] + points[4 + k]);
		}
		j++;
		k++;
		if(k == 4) k = 0;
		win.draw(background_rect_points);
		win.draw(point_text);
	}
}

int Records::response()
{
	if(isExiting) return 1;
	return 0;
}
