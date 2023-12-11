#include "Loading.h"

void Loading::render(FT_Library ft) {
    RenderPlanet();
	Planets(10);
	addParticle(10, 380, 260, 60, 150);
	RenderUtils::getInstance().drawTexture("C:/Users/User/Desktop/APISystem/DKIT/Project1/img/logo.png", 220, 170, 400, 250);
}

void Loading::init() {
	
}

void Loading::close()
{
}

void Loading::mouseClicked(int button, int action)
{
}

void Loading::keyTyped()
{
}
