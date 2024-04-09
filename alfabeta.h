#ifndef ALFABETA_H
#define ALFABETA_H

#include <iostream>
#include <algorithm>
#include "tree.h"

using namespace std;

extern const int MAX;
extern const int MIN;
extern int nodeCount;

int alfabeta(Node* node, bool isMaxPlayer, int depth, int alpha, int beta) {
    nodeCount++;  // visited node count

    // if leaf node or set depth has been reached, return heuristic function value
    if (node->getState().hasFinished() || depth == 0) {
        int value = node->getState().heuristicValue();
        // set nodes heuristic value, so that best child node can be found
        node->setValue(value);

        return value;
    }

    // if maximizing players turn
    if (isMaxPlayer) {
        int bestValue = MIN;
        vector<Node*> children = node->getChildNodes();

        // get alfabeta values for each child and find max value, prune branch if needed
        for (Node* child : children) {
            // act as minimizing player (isMaxPlayer = false)
            // reduce depth by 1
            int value = alfabeta(child, false, depth - 1, alpha, beta);
            bestValue = max(bestValue, value);

            // set new alpha if higher
            alpha = max(alpha, value);
            // check if pruning is needed
            if (beta <= alpha)
                // beta pruning
                break;
        }

        // update nodes value
        node->setValue(bestValue);
        return bestValue;
    }
    // if minimizing players turn
    else {
        int bestValue = MAX;
        vector<Node*> children = node->getChildNodes();

        // get alfabeta values for each child and find min value, prune branch if needed
        for (Node* child : children) {
            // act as minimizing player (isMaxPlayer = false)
            // reduce depth by 1
            int value = alfabeta(child, true, depth - 1, alpha, beta);
            bestValue = min(bestValue, value);

            // set new beta if lower
            beta = min(beta, value);
            // check if pruning is needed
            if (beta <= alpha)
                // alpha pruning
                break;
        }

        // update nodes value
        node->setValue(bestValue);
        return bestValue;
    }
}

#endif // ALFABETA_H
