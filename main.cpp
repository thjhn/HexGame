#include "hexengine.h"
#include "hexgui.h"
#include "hexrandomplayer.h"
#include "hexqlearn.h"
#include <iostream>
using namespace std;


int main()
{
    hexengine eng = hexengine(3);
    hexgui player_red = hexgui("Red", &eng);
    hexrandomplayer tester = hexrandomplayer(&eng);
    hexqlearn trainer = hexqlearn(&eng,2);
    hexqlearn player_blue = hexqlearn(&eng,1);

    for(int i = 1; i<50000; i++){
        if(i%1000==0){
            cout << "At training game " << i << endl;
        }
        eng.startGame(&player_blue,&trainer);
    }

    player_blue.resetNotGreedyProp(0.0); // use best policy while evaluating.

    int wins = 0;
    for(int i = 1; i<1000; i++){
        if(i%100==0){
            cout << "At test game " << i << endl;
        }
        if(eng.startGame(&player_blue,&tester)==1){
            wins++;
        }
    }
    cout << wins << " wins" << endl;

    for(int i = 0; i < 10; i++) eng.startGame(&player_blue,&player_red);

    return 0;
}
