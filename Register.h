#include "Scene.h"
class Register : public Scene {
public:
	Register(std::string name) : Scene(name) {};
	void render(FT_Library ft) override;
	void init() override;
	void close() override;
	void mouseClicked(int button, int action) override;
	void keyTyped() override;
};