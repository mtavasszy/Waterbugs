#include "creature.h"


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
	}

	// simulate actuators
	energy += 1;
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