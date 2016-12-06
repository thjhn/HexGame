#include "hexengine.h"
#include "hexgui.h"
#include "hexrandomplayer.h"
#include <iostream>

using namespace std;


int main()
{
    hexengine eng = hexengine(5);
    hexgui player_red = hexgui("Red", &eng);
    hexgui player_blue = hexgui("Blue", &eng);
   // hexrandomplayer player_blue = hexrandomplayer(&eng);
    eng.startGame(&player_red, &player_blue);
    eng.startGame(&player_red, &player_blue);

    return 0;
}
