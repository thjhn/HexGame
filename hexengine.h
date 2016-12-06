#ifndef HEXENGINE_H
#define HEXENGINE_H
#include "hexplayer.h"
#include "hexfield.h"
#include <vector>

/**
 * The class hexengine manages a game of hex.
 * It contains a representation of the current board.
 * When startGame() is called it asks the opponents for their
 * next actions.
 * Here an opponent is an object of the class hexplayer and asking
 * for the next move means invoking the player's takeAction() function.
 */
class hexengine
{
    public:
        hexengine(int);
        virtual ~hexengine();
        std::vector<short> getBoard();
        int getBoardSize();
        int play(int, int);
        int play(hexmove);
        void startGame(hexplayer*, hexplayer*);
        inline int i2x(int);
        inline int i2y(int);
        inline int xy2i(int, int);
    protected:
    private:
        const int boardsize;
        short whoseTurn;
        std::vector<hexfield> board;
        hexfield border_left;
        hexfield border_right;
        hexfield border_top;
        hexfield border_bottom;
        void restartGame();
        void linkFields(hexfield* field1, hexfield* field2);
        bool fieldsAreConnected(hexfield*, hexfield*, int);
        int gameState();
};

#endif // HEXENGINE_H
