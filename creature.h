#ifndef CREATURE_H_ 
#define CREATURE_H_

#include <SFML/Graphics.hpp>

class Creature {

public:
	Creature(sf::Vector2f p);
	
	void simulateBehaviour();

	Creature createOffspring();
	sf::Vector2f getRandomOffset();
	float getCreationCost();

	sf::Color getColor();
	void draw(sf::RenderWindow &window);

	// flags
	bool reproduceFlag = false;
	bool deathFlag = false;

	// characteristics
	int energy;
	int maxEnergy;
	int radius;
	float age = 0;
	float maxAge;
	float lifeCost;

	enum Type {PLANT, HERBIVORE, CARNIVORE, OMNIVORE, DETRITIVORE};
	Type type = Type::PLANT;
	
	// location and movement
	float direction;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f force;
};

#endif  // CREATURE_H_