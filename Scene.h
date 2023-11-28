#pragma once
#include "RenderManager2D.h"
class Scene {
public:
virtual void render(FT_Library ft);
virtual void init();
virtual void close();
virtual void mouseClicked();
virtual void keyTyped();
};