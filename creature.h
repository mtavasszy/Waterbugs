#ifndef CREATURE_H_ 
#define CREATURE_H_

#include <SFML/Graphics.hpp>

class Creature {

public:
	Creature(sf::Vector2f p);
	void simulateStep();
	Creature createOffspring();
	sf::Vector2f getRandomOffset();
	void draw(sf::RenderWindow &window);

	bool reproduceFlag = false;
	bool deathFlag = false;

	int energy;
	int radius;
	float age = 0;
	int maxAge;

	float direction;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f force;
};

#endif  // CREATURE_H_