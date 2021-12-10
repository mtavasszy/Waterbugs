#ifndef PLANT_H_ 
#define PLANT_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "Vec2.h"

class Plant {

public:
	Plant(Vec2f p, float r);

	void initRandom();
	float rnd();

	void simulateBehaviour(float dt);

	Plant createOffspring();
	Vec2f getRandomOffset(float newradius);
	float getCreationCost();

	sf::Color getColor();
	void draw(sf::RenderWindow &window);

	// random
	std::mt19937 gen; // Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis;

	// flags
	bool reproduceFlag = false;
	bool deathFlag = false;

	// characteristics
	float mass;
	float energy;
	float maxEnergy;
	float radius;
	float size;
	float age = 0;
	float maxAge;
	float lifeCost;
	
	// location and movement
	float direction;
	Vec2f position;
	Vec2f velocity;
	Vec2f force;

	// appendages
	int chloroplastCount;
};

#endif  // PLANT_H_