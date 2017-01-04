#include "hexengine.h"
#include "hexfield.h"
#include <queue>
#include <set>

#include <iostream>

/**
 * The constructor sets up a game of hex with size*size fields.
 */
hexengine::hexengine(int size):boardsize(size),whoseTurn(1)
{
    // even borders are hexfields...
    border_bottom = hexfield(); border_bottom.setColor(2);
    border_top = hexfield(); border_top.setColor(2);
    border_left = hexfield(); border_left.setColor(1);
    border_right = hexfield(); border_right.setColor(1);

    for(int i = 0; i<size*size; i++)  // create fields
        this->board.push_back(hexfield());


    for(int i = 0; i<size*size; i++){  // link fields
        int x = this->i2x(i);
        int y = this->i2y(i);

        if(y > 0){ // connect to previous row
            int up = xy2i(x,y-1);
            linkFields(&(this->board[i]), &(this->board[up]));

            if(x<size-1){ // not the last element in a row
                int up_right = xy2i(x+1,y-1);
                linkFields(&(this->board[i]), &(this->board[up_right]));
            }
        }else{ // we are in the first row...
            linkFields(&(this->board[i]), &border_top);
        }

        if(y==size-1){ // we are in the last row
            linkFields(&(this->board[i]), &border_bottom);
        }

        if(x==0){ //we are in the first column
            linkFields(&(this->board[i]), &border_left);
        }else{ // not the first element in a row
            int left = xy2i(x-1,y);
            linkFields(&(this->board[i]), &(this->board[left]));
        }
        if(x==size-1){ // we are in the last column
            linkFields(&(this->board[i]), &border_right);
        }
    }
}

void hexengine::restartGame(){
    for(std::vector<hexfield>::iterator it = this->board.begin(); it != this->board.end(); it++){
        it->resetColor();
    }
    this->whoseTurn=1;
}

/**
 * The Desctrotur should remove the board set up by the constructor.
 * @TODO: implement this
*/
hexengine::~hexengine()
{
    //dtor
}

/**
 * Get a short vector representation of the current board.
 * The board is linearized by the mapping (x,y)|-> y*size+x where
 * size is the parameter given to the constructor.
 * The integer 0 represents an empty field while 1 means that there
 * is a red block and 2 means a blue block.
 */
std::vector<short> hexengine::getBoard()
{
    std::vector<short> board;
    for(std::vector<hexfield>::iterator it = this->board.begin(); it != this->board.end(); ++it){
      board.push_back(it->getColor());
    }
    return board;
}

/**
 * Retrieve the width (==height) of the board.
 */
int hexengine::getBoardSize()
{
    return this->boardsize;
}

/**
 * Put a block of the current player (specified in the local variable
 * whoseTurn) on the field (x,y). If the move was legal it changes the current player
 * afterwards.
 *
 * @param x the x coordinate
 * @param y the y coordinate
 * @return -1 when the move is illegal, 0 otherwise
*/
int hexengine::play(int x, int y)
{
    if( 0 > x || 0 > y || x > this->boardsize || y > this->boardsize) return -1; // illegal coordinates
    int player = this->whoseTurn;
    int play = (this->board[x+this->boardsize*y]).setColor( player );

    if(play < 0) return -1; // illegal move

    this->whoseTurn = player%2+1; // switch player

    return 0;
}

/**
 * Put a block of the current player (specified in the local variable
 * whoseTurn) on the field specified by the hexmove object act.
 * Warning: This function does not handle the pie rule!
 *
 * @param act a move to be exectued
 * @return -1 when the move is illegal, 0 otherwise
*/
int hexengine::play(hexmove act)
{
    return this->play(act.Getx(),act.Gety());
}


/**
 * Internally the board is represented as a linear vector using the linearization
 * map (x,y) |-> x+size*y where size is the board with as specified by the constructor.
 * This function computes the x coordinate given an index in the linearization.
 *
 * @param i index in the linearization
 * @return x-coordinate
 */
