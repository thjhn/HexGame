#include "hexengine.h"
#include "hexgui.h"
#include "hexlearner.h"
#include "hexrandomplayer.h"
#include <iostream>

using namespace std;


int main()
{
    hexengine eng = hexengine(5);
    hexgui player_red = hexgui("Red", &eng);
    //hexgui player_blue = hexgui("Blue", &eng);
    hexlearner player_trainer = hexlearner(&eng,1);
//    hexlearner player_blue = hexlearner(&eng,2);

hexrandomplayer player_blue = hexrandomplayer(&eng);

    for(int i = 0; i < 1000; i++){
      cout << i <<":  ";
      eng.startGame(&player_trainer, &player_blue);
    }
    eng.startGame(&player_red, &player_blue);

    return 0;
}
