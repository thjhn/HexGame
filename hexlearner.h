#ifndef HEXLEARNER_H
#define HEXLEARNER_H

#include "hexmove.h"
#include "hexplayer.h"
#include "hexengine.h"

#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"

#include <vector>


/**
 * Implement a hex player that uses RL to learn the game.
 */
class hexlearner : public hexplayer
{
    public:
        hexlearner(hexengine*,int,float);
        hexmove takeAction(bool);
        void won();
        void lost();
        void pie();
	void resetGreedy(float);
    protected:
    private:
        hexengine* eng;
	short myColor;
        float evaluateAction(hexmove* action);
        hexmove* findBestAction(std::vector<hexmove>*);
        const float rewardDecay;
        float greedy;
        std::vector<hexmove> actionsTaken;
        std::vector< std::vector<short> > statesSeen;
	tensorflow::Session* session;
	tensorflow::Status status;
        int train(int);
        void createInputMatrix(std::vector< std::vector<short> >*, std::vector<hexmove>*, tensorflow::TTypes<float>::Matrix&);
};

#endif // HEXLEARNER_H
