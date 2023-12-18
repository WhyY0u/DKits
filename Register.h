#include "Scene.h"
class Register : public Scene {
public:
	Register(std::string name) : Scene(name) {
		init();
	};
	void render() override;
	void init() override;
	void close() override;
	void mouseClicked(int button, int action) override;
	void keyTyped() override;
};