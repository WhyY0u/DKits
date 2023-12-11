#pragma once
#include "Menu.h"
#include "Loading.h"
#include "MouseHelper.h"
#include <glfw3.h>
#include <Windows.h>
enum type {
	Close,
	Recover,
	RollUp
};
typedef struct {
	Color Color, HoverColor;
	type typ;
	float width, height;
	float x, y;
	const char* image;
}Button;

class Windows {
public:
bool outline;
std::string name, logo;
Color color, outlinecolor;
Windows() {}
static Windows& getInstance() {
	static Windows instance;
	return instance;
}
GLFWwindow* createWindow(float x, float y, float screenWidth, float screenHeight, Color color, bool outline, const char* name, const char* logo, FT_Library ft) {
	this->x = x;
	this->y = y;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->rounded = 0.0f;
	this->outline = outline;
	this->logo = logo;
	this->name = name;
	this->color = color;
	this->ft = ft;
	this->outlinecolor = Color("#FFFFFF");
	BarStep = 25.0f;
	RenderUtils::getInstance().screenWidth = screenWidth;
	RenderUtils::getInstance().screenHeight = screenHeight;

	EnableFontRender();
	addButtonBar(7, 15, 2, "C:/Users/User/Desktop/APISystem/DKIT/Project1/img/rollup.png", Color("#2E2E2E"), Color("#B7B0B0"), RollUp);
	addButtonBar(0, 15, 15, "C:/Users/User/Desktop/APISystem/DKIT/Project1/img/recover.png", Color("#2E2E2E"), Color("#B7B0B0"), Recover);
	addButtonBar(0, 15, 15, "C:/Users/User/Desktop/APISystem/DKIT/Project1/img/close.png", Color("#2E2E2E"), Color("#9B3030"), Close);
	return glfwCreateWindow(screenWidth, screenHeight, this->name.c_str(), nullptr, nullptr);
}
void DisableConsole() {
FreeConsole();
HWND console = GetConsoleWindow();
SendMessage(console, WM_CLOSE, 0, 0);
}

void tick() {
	MouseUpdate();
	screenWidth = RenderUtils::getInstance().screenWidth;
	screenHeight = RenderUtils::getInstance().screenHeight;

}
void mouseClicked(int button, int action) {
	if (button == 0 && action == 1) {
		mouseClickedBar();
		std::cout << "Cliced" << std::endl;
	}
	scene->mouseClicked(button, action);

}
void init() {
	RenderUtils::getInstance().initRenderUtils();
}
void Render2D() {
	if (outline) {
		RenderUtils::getInstance().Rect(0, 0, screenWidth, screenHeight, outlinecolor, outlinecolor, outlinecolor, outlinecolor, rounded, 1.0f);
		RenderUtils::getInstance().Rect(1, 1, screenWidth - 2, screenHeight - 2, color, color, color, color, rounded, 1.0f);
	}
	else {
		RenderUtils::getInstance().Rect(0, 0, screenWidth, screenHeight, color, color, color, color, rounded, 1.0f);
	}
	setBarX(screenWidth - 90);
	setBarY(20.0f);
	renderBar();
	scene->render(ft);
}
GLFWwindow* getWindows() {
	return window;
}
Scene* getScene() {
	return scene;
}
bool EnableFontRender() {
	if (FT_Init_FreeType(&ft)) {
		std::cout << "Error FT" << std::endl;
		glfwTerminate();
		return true;
	}
	return false;
}
void setScene(Scene* scene) {
	this->scene = scene;
}
bool EnableGLFW() {
	if (!glfwInit()) {
		std::cout << "GLFW NotLoaded" << std::endl;
		return true;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	return false;
}
void clear() {
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	std::cout << "Succes Clear Windows" << std::endl;
}
void setRounded(float rounded) {
	this->rounded = rounded;
}
void setRoundedAndSave(float rounded) {
	this->oldrounded = this->rounded;
	this->rounded = rounded;
}
void setOldRounded() {
	this->rounded = oldrounded;
}
void setWindows(GLFWwindow* windows) {
	this->window = windows;
}
float* ScreenSize() {
	return new float[screenWidth, screenHeight];
}
void setBarX(float BarX) {
	this->BarX = BarX;
}
void setWindowsSize(float width, float height) {
	HWND hwnd = GetActiveWindow();
	SetWindowPos(hwnd, NULL, 0, 0, width, height, SWP_NOZORDER);
	glViewport(0, 0, width, height);
	RenderUtils::getInstance().screenHeight = height;
	RenderUtils::getInstance().screenWidth = width;

}
void setBarY(float BarY) {
	this->BarY = BarY;
}
void setBarStep(float BarStep) {
	this->BarStep = BarStep;
}
bool getFullScreen() {
	return FullScreen;
}
void setEnableRecover(bool recover) {
	this->EnableRecover = recover;
}
FT_Library ft;

private:
bool FullScreen;
float BarX, BarY, BarStep;
int oldBarWidth, oldBarHeight, oldBarX, oldBarY;
bool EnableRecover;
std::vector<Button> buttons;
void renderBar() {
	float x2 = 0;
	for (unsigned int i = 0; i < buttons.size(); i++) {
		Button b = buttons[i];
		switch (b.typ) {

		case Close:
			RenderUtils::getInstance().drawTextureColor("C:/Users/User/Desktop/APISystem/DKIT/Project1/img/close.png", BarX + x2, BarY + b.y, b.width, b.height, MouseHelper::getInstance().hover(BarX + x2, BarY + b.y, b.width, b.height) ? b.HoverColor : b.Color);
			b.x = x2;
			break;

		case Recover:
			RenderUtils::getInstance().drawTextureColor("C:/Users/User/Desktop/APISystem/DKIT/Project1/img/recover.png", BarX + x2, BarY + b.y, b.width, b.height, EnableRecover ? (MouseHelper::getInstance().hover(BarX + x2, BarY + b.y, b.width, b.height) ? b.HoverColor : b.Color)  :  Color("#1A1919"));
			b.x = x2;
			break;

		case RollUp:
			RenderUtils::getInstance().drawTextureColor("C:/Users/User/Desktop/APISystem/DKIT/Project1/img/rollup.png", BarX + x2, BarY + b.y, b.width, b.height, MouseHelper::getInstance().hover(BarX + x2, BarY + b.y, b.width, b.height) ? b.HoverColor : b.Color);
			b.x = x2;
			break;
		}
		x2 += BarStep;
	}
}
bool mouseClickedBar() {
	float x2 = 0;
	HWND hwnd = GetActiveWindow();
	for (unsigned int i = 0; i < buttons.size(); i++) {
		Button b = buttons[i];
		if (MouseHelper::getInstance().hover(BarX + x2, BarY + b.y, b.width, b.height)) {
			std::cout << "Close" << std::endl;

			if (b.typ == Close) {

				DestroyWindow(hwnd);
				HWND console = GetConsoleWindow();
				SendMessage(console, WM_CLOSE, 0, 0);
				return true;
			}
			if (b.typ == Recover && EnableRecover) {
				FullScreen = !FullScreen;
				if (FullScreen) {
					RECT windowRect;
					GetWindowRect(hwnd, &windowRect);
					oldBarX = windowRect.left;
					oldBarY = windowRect.top;
					oldBarWidth = RenderUtils::getInstance().screenWidth;
					oldBarHeight = RenderUtils::getInstance().screenHeight;
					int FW = GetSystemMetrics(SM_CXSCREEN);
					int FH = GetSystemMetrics(SM_CYSCREEN);
					RenderUtils::getInstance().UpdateRenderInfo(FW, FH);
					SetWindowPos(hwnd, NULL, 0, 0, FW, FH, SWP_NOZORDER);
					glViewport(0, 0, FW, FH);
					setRoundedAndSave(0.0f);
				}
				else {
					RenderUtils::getInstance().UpdateRenderInfo(oldBarWidth, oldBarHeight);
					SetWindowPos(hwnd, NULL, oldBarX, oldBarY, oldBarWidth, oldBarHeight, SWP_NOZORDER);
					glViewport(0, 0, oldBarWidth, oldBarHeight);
					setOldRounded();
				}


				return true;
			}
			if (b.typ == RollUp) {
				ShowWindow(hwnd, SW_MINIMIZE);
				return true;
			}
		}
		x2 += BarStep;

	}
	return false;
}
void addButtonBar(float y, float width, float height, const char* image, Color color, Color hover, type typ) {

	Button* b = new Button();
	b->Color = color;
	b->HoverColor = hover;
	b->typ = typ;
	b->width = width;
	b->height = height;
	b->y = y;
	b->image = image;

	buttons.push_back(*b);

}
void MouseUpdate() {
glfwGetCursorPos(getWindows(), &MouseHelper::getInstance().mouseX, &MouseHelper::getInstance().mouseY);
}
float x, y;
float oldrounded, rounded;
float screenWidth, screenHeight;
GLFWwindow* window;
Scene* scene;
Windows(const Windows&) = delete;
void operator=(const Windows&) = delete;
};