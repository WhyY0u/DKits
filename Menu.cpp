#pragma once
#include "Menu.h"
void Menu::render(FT_Library ft) {
	RenderUtils::getInstance().drawTexture("C:/Users/User/Desktop/APISystem/DKIT/Project1/img/qr.png", 50, 50, 260, 260);
	RenderUtils::getInstance().drawText(55, 350, InterLight, L"Сканируйте QR в мобильном", ft, 18, Color("#807474"), 1.0f);
	RenderUtils::getInstance().drawText(61, 373, InterLight, L"приложения Dissimilate для", ft, 18, Color("#807474"), 1.0f);
	RenderUtils::getInstance().drawText(115, 396, InterLight, L"входа в аккунт", ft, 18, Color("#807474"), 1.0f);
	RenderUtils::getInstance().RectHvoer(55, 430, 255, 50, Color("#0E42F8"), Color("#0E42F8"), Color("#0E42F8"), 0.03f, 1.0f);
	RenderUtils::getInstance().RectHvoer(55, 500, 255, 50, Color("#212121"), Color("#212121"), Color("#212121"), 0.03f, 1.0f);

	
}

void Menu::init()
{
}

void Menu::close()
{
}

void Menu::mouseClicked()
{
}

void Menu::keyTyped()
{
}
