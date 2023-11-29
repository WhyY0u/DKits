#pragma once
#include "Register.h"
#include "WindowsHelper.h"
void Register::render(FT_Library ft) {
	Windows::getInstance().setEnableRecover(true);
	std::cout << "Register" << std::endl;
}

void Register::init()
{
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
