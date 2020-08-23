#include <string>

#include "window.cpp"

int main()
{
	Window window("Snake");
	while(window.isOpen()) window.windowLoop();

	return 0;
}