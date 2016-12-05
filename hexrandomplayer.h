#ifndef HEXRANDOMPLAYER_H
#define HEXRANDOMPLAYER_H

#include "hexplayer.h"
#include "hexengine.h"


class hexrandomplayer : public hexplayer
{
    public:
        hexrandomplayer(hexengine*);
        hexmove takeAction(bool);
        void won();
        void lost();
        void pie();
    protected:
    private:
        hexengine* eng;
};

#endif // HEXRANDOMPLAYER_H
