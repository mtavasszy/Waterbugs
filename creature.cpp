#include "creature.h"
#include<random>

Creature::Creature(sf::Vector2f p) {
	energy = 0;
	size = 5;
	position = p;
	direction = 0;
}

void Creature::simulateStep() {
	// simulate behaviour - sensor to brain to actuator

	if (energy > 100) { // reproduction
		energy -= 80;
		reproduceFlag = true;
	}

	// simulate actuators
	energy += 1;
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
	//std::uniform_real_distribution<> dis(-1.f, 1.f);
	std::uniform_real_distribution<> dis(-size, size);
	return sf::Vector2f(dis(gen), dis(gen));
}

void Creature::draw(sf::RenderWindow& window) {
	sf::CircleShape circle = sf::CircleShape(size);
	circle.setPointCount(20);
	circle.setPosition(position - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	circle.setFillColor(sf::Color(energy*2.55, 155+energy, 0, 255));
	//circle.setOutlineColor(sf::Color::Black);
	//circle.setOutlineThickness(-2.f);
	window.draw(circle);
}