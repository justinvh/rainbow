#include <rainbow/rainbow.hpp>

using namespace rb;

int main(int argc, char** argv)
{
    Python python;
    
    Python_module module;
    if (!python(&module, argv[1])) {
        std::cerr << "Failed to load module" << std::endl;
        return 2;
    }

    Python_result<int> result;
    if (!module(&result, "say", 3, "hello", 5, 6.0f, 7, 8, 9, 10)) {
        std::cerr << "Failed to call module function" << std::endl;
        return 3;
    }

    std::cout << result.value << std::endl;
    return 0;
}
