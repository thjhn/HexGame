#ifndef HEXRANDOMPLAYER_H
#define HEXRANDOMPLAYER_H

#include "hexplayer.h"
#include "hexengine.h"

/**
 * Implement a hex player that plays randomly.
 */
class hexrandomplayer : public hexplayer
{
    public:
        hexrandomplayer(hexengine*);
        hexmove takeAction(bool);
        void won();
        void lost();
        void pie();
        void resetColor(short);
    protected:
    private:
        hexengine* eng;
};

#endif // HEXRANDOMPLAYER_H
