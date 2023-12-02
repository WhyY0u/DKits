#pragma once
#include "RenderManager2D.h"
class Scene {
public:
	Scene(std::string name) {
		this->name = name;
		init();
	};
virtual void render(FT_Library ft);
virtual void init();
virtual void close();
virtual void mouseClicked(int button, int action);
virtual void keyTyped();
std::string name;
};