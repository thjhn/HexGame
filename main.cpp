#include "hexengine.h"
#include "hexgui.h"
#include <iostream>

using namespace std;


int main()
{
    hexengine eng = hexengine(5);
    hexgui player_red = hexgui("Red", &eng);
    hexgui player_blue = hexgui("Blue", &eng);
    eng.startGame(&player_red, &player_blue);

    return 0;
}
