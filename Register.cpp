#pragma once
#include "Register.h"
#include "WindowsHelper.h"
void Register::render(FT_Library ft) {
	RenderUtils::getInstance().drawTextureAttenuation("C:/Users/User/Desktop/APISystem/DKIT/Project1/img/logo.png", 260, 125, 270, 160, 3.0f, 17.1f, 8);
	//RenderUtils::getInstance().drawText(220, 54, InterBold, L"Dissimilate", ft, 19, Color("#FFFFFF"), 1.0f);
}

void Register::init() {
	//Windows::getInstance().setWindowsSize(500, 607);
	Windows::getInstance().setEnableRecover(false);
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
