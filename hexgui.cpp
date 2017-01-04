#include "hexgui.h"
#include "hexengine.h"
#include "hexmove.h"
#include <string>
#include <iostream>
#include <vector>

/**
 * Construct this gui.
 *
 * @param playername Give this player a nice name.
 * @param eng a hexgame engine
 */
hexgui::hexgui(std::string playername, hexengine* eng):playername(playername),eng(eng)
{}

/**
 * ask for the next action
 *
 * @param pieAllowed whether it is possible to apply the pie rule in this move
 * @return the selected action
 */
hexmove hexgui::takeAction(bool pieAllowed){
    int x,y;
    std::cout << playername << ", it's your move." << std::endl;
    this->drawBoard();
    if(pieAllowed){
        std::cout << "Would you like to take advantage of the pie rule? [y/n]: ";
        char pieAnswer;
        std::cin >> pieAnswer;
        if(pieAnswer == 'y' || pieAnswer == 'Y'){
            return hexmove();
        }
    }
    std::cout << "X coordinate: ";
    std::cin >> x;
    std::cout << "Y coordinate: ";
    std::cin >> y;
    std::cout << std::endl;

    return hexmove(x,y);
}

/**
 * Draw the current board to the screen.
 */
void hexgui::drawBoard()
{
    const int boardsize = eng->getBoardSize();
    std::vector<short> b = eng->getBoard();
    int counter = 0;
    int line = 0;
    for( std::vector<short>::iterator it = b.begin(); it != b.end(); it++){
        if(counter++ % boardsize == 0){
          std::cout << std::endl;
          for(int i = 3 + line++; i> 0; i--) std::cout << " ";
        }
        if(*it == 0) std::cout << ".";
        if(*it == 1) std::cout << "x";
        if(*it == 2) std::cout << "o";
        std::cout << " ";
    }
    std::cout << std::endl;
}

/**
 * inform the player of a win
 */
void hexgui::won()
{
    std::cout << playername << ", you won." << std::endl;
}

/**
 * inform the player of a loss
 */
void hexgui::lost()
{
    std::cout << playername << ", you lost." << std::endl;
}

/**
 * inform the player that the opponent has applied the pie rule
 */
void hexgui::pie(){
    std::cout << playername << ", your opponent takes the pie rule." << std::endl;
}


/**
 * reset the color; i.e. do nothing
 */
void hexgui::resetColor(short color){
}
