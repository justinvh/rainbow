#include <rainbow/display/gui.hpp>

using namespace rb;

Gui::Gui()
{
}

void Gui::draw(int x, int y, const std::string& message)
{
    std::cout << "Drawing " << x << " " << y << ": " << message << "\n";
}

