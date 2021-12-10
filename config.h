#ifndef CONFIG_H_ 
#define CONFIG_H_

#define PI 3.141592654f

class Config {

public:
	// mass
	static constexpr float plantMass = 0.5f;

	// creation costs
	static constexpr float sizeCost = 1.1f;
	static constexpr float chloroplastCost = 10.f;

	// limb creation costs
	static constexpr float chloroplastGain = 60;
	static constexpr float moveCost = 1;

	// eating
	static constexpr float eatSizeFactor = 1.2f; // creature has to be this much bigger to eat the other
	static constexpr float digestionEffiency = 0.8f;

	// age
	static constexpr float ageSizeFactor = 8.f;

	static constexpr float energySizeFactor = 1.f;

private:
	Config() {};
};

#endif  // CONFIG_H_