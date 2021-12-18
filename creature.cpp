#include "creature.h"
#include <random>
#include "config.h"
#include "Vec2.h"

Creature::Creature(unsigned int seed, Vec2f p, float r) {
	initRandom(seed);

	position = p;
	radius = r;
	size = r * r;

	mass = size * Config::plantMass;
	energy = 0;
	maxEnergy = size * Config::energySizeFactor;
	maxAge = size * Config::ageSizeFactor;

	velocity = Vec2f(0, 0);
	direction = rnd() * 2 * PI; // TODO randomize
	lifeCost = 0.f; // TODO should depend on size/type

	initDrawShape();
}

void Creature::initRandom(unsigned int seed)
{
	gen = std::mt19937(seed); // Standard mersenne_twister_engine seeded with rd()
	dis = std::uniform_real_distribution<>(0.f, 1.f);
}

float Creature::rnd()
{
	return float(dis(gen));
}

void Creature::simulateBehaviour(float dt) {
	// constants
	energy -= lifeCost * dt;
	age += 1.f * dt;

	// death
	if (energy < 0 || age > maxAge) {
		deathFlag = true;
	}
	else { // if not dead
		// simulate behaviour - sensor to brain to actuator

		// Reproduce if possible
		if (energy >= maxEnergy) {
			energy = maxEnergy;
			reproduceFlag = true;
		}
	}
}

Creature Creature::createOffspring(unsigned int seed)
{
	reproduceFlag = false;
	
	float newradius = radius;
	Vec2f offset = getRandomOffset(newradius);
	position -= offset * 0.5;
	velocity -= Config::replicatePushSpeed * offset;
	Vec2f newPos = position + offset;
	Creature offspring = Creature(seed, newPos, radius);
	offspring.velocity = Config::replicatePushSpeed * offset;
	// TODO mutate
	// TODO type depends on mutation

	energy -= offspring.getCreationCost();
	return offspring;
}

Vec2f Creature::getRandomOffset(float newradius)
{
	float a = 2 * PI * rnd();
	return Vec2f(cos(a), sin(a)) * (radius+newradius);
}

float Creature::getCreationCost()
{
	return Config::sizeCost * size; // TODO let this depend on size and appendages
}

sf::Color Creature::mixColors(sf::Color x, sf::Color y, float factor) {
	int r = int(x.r * (1 - factor) + y.r * factor);
	int g = int(x.g * (1 - factor) + y.g * factor);
	int b = int(x.b * (1 - factor) + y.b * factor);

	return sf::Color(r, g, b);
}

sf::Color Creature::getColor()
{
	sf::Color deadColor(50, 50, 0);
	if (deathFlag)
		return deadColor;

	sf::Color youngColor = sf::Color(170, 219, 30);

	// interpolate with age
	float ageRatio = age / maxAge;
	return mixColors(youngColor, deadColor, ageRatio * ageRatio);
}

void Creature::initDrawShape()
{
	circle = sf::CircleShape(radius);
	//circle.setPointCount(20);
	circle.setFillColor(Config::creatureColor);
}

void Creature::draw(sf::RenderWindow& window) {
	const Vec2f drawPos = position - Vec2f(circle.getRadius());
	circle.setPosition(sf::Vector2f(drawPos.x, drawPos.y));
	window.draw(circle);
}