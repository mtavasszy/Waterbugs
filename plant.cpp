#include "plant.h"
#include <random>
#include "config.h"
#include "Vec2.h"

Plant::Plant(unsigned int seed, Vec2f p, float r) {
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

void Plant::initRandom(unsigned int seed)
{
	gen = std::mt19937(seed); // Standard mersenne_twister_engine seeded with rd()
	dis = std::uniform_real_distribution<>(0.f, 1.f);
}

float Plant::rnd()
{
	return float(dis(gen));
}

void Plant::simulateBehaviour(float dt) {
	// constants
	energy -= lifeCost * dt;
	age += 1.f * dt;

	// Constant energy from cloroplasts
	energy += chloroplastCount * Config::chloroplastGain * dt;

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

Plant Plant::createOffspring(unsigned int seed)
{
	reproduceFlag = false;
	
	float newradius = radius;
	Vec2f offset = getRandomOffset(newradius);
	position -= offset * 0.5;
	velocity -= Config::replicatePushSpeed * offset;
	Vec2f newPos = position + offset;
	Plant offspring = Plant(seed, newPos, radius);
	offspring.velocity = Config::replicatePushSpeed * offset;
	offspring.chloroplastCount = 1;
	// TODO mutate
	// TODO type depends on mutation

	energy -= offspring.getCreationCost();
	return offspring;
}

Vec2f Plant::getRandomOffset(float newradius)
{
	float a = 2 * PI * rnd();
	return Vec2f(cos(a), sin(a)) * (radius+newradius);
}

float Plant::getCreationCost()
{
	return Config::sizeCost * size; // TODO let this depend on size and appendages
}

sf::Color mixColors(sf::Color x, sf::Color y, float factor) {
	int r = int(x.r * (1 - factor) + y.r * factor);
	int g = int(x.g * (1 - factor) + y.g * factor);
	int b = int(x.b * (1 - factor) + y.b * factor);

	return sf::Color(r, g, b);
}

sf::Color Plant::getColor()
{
	sf::Color deadColor(50, 50, 0);
	if (deathFlag)
		return deadColor;

	sf::Color youngColor = sf::Color(170, 219, 30);

	// interpolate with age
	float ageRatio = age / maxAge;
	return mixColors(youngColor, deadColor, ageRatio * ageRatio);
}

void Plant::initDrawShape()
{
	circle = sf::CircleShape(radius);
	//circle.setPointCount(20);
	circle.setFillColor(Config::plantColor);
}

void Plant::draw(sf::RenderWindow& window) {
	const Vec2f drawPos = position - Vec2f(circle.getRadius());
	circle.setPosition(sf::Vector2f(drawPos.x, drawPos.y));
	window.draw(circle);
}