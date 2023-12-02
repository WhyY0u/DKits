#include <cstdlib>
#include <ctime>
class Random {
public:
static Random& getInstance() {
		static Random random;
		return random;
}
int getRandomInt(int min, int max) {
    static bool initialized = false;
    if (!initialized) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        initialized = true;
    }
    int range = max - min + 1;
    int random_int = std::rand() % range + min;
    return random_int;
}

float getRandomFloat(float min, float max) {
    static bool initialized = false;
    if (!initialized) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        initialized = true;
    }
    int random_int = std::rand();
    float random = static_cast<float>(random_int) / RAND_MAX * (max - min) + min;
    return random;
}
};