#ifndef SIMULATION_H_ 
#define SIMULATION_H_

#include <vector>
#include "creature.h"

class Simulation {

public:
	Simulation();

	void initialize();
	void simulateStep();
	void simulateBehaviour();
	void applyReproduction();
	void resolvePhysics();
	void draw(sf::RenderWindow& window);

	std::vector<Creature> creatures;
};

#endif  // SIMULATION_H_