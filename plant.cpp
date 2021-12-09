#include "plant.h"
#include <random>
#include "config.h"

Plant::Plant(sf::Vector2f p, float r) {
	initRandom();

	position = p;
	radius = r;
	size = r * r;

	energy = 0;
	maxEnergy = radius * radius;
	maxAge = radius * radius * Config::ageSizeFactor;

	velocity = sf::Vector2f(0, 0);
	direction = rnd() * 2 * PI; // TODO randomize
	lifeCost = 0.f; // TODO should depend on size/type
}

void Plant::initRandom()
{
	std::random_device rd;  // Will be used to obtain a seed for the random number engine
	gen = std::mt19937(rd()); // Standard mersenne_twister_engine seeded with rd()
	dis = std::uniform_real_distribution<>(0.f, 1.f);
}

float Plant::rnd()
{
	return float(dis(gen));
}

void Plant::simulateBehaviour() {
	// constants
	energy -= lifeCost;
	age += 1;

	// Constant energy from cloroplasts
	energy += chloroplastCount * Config::chloroplastGain;

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

Plant Plant::createOffspring()
{
	reproduceFlag = false;
	
	float newradius = radius;
	Plant offspring = Plant(position + getRandomOffset(newradius), radius);
	offspring.chloroplastCount = 1;
	// TODO mutate
	// TODO type depends on mutation

	energy -= offspring.getCreationCost();
	return offspring;
}

sf::Vector2f Plant::getRandomOffset(float newradius)
{
	float a = 2 * PI * rnd();
	return sf::Vector2f(cos(a), sin(a)) * (radius+newradius) * 0.9f;
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

	sf::Color youngColor = sf::Color(170, 219, 30);;

	// interpolate with age
	float ageRatio = age / maxAge;
	return mixColors(youngColor, deadColor, ageRatio * ageRatio);
}

void Plant::draw(sf::RenderWindow& window) {
	//for (Mouth m : mouths) {
	//	m.draw(window, position, direction, radius);
	//}

	sf::CircleShape circle = sf::CircleShape(radius);
	circle.setPointCount(20);
	circle.setPosition(position - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	circle.setFillColor(getColor());
	window.draw(circle);
}