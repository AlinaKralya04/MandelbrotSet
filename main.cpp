#include <sstream>
#include <vector>
#include "ComplexPlane.h"

int main()
{
    int pixelWidth = VideoMode::getDesktopMode().width;
    int pixelHeight = VideoMode::getDesktopMode().height;
    VideoMode vm(pixelWidth, pixelHeight);
    RenderWindow window(vm, "Mandelbrot Set", Style::Default);

    Text text;
    Font font;
    font.loadFromFile("SparkyStonesRegular-BW6ld.ttf");
    text.setFont(font);
    text.setCharacterSize(30); 
    text.setFillColor(Color::White);
    text.setStyle(Text::Italic);
    text.setPosition(400.f, 200.f);

    ComplexPlane plane;
    Event event;
    while (window.pollEvent(event))
    {
        // handle input segment?
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
                plane.setCenter();
                std::cout << "the left button was pressed" << std::endl;
                std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            }
            else if (event.mouseButton.button == sf::Mouse::Right)
            {
                plane.zoomOut();
                plane.setCenter();
                std::cout << "the right button was pressed" << std::endl;
                std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            }
        }
        if (event.type == sf::Event::MouseMoved)
        {
            plane.setMouseLocation();
            std::cout << "new mouse x: " << event.mouseMove.x << std::endl;
            std::cout << "new mouse y: " << event.mouseMove.y << std::endl;
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
        //update scene segment
        plane.updateRender();
        plane.loadText();
        //draw scene segemnt
        window.clear();
        plane.draw();
        window.draw(text);
        window.display();


    }
}
