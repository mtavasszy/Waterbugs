#include "simulation.h"
#include "plant.h"

#include <SFML/Graphics.hpp>
#include "config.h"
#include "Vec2.h"

Simulation::Simulation(Vec2f boxSize)
{
	this->boxSize = boxSize;
	initialize();
}

void Simulation::initialize()
{
	//Plant plant(Vec2f(640, 360), 5.f);
	//plant.chloroplastCount = 1;
	//plants.push_back(plant);
	Plant p0(Vec2f(520, 355), 10.f);
	p0.velocity.x = 80;
	Plant p1(Vec2f(760, 365), 10.f);
	p1.velocity.x = -80;
	plants.push_back(p0);
	plants.push_back(p1);
}

void Simulation::update(float dt)
{
	simulateBehaviour(dt);
	checkFlags();
	resolveCollisions(dt); // TODO multiple steps per frame?
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
	for (int i = 0; i < plants.size(); i++) {
		Plant* a = &plants[i];

		// collisions with other plants
		for (int j = i + 1; j < plants.size(); j++) {
			Plant* b = &plants[j];
			checkCollision(a, b, i, j, dt);
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
void Simulation::checkCollision(Plant* a, Plant* b, int i, int j, float dt)
{
	Vec2f dp = b->position - a->position;
	float distSqr = dp.x * dp.x + dp.y * dp.y;

	float rSum = a->radius + b->radius;
	float rSumSqr = rSum * rSum;

	if (distSqr < rSumSqr) {
		// If they don't eat each other, resolve collision
		//if (!checkEat(a, b, i, j) && !checkEat(b, a, j, i)) { // TODO animal only
		//float dist = sqrt(distSqr);

		float dist = sqrt(distSqr);
		Vec2f dv = b->velocity - a->velocity;
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
		Vec2f d_a = a->position - midpoint;
		Vec2f d_b = b->position - midpoint;

		float dist_a = sqrtf(d_a.x * d_a.x + d_a.y * d_a.y);
		float dist_b = sqrtf(d_b.x * d_b.x + d_b.y * d_b.y);
		float corr_a = rSum * 0.5f - dist_a;
		float corr_b = rSum * 0.5f - dist_b;
		a->position += (d_a / dist_a) * corr_a;
		b->position += (d_b / dist_b) * corr_b;

		

		//Vec2f dv = b->velocity - a->velocity;
		//Vec2f normal = dp / dist;

		//// Calculate relative velocity in terms of the normal direction
		//float velAlongNormal = Vec2f::dot(dv, normal);

		//// Do not resolve if velocities are separating
		//if (velAlongNormal <= 0) {
		//	// Calculate restitution
		//	float e = 1.f;//  min(A.restitution, B.restitution) - "bouncyness"

		//	// Calculate impulse scalar
		//	float imp = ((-(1 + e) * velAlongNormal) / (a->invMass + b->invMass)) * dt;
		//	float penetration = rSum - dist;
		//	float slop = 0.01f;
		//	float percent = 0.2;

		//	Vec2f corr = std::max(penetration - slop, 0.f) / (a->invMass + b->invMass) * percent * normal;

		//	// Apply impulse
		//	a->velocity -= a->invMass * (normal * imp - corr);
		//	b->velocity += b->invMass * (normal * imp + corr);
		//}
		//}
		//Vec2f dv = b->velocity - a->velocity;
		//a->velocity -= -(2 * b->mass / (a->mass + b->mass)) * Vec2f::dot(-dv, -dp) / distSqr * -dp;
		//b->velocity -= -(2 * a->mass / (a->mass + b->mass)) * Vec2f::dot(dv, dp) / distSqr * dp;

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
		//plants[i].velocity *= powf(0.8f, dt);
	}
}



void Simulation::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < plants.size(); i++) {
		Plant* p = &plants[i];
		p->draw(window);
	}
}
