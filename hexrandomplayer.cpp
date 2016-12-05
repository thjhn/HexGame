#include "hexrandomplayer.h"
#include <cstdlib>
#include <ctime>

/**
 * Construct this player.
 *
 * @param eng a hexgame engine
 */
hexrandomplayer::hexrandomplayer(hexengine* eng):eng(eng)
{
    srand (time(NULL));
}

/**
 * ask for the next action
 *
 * @param pieAllowed whether it is possible to apply the pie rule in this move
 * @return the selected action
 */
hexmove hexrandomplayer::takeAction(bool pieAllowed){
    if(pieAllowed){
        int num = (rand() % 100);
        if(num < 66){ // apply pie
            return hexmove();
        }
    }

    int x = (rand() % this->eng->getBoardSize());
    int y = (rand() % this->eng->getBoardSize());

    return hexmove(x,y);
}

/**
 * inform the player of a win
 * A random play does not care about this information. Thus, the function
 * does nothing.
 */
void hexrandomplayer::won()
{}

/**
 * inform the player of a loss
 * A random play does not care about this information. Thus, the function
 * does nothing.
 */
void hexrandomplayer::lost()
{}

/**
 * Inform the player that the opponent has applied the pie rule.
 * A random play does not care about this information. Thus, the function
 * does nothing.
 */
void hexrandomplayer::pie(){}
