#include <SFML/Graphics.hpp>
#include "simulation.h"

int main()
{
    int targetFPS = 60;
    sf::Vector2f windowSize(1280, 720);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Waterbugs", sf::Style::Default, settings);
    window.setFramerateLimit(targetFPS);
    sf::Color clearColor(14, 135, 204, 255);

    Simulation simulation(windowSize);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        simulation.simulateStep();
        window.clear(clearColor);
        simulation.draw(window);
        window.display();
    }

    return 0;
}