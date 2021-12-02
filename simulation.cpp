#include "simulation.h"

Simulation::Simulation()
{
	initialize();
}

void Simulation::initialize()
{
	Creature c(sf::Vector2f(640, 360));
	creatures.push_back(c);
}

void Simulation::simulateStep()
{
	simulateBehaviour();
	applyReproduction();
	resolvePhysics();
}
void Simulation::simulateBehaviour()
{	
	for (int i = 0; i < creatures.size(); i++) {
		Creature* c = &creatures[i];
		c->simulateStep();
	}
}
// Check creatures for reproduce flag, if true add offspring to creature list
void Simulation::applyReproduction()
{
	for (int i = 0; i < creatures.size(); i++) {
		Creature* c = &creatures[i];
		if (c->reproduceFlag) {
			creatures.push_back(c->createOffspring());
		}
	}
}

void Simulation::resolvePhysics()
{
	// resolve collisions etc
}

void Simulation::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < creatures.size(); i++) {
		Creature* c = &creatures[i];
		c->draw(window);
	}
}
