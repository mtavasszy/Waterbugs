#ifndef SIMULATION_H_ 
#define SIMULATION_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include "plant.h"

class Simulation {

public:
	Simulation(sf::Vector2f boxSize);

	void initialize();
	void simulateStep();
	void simulateBehaviour();
	void checkFlags();
	void resolvePhysics();
	void checkCollision(Plant *a, Plant *b, int i, int j);
	bool checkEat(Plant* a, Plant* b, int i, int j);
	void draw(sf::RenderWindow& window);

	sf::Vector2f boxSize;
	std::vector<Plant> plants;
};

#endif  // SIMULATION_H_