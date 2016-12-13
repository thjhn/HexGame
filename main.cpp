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
    hexlearner player_trainer = hexlearner(&eng,1,0.9,"models/graph.pb");
    hexrandomplayer player_tester = hexrandomplayer(&eng);
    hexlearner player_blue = hexlearner(&eng,2,0.9,"models/graph.pb");


    cout << "Started training..." << endl;
    for(int i = 0; i < 50; i++){
        if(i%10 ==0){
            cout << "at training game "<< i << endl;
        }
        eng.startGame(&player_trainer, &player_blue);
    }

    player_blue.resetGreedy(1.0); // for evaluation we don't want to take the best action.

    cout << "Started evaluation..." << endl;
    int wins = 0;
    const int testgames = 50;
    for(int i = 0; i < testgames; i++){
        if(eng.startGame(&player_tester, &player_blue) == 2){
          wins++;
        }
    }

    cout << "Won " << wins << " (" << static_cast<float>(wins)*100.0/testgames << "%) games." << endl;    

    eng.startGame(&player_red, &player_blue);

    return 0;
}
