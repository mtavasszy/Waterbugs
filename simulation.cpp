#include "simulation.h"
#include "plant.h"

#include <SFML/Graphics.hpp>
#include "config.h"

Simulation::Simulation(sf::Vector2f boxSize)
{
	this->boxSize = boxSize;
	initialize();
}

void Simulation::initialize()
{
	Plant plant(sf::Vector2f(640, 360), 5.f);
	plant.chloroplastCount = 1;
	plants.push_back(plant);
}

void Simulation::update(float dt)
{
	simulateBehaviour(dt);
	checkFlags();
	resolveCollisions(); // TODO multiple steps per frame?
	applyWaterForce();
	applyMotion(dt);
}

void Simulation::simulateBehaviour(float dt)
{
	for (int i = 0; i < plants.size(); i++) {
		Plant* p = &plants[i];
		if (!p->deathFlag) {
			p->simulateBehaviour(dt);
		}
	}
}
// Check plants for reproduce flag, if true add offspring to creature list
void Simulation::checkFlags()
{
	for (int i = 0; i < plants.size(); i++) {
		Plant* p = &plants[i];
		if (p->reproduceFlag) {
			plants.push_back(p->createOffspring());
		}
	}
}

void Simulation::resolveCollisions()
{
	for (int i = 0; i < plants.size(); i++) {
		Plant* a = &plants[i];

		// collisions with other plants
		for (int j = i + 1; j < plants.size(); j++) {
			Plant* b = &plants[j];
			checkCollision(a, b, i, j);
		}

		// walls
		if (a->position.x < a->radius) {
			a->position.x = a->radius;
			a->velocity.x = 0;
		}
		if (a->position.y < a->radius) {
			a->position.y = a->radius;
			a->velocity.y = 0;
		}
		if (a->position.x > boxSize.x - a->radius) {
			a->position.x = boxSize.x - a->radius;
			a->velocity.x = 0;
		}
		if (a->position.y > boxSize.y - a->radius) {
			a->position.y = boxSize.y - a->radius;
			a->velocity.y = 0;
		}
	}
}

// Check for a collision, and resolve eating and/or physics
void Simulation::checkCollision(Plant* a, Plant* b, int i, int j)
{
	float dx = a->position.x - b->position.x;
	float dy = a->position.y - b->position.y;
	float dSqr = dx * dx + dy * dy;

	float rSum = a->radius + b->radius;
	float rSumSqr = rSum * rSum;

	if (dSqr < rSumSqr) {
		// If they don't eat each other, resolve collision
		//if (!checkEat(a, b, i, j) && !checkEat(b, a, j, i)) { // TODO animal only

			// push each other away
			sf::Vector2f midpoint = (a->position + b->position) * 0.5f;
			sf::Vector2f d_a = a->position - midpoint;
			sf::Vector2f d_b = b->position - midpoint;

			float dist_a = sqrtf(d_a.x * d_a.x + d_a.y * d_a.y);
			float dist_b = sqrtf(d_b.x * d_b.x + d_b.y * d_b.y);
			float corr_a = rSum * 0.5f - dist_a;
			float corr_b = rSum * 0.5f - dist_b;
			a->velocity += (d_a / dist_a) * corr_a;
			b->velocity += (d_b / dist_b) * corr_b;
		//}
	}
}

// Check if Plant a eats Plant b, return true if this is the case
bool Simulation::checkEat(Plant* a, Plant* b, int i, int j)
{
	if (/*!a->mouths.empty() && */a->size > b->size * Config::eatSizeFactor) { // TODO check mouth direction
		// Herbivore eats plant
		a->energy += (b->getCreationCost() + b->energy) * Config::digestionEffiency;
		// Remove b
		plants[j] = plants[plants.size() - 1];
		plants.pop_back();

		return true;
	}

	return false;
}

void Simulation::applyWaterForce()
{
}

void Simulation::applyMotion(float dt)
{
	// apply motion
	for (int i = 0; i < plants.size(); i++) {
		plants[i].position += plants[i].velocity * dt;
	}
}



void Simulation::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < plants.size(); i++) {
		Plant* p = &plants[i];
		p->draw(window);
	}
}
