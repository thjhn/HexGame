#ifndef HEXFIELD_H
#define HEXFIELD_H
#include <vector>

/**
 * hexfield is a field in a board of a hex game.
 * It contains a color which is 0 (empty), 1 (red) or 2 (blue) and has a vector of
 * neighbour fields.
 */
class hexfield
{
    public:
        hexfield();
        int setColor(int);
        int getColor();
        void addNeighbour(hexfield*);
        std::vector<hexfield*> getNeighbours();
    protected:
    private:
        short color;
        std::vector<hexfield*> neighbours;

};

#endif // HEXFIELD_H
