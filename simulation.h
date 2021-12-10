#ifndef SIMULATION_H_ 
#define SIMULATION_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include "Vec2.h"
#include "plant.h"

class Simulation {

public:
	Simulation(Vec2f boxSize);

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

	Vec2f boxSize;
	std::vector<Plant> plants;
};

#endif  // SIMULATION_H_