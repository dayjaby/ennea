
#include <string>
#include <iostream>
#include <boost/ref.hpp>
#include "gui.hpp"

int main()
{
    using namespace Ennovia;
    GUI game(1024,768);
    game.run();
    return 0;
}
