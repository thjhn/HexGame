#include "hexmove.h"

/**
 * Construct an action that puts a piece on field (x,y)
 *
 * @param x x-coordinate of the field
 * @param y y-coordinate of the field
 */
hexmove::hexmove(int x, int y):x(x),y(y),pie(false){}

/**
 * Construct an action that applies the pie rule.
 */
hexmove::hexmove():x(-1),y(-1),pie(true){}

/**
 * Copy constructor
 */
hexmove::hexmove(hexmove* other):x(other->Getx()),y(other->Getx()),pie(other->getPie()){}

/**
 * Send hexmove to outstrem.
 */
std::ostream& operator<<(std::ostream& out, hexmove& move){
    if(move.getPie()) return out << "PIE";
    return out << "S(" << move.Getx() << ", " << move.Gety() << ")";
}
