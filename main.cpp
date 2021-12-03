#include <SFML/Graphics.hpp>
#include <sstream>
#include "simulation.h"

int main()
{
    int targetFPS = 60;
    float fps = targetFPS;
    float frameCount = 0;
    sf::Vector2f windowSize(1280, 720);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Waterbugs", sf::Style::Default, settings);
    window.setFramerateLimit(targetFPS);
    sf::Color clearColor(14, 135, 204, 255);

    Simulation simulation(windowSize);

    sf::Clock clock;

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
        
        // Update title info
        std::string creatureCountStr = std::to_string(simulation.creatures.size()) + " creature" + (simulation.creatures.size() > 1 ? "s" : "");

        frameCount++;
        if (clock.getElapsedTime().asSeconds() >= 1.f) {
            fps = frameCount / clock.restart().asSeconds();
            frameCount = 0;
        }
        std::ostringstream out;
        out.precision(2);
        out << std::fixed << fps;
        std::string fpsStr = "fps: " + out.str();

        window.setTitle("Waterbugs, " + creatureCountStr + ", " + fpsStr);
    }

    return 0;
}