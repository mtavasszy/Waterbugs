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
	checkFlags();
	resolvePhysics();
}
void Simulation::simulateBehaviour()
{	
	for (int i = 0; i < creatures.size(); i++) {
		Creature* c = &creatures[i];
		if (!c->deathFlag) {
			c->simulateStep();
		}
	}
}
// Check creatures for reproduce flag, if true add offspring to creature list
void Simulation::checkFlags()
{
	for (int i = 0; i < creatures.size(); i++) {
		Creature* c = &creatures[i];
		if (c->reproduceFlag) {
			creatures.push_back(c->createOffspring());
		}
		if (c->deathFlag) {
			//creatures[i] = creatures[creatures.size() - 1]; // creature's remains persist, dont remove for now
			//creatures.pop_back();
		}
	}
}

void Simulation::resolvePhysics()
{
	// resolve forces
	// resolve collisions
	int steps = 1;

	for (int k = 0; k < steps; k++) {
		for (int i = 0; i < creatures.size(); i++) {
			for (int j = 0; j < creatures.size(); j++) {
				if (i < j) {

					Creature* c0 = &creatures[i];
					Creature* c1 = &creatures[j];

					float dx = c0->position.x - c1->position.x;
					float dy = c0->position.y - c1->position.y;
					float dSqr = dx * dx + dy * dy;

					float rSum = c0->radius + c1->radius;
					float rSumSqr = rSum * rSum;

					if (dSqr < rSumSqr) { // colliding, resolve
						sf::Vector2f midpoint = (c0->position + c1->position) * 0.5f;
						sf::Vector2f d0 = c0->position - midpoint;
						sf::Vector2f d1 = c1->position - midpoint;

						float dist0 = sqrtf(d0.x*d0.x+d0.y*d0.y);
						float dist1 = sqrtf(d1.x*d1.x+d1.y*d1.y);
						float corr0 = rSum * 0.5 - dist0;
						float corr1 = rSum * 0.5 - dist1;
						c0->velocity += (d0 / dist0) * corr0;
						c1->velocity += (d1 / dist1) * corr1;
						
					}
				}
			}
		}
		// translation
		for (int i = 0; i < creatures.size(); i++) {
			creatures[i].position += creatures[i].velocity;
			creatures[i].velocity = sf::Vector2f(0,0);
		}
	}


}

void Simulation::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < creatures.size(); i++) {
		Creature* c = &creatures[i];
		c->draw(window);
	}
}
