#pragma once
#include "GLFWHelper.h"
class MouseHelper{
public:
    static MouseHelper& getInstance() {
        static MouseHelper instance;
        return instance;
}
    double mouseX, mouseY;
    bool hover(float x, float y, float width, float height) {
        return (x <= mouseX && y <= mouseY && (x + width) >= mouseX && (y + height) >= mouseY);
    }
    
private:
MouseHelper(){}
MouseHelper(const MouseHelper&) = delete;
void operator=(const MouseHelper&) = delete;
};
