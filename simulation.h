#ifndef SIMULATION_H_ 
#define SIMULATION_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include "plant.h"

class Simulation {

public:
	Simulation(sf::Vector2f boxSize);

	void initialize();
	void update(float dt);
	void simulateBehaviour(float dt);
	void checkFlags();
	void resolveCollisions();
	void checkCollision(Plant *a, Plant *b, int i, int j);
	bool checkEat(Plant* a, Plant* b, int i, int j);
	void applyWaterForce();
	void applyMotion(float dt);
	void draw(sf::RenderWindow& window);

	sf::Vector2f boxSize;
	std::vector<Plant> plants;
};

#endif  // SIMULATION_H_