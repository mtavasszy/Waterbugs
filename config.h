#ifndef CONFIG_H_ 
#define CONFIG_H_

class Config {

public:
	// type creation costs
	static constexpr float plantCost = 80;

	// limb creation costs
	static constexpr float chloroplastGain = 1;
	static constexpr float moveCost = 1;


private:
	Config() {};
};

#endif  // CONFIG_H_