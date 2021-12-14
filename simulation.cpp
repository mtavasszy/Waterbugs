#include "simulation.h"
#include "plant.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <SFML/Graphics.hpp>
#include "config.h"
#include "Vec2.h"

Simulation::Simulation(Vec2f boxSize)
{
	this->boxSize = boxSize;
	collisionGrid = std::unordered_map<int, std::vector<int>>();
	initialize();
}

void Simulation::initialize()
{
	Plant plant(Vec2f(640, 360), 5.f);
	plant.chloroplastCount = 1;
	plants.push_back(plant);
}

void Simulation::update(float dt)
{
	simulateBehaviour(dt);
	checkFlags();
	resolveCollisions(dt);
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

void Simulation::resolveCollisions(float dt)
{
	const float invGridCellSize = 1.f / Config::gridCellSize;

	// reset grid
	collisionGrid.clear();
	std::unordered_set<int> collidedObjs;

	for (int i = 0; i < plants.size(); i++) {
		Plant* a = &plants[i];

		// add new to grid
		int gridX0 = int((plants[i].position.x - plants[i].radius) * invGridCellSize);
		int gridX1 = int((plants[i].position.x + plants[i].radius) * invGridCellSize);
		int gridY0 = int((plants[i].position.y - plants[i].radius) * invGridCellSize);
		int gridY1 = int((plants[i].position.y + plants[i].radius) * invGridCellSize);

		bool isSingleCell = (gridX0 == gridX1) && (gridY0 == gridY1);

		if (isSingleCell) {
			int coord = gridX0 + gridY0 * int(boxSize.x * invGridCellSize);
			if (collisionGrid.count(coord) == 0) {
				// no other items in grid cell yet, add index
				collisionGrid[coord] = std::vector<int>{ i };
			}
			else {
				// list of items found, check collisions
				for (int j : collisionGrid[coord]) {
					Plant* b = &plants[j];
					checkCollision(a, b, i, j, dt);
				}
				collisionGrid[coord].push_back(i);
			}
		}
		else {
			collidedObjs.clear();

			for (int xx = gridX0; xx <= gridX1; xx++) {
				for (int yy = gridY0; yy <= gridY1; yy++) {
					int coord = xx + yy * int(boxSize.x * invGridCellSize);
					if (collisionGrid.count(coord) == 0) {
						// no other items in grid cell yet, add index
						collisionGrid[coord] = std::vector<int>{ i };
					}
					else {
						// list of items found, check collisions
						Plant* a = &plants[i];
						for (int j : collisionGrid[coord]) {
							// verify that collision with this obj has not been checked yet
							if (collidedObjs.count(j) == 0) {
								Plant* b = &plants[j];
								checkCollision(a, b, i, j, dt);
								collidedObjs.insert(j);
							}
						}
						collisionGrid[coord].push_back(i);
					}
				}
			}
		}

		// walls
		if (a->position.x < a->radius) {
			a->position.x = a->radius;
			a->velocity.x = -a->velocity.x;
		}
		if (a->position.y < a->radius) {
			a->position.y = a->radius;
			a->velocity.y = -a->velocity.y;
		}
		if (a->position.x > boxSize.x - a->radius) {
			a->position.x = boxSize.x - a->radius;
			a->velocity.x = -a->velocity.x;
		}
		if (a->position.y > boxSize.y - a->radius) {
			a->position.y = boxSize.y - a->radius;
			a->velocity.y = -a->velocity.y;
		}
	}
}

// Check for a collision, and resolve eating and/or physics
void Simulation::checkCollision(Plant* a, Plant* b, int i, int j, float dt)
{
	Vec2f dp = b->position - a->position;
	float distSqr = dp.x * dp.x + dp.y * dp.y;

	float rSum = a->radius + b->radius;
	float rSumSqr = rSum * rSum;

	if (distSqr < rSumSqr) {
		// If they don't eat each other, resolve collision
		//if (!checkEat(a, b, i, j) && !checkEat(b, a, j, i)) { // TODO animal only

		float dist = sqrt(distSqr);
		Vec2f un = dp / dist;
		Vec2f ut(-un.y, un.x);

		float an = Vec2f::dot(un, a->velocity);
		float at = Vec2f::dot(ut, a->velocity);
		float bn = Vec2f::dot(un, b->velocity);
		float bt = Vec2f::dot(ut, b->velocity);

		float at_new = at;
		float bt_new = bt;

		float an_new = (an * (a->mass - b->mass) + 2 * b->mass * bn) / (a->mass + b->mass);
		float bn_new = (bn * (b->mass - a->mass) + 2 * a->mass * an) / (a->mass + b->mass);

		Vec2f v_an_new = an_new * un;
		Vec2f v_bn_new = bn_new * un;
		Vec2f v_at_new = at_new * ut;
		Vec2f v_bt_new = bt_new * ut;

		a->velocity = v_an_new + v_at_new;
		b->velocity = v_bn_new + v_bt_new;

		// correct position
		Vec2f midpoint = (a->position + b->position) * 0.5f;
		a->position = midpoint - un * a->radius;
		b->position = midpoint + un * b->radius;
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

		// friction
		plants[i].velocity *= powf(Config::friction, dt);
	}
}



void Simulation::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < plants.size(); i++) {
		Plant* p = &plants[i];
		p->draw(window);
	}
}
