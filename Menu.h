#pragma once
#include "Scene.h"
class Menu : public Scene {
public:
	Menu(std::string name) : Scene(name) {};
	void render() override;
	void init() override;
	void close() override;
	void mouseClicked(int button, int action) override;
	void keyTyped() override;
};