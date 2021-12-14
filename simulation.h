#ifndef SIMULATION_H_ 
#define SIMULATION_H_

#include <SFML/Graphics.hpp>
#include <unordered_map>
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
	void resolveCollisions(float dt);
	void resolveCollisionsOld(float dt);
	void checkCollision(Plant *a, Plant *b, int i, int j, float dt);
	bool checkEat(Plant* a, Plant* b, int i, int j);
	void applyWaterForce();
	void applyMotion(float dt);
	void draw(sf::RenderWindow& window);

	Vec2f boxSize;
	std::unordered_map<int, std::vector<int>> collisionGrid;
	std::vector<Plant> plants;
	//std::vector<int> deleteList; // for eating
};

#endif  // SIMULATION_H_