#include "ComplexPlane.h"

int main()
{
    //Get the desktop resolution using:
    //VideoMode::getDesktopMode().width;
    //VideoMode::getDesktopMode().height;
    //Note:  you may want to divide these values by 2 to start with a smaller screen so your program runs faster
    int pixelWidth = VideoMode::getDesktopMode().width;
    int pixelHeight = VideoMode::getDesktopMode().height;

    //Construct the RenderWindow
    VideoMode vm(pixelWidth, pixelHeight);
    RenderWindow window(vm, "Mandelbrot Set", Style::Default);

    //Construct the ComplexPlane
    ComplexPlane plane(pixelWidth, pixelHeight);

    //Construct the Font and Text objects
    Text text;
    Font font;

    font.loadFromFile("SparkyStonesRegular-BW6ld.ttf");
    text.setFont(font);

    text.setCharacterSize(20);
    text.setFillColor(Color::White);
    text.setStyle(Text::Italic);
    text.setPosition(10.0, 10.0);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			// handle input segment
			Vector2i current_pos(event.mouseButton.x, event.mouseButton.y);
			if (event.type == Event::Closed)
			{
				// Quit the game when the window is closed
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					plane.zoomIn();
					plane.setCenter(current_pos);
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					plane.zoomOut();
					plane.setCenter(current_pos);
				}
			}
			if (event.type == sf::Event::MouseMoved)
			{
				plane.setMouseLocation(current_pos);
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				window.close();
			}

			//update scene segment
			plane.updateRender();
			plane.loadText(text);

			//draw scene segment
			window.clear();
			window.draw(plane);
			window.draw(text);
			window.display();
		}
	}
}
