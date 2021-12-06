#ifndef SIMULATION_H_ 
#define SIMULATION_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include "creature.h"

class Simulation {

public:
	Simulation(sf::Vector2f boxSize);

	void initialize();
	void simulateStep();
	void simulateBehaviour();
	void checkFlags();
	void resolvePhysics();
	void checkCollision(Creature *c0, Creature *c1);
	void draw(sf::RenderWindow& window);

	sf::Vector2f boxSize;
	std::vector<Creature> creatures;
};

#endif  // SIMULATION_H_