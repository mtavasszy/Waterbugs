#include "creature.h"
#include<random>

Creature::Creature(sf::Vector2f p) {
	energy = 0;
	radius = 5;
	position = p;
	velocity = sf::Vector2f(0, 0);
	direction = 0;
}

void Creature::simulateStep() {
	// simulate behaviour - sensor to brain to actuator

	if (energy > 99) { // reproduction
		energy -= 80;
		reproduceFlag = true;
	}

	// simulate actuators
	energy += 1; // chloroplast, constant energy gain

	// death
	age += 1;
	if (energy < 0 || age > 200) {
		deathFlag = true;
	}
}

Creature Creature::createOffspring()
{
	reproduceFlag = false;
	Creature offspring = Creature(position + getRandomOffset());
	// mutate

	return offspring;
}

sf::Vector2f Creature::getRandomOffset()
{
	std::random_device rd;  // Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(-1.f, 1.f);
	return sf::Vector2f(dis(gen), dis(gen));
}

sf::Color lerp(sf::Color x, sf::Color y, float a) {
	int r = int(x.r * (1-a) + y.r * a);
	int g = int(x.g * (1-a) + y.g * a);
	int b = int(x.b * (1-a) + y.b * a);

	return sf::Color(r, g, b);
}

sf::Color Creature::getColor()
{
	sf::Color deadColor(50, 50, 0);
	sf::Color youngColor;

	switch (type) {
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
	return lerp(youngColor, deadColor, ageRatio);
}

void Creature::draw(sf::RenderWindow& window) {
	sf::CircleShape circle = sf::CircleShape(radius);
	circle.setPointCount(20);
	circle.setPosition(position - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	circle.setFillColor(getColor());
	window.draw(circle);
}