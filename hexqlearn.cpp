#include "hexqlearn.h"
#include "hexengine.h"
#include "hexmove.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

/**
 * Construct this player.
 *
 * @param eng a hexgame engine
 * @param color the own color, 1 for red, 2 for blue
 */
hexqlearn::hexqlearn(hexengine* eng, short color):eng(eng),myColor(color)
{
    // init random gen
    srand (time(NULL));
}

/**
 * ask for the next action
 *
 * @param pieAllowed whether it is possible to apply the pie rule in this move
 * @return the selected action
 */
hexmove hexqlearn::takeAction(bool pieAllowed){
    std::vector<short> board = eng->getBoard();
    this->statesSeen.push_back(board);
    this->colors.push_back(this->myColor);

    // collect all actions that we currently could take
    std::vector<hexmove> possibleActions;
    for(int i = 0; i < (eng->getBoardSize()*eng->getBoardSize()); i++){
        if(board[i] == 0){ // empty field
            possibleActions.push_back(hexmove(eng->i2x(i),eng->i2y(i)));
        }
    }
    if(pieAllowed) possibleActions.push_back(hexmove());

    // using the collected actions we follow a epsilon-greedy policy based on the
    // the values from the current neural net
    hexmove bestAction;// = this->findBestAction(&possibleActions);
    if(static_cast<float>(std::rand())/RAND_MAX > 1.0-this->notGreedyProp){ // take some random action
	    bestAction = possibleActions[std::rand()%possibleActions.size()];
    }else{ // take the optimal action

        float bestValue = -100;
        for(std::vector<hexmove>::iterator it = possibleActions.begin(); it != possibleActions.end(); it++){
            float q = this->qtable[this->hashState(&(*it))];
            if(q>bestValue){
                bestValue = q;
                bestAction = *it;
            }
        }
    }
    this->actionsTaken.push_back(hexmove(&bestAction));
	if(bestAction.getPie()){
        this->pie(); // notify myself that I played pie action.
	}
    return bestAction;

}

/**
 * Train the model.
 *
 * @param reward final reward
 */
void hexqlearn::train(short finalReward){
    float reward = static_cast<float>(finalReward);
    bool pieAllowed;
    for(int i = this->actionsTaken.size()-1; i>=0; i--){
        pieAllowed = true;
        unsigned int hash = this->hashState(&(this->actionsTaken[i]),this->statesSeen[i],this->colors[i]);
        this->qtable[hash] = this->qtable[hash]*0.7 + 0.3*reward;
        //std::cout << "stored in " << i <<": " << this->qtable[hash] << " for hash " << hash << " w r " << reward << " act " << this->actionsTaken[i] << std::endl;
        //reward = this->qtable[hash]*1.0;

        std::vector<short> board = this->statesSeen[i];
        // collect all actions that we currently could take
        std::vector<hexmove> possibleActions;
        for(int j = 0; j < (eng->getBoardSize()*eng->getBoardSize()); j++){
            if(board[j] == 2){ pieAllowed = false;}// if there is one blue piece there can't be pie
            if(board[j] == 0){ // empty field
                possibleActions.push_back(hexmove(eng->i2x(j),eng->i2y(j)));
            }
        }
        if(pieAllowed && this->colors[i]==2) possibleActions.push_back(hexmove());
        hexmove* bestAction;// = this->findBestAction(&possibleActions);
        float bestValue = -100.0;
        for(std::vector<hexmove>::iterator it = possibleActions.begin(); it != possibleActions.end(); it++){
            float q = this->qtable[this->hashState(&(*it),this->statesSeen[i],this->colors[i])];
            //std::cout <<this->hashState(&(*it),this->statesSeen[i],this->colors[i]) << "=" << q << "; " << *it <<"\n";
            if(q>bestValue){
                bestValue = q;
                bestAction = &(*it);
            }
        }

        unsigned int hashBest = this->hashState(bestAction,this->statesSeen[i],this->colors[i]);
//std::cout << std::endl << "HB:" << hashBest << std::endl;
        reward = this->qtable[hashBest];
    }

    // Delete the history of this game
    this->actionsTaken.clear();
    this->statesSeen.clear();
    this->colors.clear();


}

/**
 * inform the player of a win
 */
void hexqlearn::won()
{
    this->train(1);
}

/**
 * inform the player of a loss
 */
void hexqlearn::lost()
{
    this->train(-1);
}

/**
 * Reset Color
 */
void hexqlearn::resetColor(short color){
  this->myColor = color;
}

/**
 * Inform the player that the opponent has applied the pie rule.
 */
void hexqlearn::pie(){
  this->myColor = this->myColor%2+1;
}

void hexqlearn::resetNotGreedyProp(float notGreedyProp){
    this->notGreedyProp=notGreedyProp;
}

unsigned int hexqlearn::hashState(hexmove* action){
    return this->hashState(action, this->eng->getBoard(), this->myColor);
}

unsigned int hexqlearn::hashState(hexmove* action, std::vector<short> board, short player){
    int factor=1;
    int hashVal = 0;
    for(std::vector<short>::iterator it = board.begin(); it != board.end(); it++){
        hashVal += (*it) * factor;
        factor *= 3;
    }

    hashVal += action->Getx() * factor;
    factor *= this->eng->getBoardSize();
    hashVal += action->Gety() * factor;
    factor *= this->eng->getBoardSize();
    hashVal += static_cast<int>(action->getPie()) * factor;
    factor *= 2;
    hashVal += player * factor;

    return hashVal;
}
