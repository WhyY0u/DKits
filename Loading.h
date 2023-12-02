#pragma once
#include "Scene.h"
#include <math.h>
#include "Math.h"
#include<cstdlib>
#include "Random.h"
typedef struct {
	float x;
	Color color;
}Particle;
typedef struct {
	vec2 position;
	std::string figure;
	float rotate;
	Color color;
}Planet;
class Loading : public Scene {
public:
	Loading(std::string name) : Scene(name) {};
	void render(FT_Library ft) override;
	void init() override;
	void close() override;
	void mouseClicked(int button, int action) override;
	void keyTyped() override;
	std::string getRandomFigure(int i) {
		switch (i) {
		case 1:
		   return "C:/Users/User/Desktop/APISystem/DKIT/Project1/img/triangele.png";
		break;
		case 2:
			return "C:/Users/User/Desktop/APISystem/DKIT/Project1/img/circle.png";
			break;

		case 3:
			return "C:/Users/User/Desktop/APISystem/DKIT/Project1/img/multistar.png";
			break;

		case 4:
			return "C:/Users/User/Desktop/APISystem/DKIT/Project1/img/star.png";
			break;

		case 5:
			return "C:/Users/User/Desktop/APISystem/DKIT/Project1/img/squad.png";
			break;

		}
		return nullptr;
	}
	std::vector<Planet*> pl;
	std::vector<Particle*> Particles;
	void Planets(float max) {
		if (pl.size() < max) {
			float y = Random::getInstance().getRandomFloat(30.0f, RenderUtils::getInstance().screenHeight - 20);
			float x = RenderUtils::getInstance().screenWidth + 1.0f;
			if (pl.size() == 0) {
				vec2 pos = vec2(x, y);
				Planet* p = new Planet();
				p->color = Color(Random::getInstance().getRandomFloat(0, 255) / 255, Random::getInstance().getRandomFloat(0, 255) / 255, Random::getInstance().getRandomFloat(0, 255) / 255);
				p->figure = getRandomFigure(Random::getInstance().getRandomInt(1, 5));
				p->position = pos;
				p->rotate = Random::getInstance().getRandomFloat(0.0f, 360.0f);
				pl.push_back(p);
			}
			else {
				Planet* p = pl.back();
				if (std::abs(p->position.x - x) >= 100) {
					vec2 pos = vec2(x, y);
					Planet* p = new Planet();
					p->color = Color("#272725");
					p->figure = getRandomFigure(Random::getInstance().getRandomInt(1, 5));
					p->position = pos;
					p->rotate = Random::getInstance().getRandomFloat(0.0f, 360.0f);
					pl.push_back(p);
				}

			}
		}
		}
	void RenderPlanet() {
		for (int i = 0; i < pl.size(); i++) {
			Planet* plaent = pl[i];
			bool check = plaent->figure == "C:/Users/User/Desktop/APISystem/DKIT/Project1/img/triangele.png";
			RenderUtils::getInstance().drawTextureColor(plaent->figure.c_str(), plaent->position.x, plaent->position.y, 50, check ? 47 : 50, plaent->color);
			plaent->position.x -= 10;
			
			if (plaent->position.x < -200)  pl.erase(pl.begin() + i);
		}
	}
	void renderParticle(float max, float x, float y, float height, float width) {
		if (Particles.size() < max) {
			
	  }
	}
};