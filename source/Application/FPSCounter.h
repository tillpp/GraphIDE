#include "ApplicationFeature.h"
#include <SFML/System.hpp>
#include <mutex>

class FPSCounter
	:public ApplicationFeature
{
	//mulithreading
	std::recursive_mutex mutex;

	sf::Clock clock;
	int elapsedFrames = 0;
public:
	FPSCounter();
	~FPSCounter();

	void init(Application*)override {}
	void draw(Application*)override;
	void update(Application*)override;
	
};
