#ifndef HEXPLAYER_H
#define HEXPLAYER_H
#include "hexmove.h"

/**
 * A hexplayer is an abstract class of an entity that is able to communicate
 * with a hexengine.
 */
class hexplayer
{
    public:
        hexplayer();
        /**
         * ask for the next action.
         * @param bool whether it is possible to apply the pie rule.
         */
        virtual hexmove takeAction(bool) = 0;
        /**
         * inform the player of a win
         */
        virtual void won() = 0;
        /**
         * inform the player of a loss
         */
        virtual void lost() = 0;
        /**
         * inform the player that the opponent has applied the pie rule
         */
        virtual void pie() = 0;
    protected:
    private:
};

#endif // HEXPLAYER_H
