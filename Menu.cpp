#pragma once
#include "Menu.h"
#include "WindowsHelper.h"
#include "Register.h"
void Menu::render(FT_Library ft) {
	Windows::getInstance().setEnableRecover(false);
	RenderUtils::getInstance().drawTexture("C:/Users/User/Desktop/APISystem/DKIT/Project1/img/qr.png", 50, 50, 260, 260);
	RenderUtils::getInstance().drawText(55, 350, InterLight, L"Сканируйте QR в мобильном", ft, 18, Color("#807474"), 1.0f);
	RenderUtils::getInstance().drawText(61, 373, InterLight, L"приложения Dissimilate для", ft, 18, Color("#807474"), 1.0f);
	RenderUtils::getInstance().drawText(115, 396, InterLight, L"входа в аккунт", ft, 18, Color("#807474"), 1.0f);

	RenderUtils::getInstance().drawTripleGradient(55, 430, 255, 50, Color("#0E42F8"), MouseHelper::getInstance().hover(55, 430, 255, 50) ? Color("#0E8BFF") :  Color("#0E42F8"), Color("#0E42F8"), 0.01f, 1.0f);
	RenderUtils::getInstance().drawTripleGradient(55, 500, 255, 50, Color("#212121"), MouseHelper::getInstance().hover(55, 500, 255, 50) ? Color("#313131") : Color("#212121"), Color("#212121"), 0.01f, 1.0f);

	RenderUtils::getInstance().drawText(155, 461, InterLight, L"Войти", ft, 18, Color("#FFFFFF"), 1.0f);
	RenderUtils::getInstance().drawText(100, 531, InterLight, L"Зарегистрироватся", ft, 18, Color("#FFFFFF"), 1.0f);
}

void Menu::init()
{
}

void Menu::close()
{
}

void Menu::mouseClicked(int button, int action)
{
	if (MouseHelper::getInstance().hover(55, 500, 255, 50) && button == 0 && action == 1) {
		Windows::getInstance().setScene(new Register("Register"));
		std::cout << "RegisterClick" << std::endl;

	}
}

void Menu::keyTyped()
{
}
