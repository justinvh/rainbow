#include <functional>
#include <random>

#include <rainbow/math/random.hpp>

using namespace rb;
using namespace std;

namespace {
mt19937 engine;
std::random_device rd;
}

void rb::random_seed()
{
    engine.seed(rd());
}

float rb::random_scale()
{
    static uniform_real_distribution<float> distribution(0.0, 1.0);
    static auto generator = std::bind(distribution, engine);
    return generator();
}

float rb::random_range(float low, float high)
{
    uniform_real_distribution<float> distribution(low, high);
    auto generator = std::bind(distribution, engine);
    return generator();
}

int rb::random_range(int low, int high)
{
    uniform_int_distribution<int> distribution(low, high);
    auto generator = bind(distribution, engine);
    return generator();
}
