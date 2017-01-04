#ifndef HEXQLEARN_H
#define HEXQLEARN_H

#include "hexplayer.h"
#include "hexengine.h"
#include "hexmove.h"

#include <map>

class hexqlearn : public hexplayer
{
    public:
        hexqlearn(hexengine*,short);
        hexmove takeAction(bool);
        void won();
        void lost();
        void pie();
        void resetNotGreedyProp(float);
        void resetColor(short);
    protected:
    private:
        hexengine* eng;
        short myColor;
        float notGreedyProp = 0.2;
        std::vector<hexmove> actionsTaken;
        std::vector< std::vector<short> > statesSeen;
        std::vector<short> colors;

        unsigned int hashState(hexmove*);
        unsigned int hashState(hexmove*, std::vector<short>, short);
        std::map<unsigned int,float> qtable;
        void train(short);
};

#endif // HEXQLEARN_H
