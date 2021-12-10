#include <SFML/Graphics.hpp>
#include <sstream>
#include "simulation.h"

int main()
{
    float targetFPS = 60.f;
    float targetFrameTime = 1.f / targetFPS;
    float fps = float(targetFPS);
    float frameCount = 0.f;
    sf::Vector2f windowSize(1280.f, 720.f);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(int(windowSize.x), int(windowSize.y)), "Waterbugs", sf::Style::Default, settings);
    window.setFramerateLimit(int(targetFPS));
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

        simulation.update(targetFrameTime);
        window.clear(clearColor);
        simulation.draw(window);
        window.display();
        
        // Update title info
        std::string creatureCountStr = std::to_string(simulation.plants.size()) + " creature" + (simulation.plants.size() > 1 ? "s" : "");

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