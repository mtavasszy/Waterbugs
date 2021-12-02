#ifndef CREATURE_H_ 
#define CREATURE_H_

#include <SFML/Graphics.hpp>

class Creature {

public:
	Creature(sf::Vector2f p);
	void simulateStep();
	void draw(sf::RenderWindow &window);

	float energy;
	float size;

	float direction;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f force;
};

#endif  // CREATURE_H_