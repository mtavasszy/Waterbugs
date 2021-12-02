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
	for (int i = 0; i < creatures.size(); i++) {
		Creature* c = &creatures[i];
		c->simulateStep();
	}
}

void Simulation::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < creatures.size(); i++) {
		Creature* c = &creatures[i];
		c->draw(window);
	}
}
