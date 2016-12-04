#ifndef HEXGUI_H
#define HEXGUI_H
#include "hexplayer.h"
#include "hexmove.h"
#include "hexengine.h"
#include <string>

/**
 * Implement a hex player which allows the user to play a game of hex on the console.
 */
class hexgui: virtual public hexplayer
{
    public:
        hexgui(std::string, hexengine*);
        hexmove takeAction(bool);
        void won();
        void lost();
        void pie();
    protected:
    private:
        const std::string playername;
        hexengine* eng;

        void drawBoard();
};

#endif // HEXGUI_H