inline int hexengine::i2x(int i){
    return i%this->boardsize;
}

/**
 * Internally the board is represented as a linear vector using the linearization
 * map (x,y) |-> x+size*y where size is the board with as specified by the constructor.
 * This function computes the y coordinate given an index in the linearization.
 *
 * @param i index in the linearization
 * @return y-coordinate
 */
inline int hexengine::i2y(int i){
    return i/this->boardsize;
}

/**
 * Internally the board is represented as a linear vector using the linearization
 * map (x,y) |-> x+size*y where size is the board with as specified by the constructor.
 * This function computes the image of the pair (x,y)
 *
 * @param x x-coordinate
 * @param y y-coordinate
 * @return index in the linearization
 */
inline int hexengine::xy2i(int x, int y){
    return y*this->boardsize+x;
}

/**
 * Given two fields in the hex board this function introduces them as neighbours.
 *
 * @param field1 a field to be introduced to field2
 * @param field2 a field to be introduced to field1
 */
void hexengine::linkFields(hexfield* field1, hexfield* field2){
    field1->addNeighbour(field2);
    field2->addNeighbour(field1);
}

/**
 * Check whether the fields src and dest are connected by a path of the given color.
 * Usually this is used to check whether the border elements are connected.
 *
 * @param src pointer to the source field
 * @param dest pointer to the destination field
 * @return whether the two fields are connected.
 */
bool hexengine::fieldsAreConnected(hexfield* src, hexfield* dest, int color)
{
    std::set<hexfield*> visited = std::set<hexfield*>();
    std::queue<hexfield*> enqueued = std::queue<hexfield*>();
    enqueued.push(src);
    while(!enqueued.empty()){
        hexfield* cur = enqueued.front();
        if(cur == dest) return true;
        if(cur->getColor() == color){ // only add field if it has the right color
            std::vector<hexfield*> neighbours = cur->getNeighbours();
            for(std::vector<hexfield*>::iterator it = neighbours.begin(); it != neighbours.end(); it++){
                if(visited.find(*it) == visited.end())
                    enqueued.push(*it);
            }
        }
        visited.insert(cur);
        enqueued.pop();
    }
    return false;
}

/**
 * Get the current game state. This is 0 if the game is currently running, 1 if red won and 2 if blue won.
 *
 * @return the current game state
 */
short hexengine::gameState()
{
    if(this->fieldsAreConnected(&(this->border_left), &(this->border_right),1)) // red wins
        return 1;
    if(this->fieldsAreConnected(&(this->border_top), &(this->border_bottom),2)) // blue wins
        return 2;
    return 0; // nobody wins
}

/**
 * Start a game of hex between two hexplayers.
 *
 * @param red the player of the red pieces.
 * @param blue the player of the blue pieces.
 *
 * @return the winner of the game
 */
short hexengine::startGame(hexplayer* red, hexplayer* blue){
    red->resetColor(1);
    blue->resetColor(2);

    bool pieApplied=false;
    this->restartGame();
    int gameState;
    bool firstMove = true; // for the pie rule
    while((gameState = this->gameState()) == 0){
        if(this->whoseTurn == 1){
            // ask the red player to move
            this->play( red->takeAction(false) ); // ask for a action
        }else{
            // ask the blue player to move
            if(firstMove){
                hexmove blue_move = blue->takeAction(true);
                if(blue_move.getPie()){ // pie rule applied, swap players
                    pieApplied = true;
                    red->pie();
                    // now, swap players
                    hexplayer* tmp = red;
                    red = blue;
                    blue = tmp;
                }else{

                    this->play(blue_move);

                }
                firstMove = false;
            }else this->play( blue->takeAction(firstMove) ); // ask for a action
        }
    }

    if(gameState == 1){ // red wins
        red->won();
        blue->lost();
    }else{
        blue->won();
        red->lost();
    }
    if(pieApplied) return gameState%2+1;
    return gameState;
}
