#include "hexengine.h"
#include "hexgui.h"
#include "hexlearner.h"
#include <iostream>

using namespace std;


int main()
{
    hexengine eng = hexengine(5);
    hexgui player_red = hexgui("Red", &eng);
    //hexgui player_blue = hexgui("Blue", &eng);
    hexlearner player_trainer = hexlearner(&eng,1);
    hexlearner player_blue = hexlearner(&eng,2);
    for(int i = 0; i < 1; i++){
      cout<< "Start training.";
      eng.startGame(&player_trainer, &player_blue);
    }
    eng.startGame(&player_red, &player_blue);
    eng.startGame(&player_red, &player_blue);
    eng.startGame(&player_red, &player_blue);

    return 0;
}
