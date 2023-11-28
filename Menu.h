#pragma once
#include "Scene.h"
class Menu : public Scene {
public:
	Menu() : Scene() {};
	void render(FT_Library ft) override;
	void init() override;
	void close() override;
	void mouseClicked() override;
	void keyTyped() override;
};