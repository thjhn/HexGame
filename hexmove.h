#ifndef HEXMOVE_H
#define HEXMOVE_H

/**
 * Describes a move that can be taken in a hex game.
 * There are two types of actions: put a block on field (x,y) or
 * apply the pie rule.
 * To each of these types there exists one constructor.
 */
class hexmove
{
    public:
        hexmove(int, int);
        hexmove();
        int Getx() { return x; }
        int Gety() { return y; }
        int getPie() { return pie; }
    protected:
    private:
        int x;
        int y;
        bool pie;
};

#endif // HEXMOVE_H
