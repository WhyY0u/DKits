#pragma once
#include "Register.h"
#include "WindowsHelper.h"
void Register::render(FT_Library ft) {
	RenderUtils::getInstance().drawGlowTexture("C:/Users/User/Desktop/APISystem/DKIT/Project1/img/logo.png", 100, 0, 270, 180);
}

void Register::init() {
	Windows::getInstance().setWindowsSize(500, 607);
	Windows::getInstance().setEnableRecover(true);
	Windows::getInstance().setRounded(0.04f);

}

void Register::close()
{
}

void Register::mouseClicked(int button, int action)
{

}

void Register::keyTyped()
{
}
