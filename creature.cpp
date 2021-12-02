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

void Creature::draw(sf::RenderWindow& window) {
	sf::CircleShape circle = sf::CircleShape(radius);
	circle.setPointCount(20);
	circle.setPosition(position - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	circle.setFillColor(sf::Color(std::min(int(float(energy) * 2.55f), 255), std::min(155 + energy, 255), 0, 255));
	if (deathFlag)
		circle.setFillColor(sf::Color(50, 50, 0, 255));
	window.draw(circle);
}