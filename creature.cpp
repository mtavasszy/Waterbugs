#include "creature.h"
#include <random>
#include "config.h"

Creature::Creature(sf::Vector2f p) {
	position = p;

	radius = 5; // size
	energy = 0;
	maxEnergy = 4 * radius * radius;
	maxAge = 4 * radius * radius * 2; // todo config const
	velocity = sf::Vector2f(0, 0);
	direction = 0; // TODO randomize
	lifeCost = 0.f; // TODO should depend on size/type
}

void Creature::simulateBehaviour() {
	// Constant energy from cloroplasts
	if (type == Type::PLANT) {
		energy += Config::chloroplastGain;
		age += 1;
	}

	// constants
	energy -= lifeCost;

	// death
	if (energy < 0 || age > maxAge) {
		type = Type::DEAD;
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

Creature Creature::createOffspring()
{
	reproduceFlag = false;
	Creature offspring = Creature(position + getRandomOffset());
	// TODO mutate
	// TODO type depends on mutation

	energy -= offspring.getCreationCost();
	return offspring;
}

sf::Vector2f Creature::getRandomOffset()
{
	std::random_device rd;  // Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(-1.f, 1.f);
	return sf::Vector2f(dis(gen), dis(gen));
}

float Creature::getCreationCost()
{
	return Config::plantCost; // TODO let this depend on size and appendages
}

sf::Color mixColors(sf::Color x, sf::Color y, float factor) {
	int r = int(x.r * (1 - factor) + y.r * factor);
	int g = int(x.g * (1 - factor) + y.g * factor);
	int b = int(x.b * (1 - factor) + y.b * factor);

	return sf::Color(r, g, b);
}

sf::Color Creature::getColor()
{
	sf::Color deadColor(50, 50, 0);
	sf::Color youngColor;

	switch (type) {
	case Type::DEAD:
		return deadColor;
		break;
	case Type::PLANT:
		youngColor = sf::Color(170, 219, 30);
		break;
	case Type::HERBIVORE:
		youngColor = sf::Color(78, 91, 49);
		break;
	case Type::CARNIVORE:
		youngColor = sf::Color(133, 29, 45);
		break;
	case Type::OMNIVORE:
		youngColor = sf::Color(210, 176, 76);
		break;
	case Type::DETRITIVORE:
		youngColor = sf::Color(33, 46, 82);
		break;
	default:
		youngColor = deadColor;
		break;
	}

	// interpolate with age
	float ageRatio = age / maxAge;
	return mixColors(youngColor, deadColor, ageRatio * ageRatio);
}

void Creature::draw(sf::RenderWindow& window) {
	sf::CircleShape circle = sf::CircleShape(radius);
	circle.setPointCount(20);
	circle.setPosition(position - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	circle.setFillColor(getColor());
	window.draw(circle);
}