#include "hexlearner.h"
#include <vector>

#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"

#include <iostream>
#include <cstdlib>
#include <ctime>


using namespace tensorflow;

/**
 * Construct this player.
 *
 * The implementation of tensorflow was mainly inspired by the blogpost https://medium.com/jim-fleming/loading-a-tensorflow-graph-with-the-c-api-4caaff88463f#.oe7w6x7at
 *
 * @param eng a hexgame engine
 * @param color the color with wich this player starts
 * @param greedy the epsilon in epsilon-greedy
 */
hexlearner::hexlearner(hexengine* eng, int color, float greedy):eng(eng),myColor(color),rewardDecay(0.9),greedy(greedy),actionsTaken(),statesSeen()
{
    // init random gen
    srand (time(NULL));

    // Initialize a tensorflow session
    this->status = NewSession(SessionOptions(), &(this->session));
    if (!this->status.ok()) {
        std::cout << this->status.ToString() << "\n";
	// error! should stop here
    }

    // Read in the protobuf graph we exported
    // (The path seems to be relative to the cwd. Keep this in mind
    // when using `bazel run` since the cwd isn't where you call
    // `bazel run` but from inside a temp folder.)
    GraphDef graph_def;
    this->status = ReadBinaryProto(Env::Default(), "models/graph.pb", &graph_def);
    if (!this->status.ok()) {
      std::cout << this->status.ToString() << "\n";
      // error! should stop here
    }

    // Add the graph to the session
    this->status = session->Create(graph_def);
    if (!this->status.ok()) {
      std::cout << this->status.ToString() << "\n";
      // error! should stop here
    }

    // Run the session and call initialisation of all variables
    this->status = this->session->Run({}, {}, {"init_all_vars_op"}, 0);
    if (!this->status.ok()) {
        std::cout << this->status.ToString() << "\n";
        std::cout << "EXIT." <<std::endl;
        // error! should stop here
    }
}

/**
 * Compute the value of the state action pair of the current state
 * and a given action using the neural net.
 *
 * @param action the action to be evaluated.
 * @return computed value
 */
float hexlearner::evaluateAction(hexmove* action){
    // createInputMatrix needs vectors of states and vectors of actions.
    // we create vectors containing exactly one element...
    std::vector<hexmove> vecAction = std::vector<hexmove>();
    vecAction.push_back(*action);

    std::vector< std::vector<short> > vecState = std::vector< std::vector<short> >();
    vecState.push_back(this->eng->getBoard());

    // Setup inputs and outputs:
    Tensor inp(DT_FLOAT, TensorShape({ 1, 103 }));
    auto inp_matrix = inp.tensor<float,2>();
    this->createInputMatrix(&vecState, &vecAction, inp_matrix);
  
    std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
        { "inp", inp },
    };

    // The session will initialize the outputs
    std::vector<tensorflow::Tensor> outputs;

    this->status = this->session->Run(inputs, {"outp"}, {}, &outputs);
    if (!this->status.ok()) {
      std::cout << this->status.ToString() << "\n";
      std::cout << "EXIT." <<std::endl;
      return 1;
    }

    TTypes<float>::Matrix output_c = outputs[0].matrix<float>();

    return output_c(0,0);
}


/**
 * Iterate over a vector of actions and return the one with the best value.
 *
 * @param actions Pointer to a vector of actions.
 * @return pointer to the best action.
 */
hexmove* hexlearner::findBestAction(std::vector<hexmove>* actions){
    float bestValueSoFar = -100.0;
    hexmove* bestActionSoFar = 0;


    for(std::vector<hexmove>::iterator it = actions->begin(); it != actions->end(); it++){
        float val = evaluateAction(&*it);
        if(val > bestValueSoFar){
            bestValueSoFar = val;
            bestActionSoFar = &*it;
        }

    }
    return bestActionSoFar;
}


/**
 * ask for the next action
 *
 * @param pieAllowed whether it is possible to apply the pie rule in this move
 * @return the selected action
 */
hexmove hexlearner::takeAction(bool pieAllowed){
    std::vector<short> board = eng->getBoard();
    this->statesSeen.push_back(board);

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
    if(static_cast<float>(std::rand())/RAND_MAX < this->epsilon){ // take some random action
	return possibleActions[std::rand()%possibleActions.size()];
    }else{ // take the optimal action
        hexmove* bestAction = this->findBestAction(&possibleActions);
        this->actionsTaken.push_back(hexmove(bestAction));
        return *bestAction;
    }
}

/**
 * inform the player of a win
 */
void hexlearner::won()
{
    train(1);
}

/**
 * inform the player of a loss
 */
void hexlearner::lost()
{
    train(-1);
}


/**
 * Create the matrix for tensorflow based on the given states and actions.
 *
 * @param states A pointer to a vector of states.
 * @param actions A pointer to a vector of actions. The action at index i corresponds to the state at index i.
 * @param matrix The matrix to be filled.
 */
void hexlearner::createInputMatrix(std::vector< std::vector<short> >* states, std::vector<hexmove>* actions, TTypes<float>::Matrix& matrix)
{
    for(int i=0; i < actions->size(); i++){
        // initialize with zeros
        for(int j=0; j<103; j++) matrix(i,j) = 0.0;
      
        // the first dimensions represent the state
	for(int j = 0; j < (*states)[i].size(); j++){
            matrix(i,3*j + (*states)[i][j]) = 1.0;
        }

	// the next dimensions (starting from 5*5*3=75) are the action
        hexmove curAction = (*actions)[i];
	if(curAction.getPie()){
          matrix(i,75+5+5) = 1.0;
        }else{
          matrix(i,75+curAction.Getx()) = 1.0;
          matrix(i,75+5+curAction.Gety()) = 1.0;
        }

	// finally we include the current player
	matrix(i,75+5+5+this->myColor) = 1.0;
    }
}

/**
 * Use the stored sequence to train the model.
 * 
 */
int hexlearner::train(int reward){
    // Setup inputs and outputs:
    Tensor inp(DT_FLOAT, TensorShape({ this->actionsTaken.size(), 103 }));
    auto inp_matrix = inp.tensor<float,2>();
    this->createInputMatrix(&(this->statesSeen), &(this->actionsTaken), inp_matrix);

    int value = reward;
    Tensor rew(DT_FLOAT, TensorShape({ this->actionsTaken.size(), 1 }));
    auto rew_matrix = rew.tensor<float,2>();
    for(int i = this->actionsTaken.size()-1; i>=0; i--){
        rew_matrix(i,0) = reward;
        reward = reward * this->rewardDecay;
    }

    std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
        { "inp", inp },
        { "rew", rew },
    };

    // The session will initialize the outputs
    std::vector<tensorflow::Tensor> outputs;

    this->status = this->session->Run(inputs, {"loss"}, {"train"}, &outputs);
    if (!this->status.ok()) {
        std::cout << this->status.ToString() << "\n";
        return 1;
    }

    TTypes<float>::Scalar output = outputs[0].scalar<float>();
    // output contains the current loss; we should log it somehow...
    //std::cout << output << std::endl;

    // Delete the history of this game
    this->actionsTaken.clear();
    this->statesSeen.clear();

    return 0;
}


/**
 * Inform the player that the opponent has applied the pie rule.
 */
void hexlearner::pie(){
    this->myColor = (this->myColor%2)+1;
}

/**
 * Change the greedy-probability.
 *
 * @param greedy new greedy probability. Assumed to be in [0.0,1.0].
 */
void resetGreedy(float greedy){
    this->greedy = greedy;
}
