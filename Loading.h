#pragma once
#include "Scene.h"
#include <math.h>
#include "Math.h"
#include<cstdlib>
#include "Random.h"
typedef struct {
	vec2 pos;
	float alpha;
	float width;
}Particle;
typedef struct {
	vec2 position;
	std::string figure;
	float rotate;
	Color color;
}Planet;
class Loading : public Scene {
public:
	Loading(std::string name) : Scene(name){
		init();
	};
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
	void addParticle(float max, float x, float y, float height, float x2) {
		if (Particles.size() < max) {
			if (Particles.size() == 0) {
				Particle* part = new Particle();
				part->pos = vec2(0, Random::getInstance().getRandomFloat(y, y + height));
				part->alpha = 1.0f;
				part->width = Random::getInstance().getRandomFloat(15, 40);
				Particles.push_back(part);
			} else {
				Particle* p = Particles.back();
				if (std::abs((p->pos.x + x) - x) >= 30) {
					float ys = Random::getInstance().getRandomFloat(y, y + height);
					if (std::abs(p->pos.y - ys) >= 15) {
						Particle* part = new Particle();
						part->pos = vec2(0, ys);
						part->alpha = 1.0f;
						part->width = Random::getInstance().getRandomFloat(15, 40);
						Particles.push_back(part);
					}

				}


			}
			
	  }
		for (int i = 0; i < Particles.size(); i++) {
			Particle* partic = Particles[i];
			RenderUtils::getInstance().drawImageAlpha("C:/Users/User/Desktop/APISystem/DKIT/Project1/img/particle.png", (-partic->pos.x) + x, partic->pos.y, partic->width, 8, partic->alpha);
			partic->pos.x += 6;
			float alpha = (0.0 - 1.0) / (x2 - 0.0) * (partic->pos.x - 0.0) + 1.0;
			partic->alpha = alpha;
			if (-partic->pos.x < (-x2 - 130))  Particles.erase(Particles.begin() + i);
		}
	}
	

};