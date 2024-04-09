#ifndef MINIMAX_H
#define MINIMAX_H

#include <iostream>
#include <algorithm>
#include "tree.h"

using namespace std;

extern const int MAX;
extern const int MIN;
extern int nodeCount;

int minimax(Node* node, bool isMaxPlayer, int depth) {
    nodeCount++;  // visited node count

    // if leaf node or depth 0 has been reached, return heuristic function value
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

        // get minimax values for each child and find highest value
        for (Node* child : children) {
            // act as minimizing player (isMaxPlayer = false)
            // reduce depth by 1
            int value = minimax(child, false, depth - 1);
            bestValue = max(bestValue, value);
        }

        // update nodes value
        node->setValue(bestValue);
        return bestValue;
    }
    // if minimizing players turn
    else {
        int bestValue = MAX;
        vector<Node*> children = node->getChildNodes();

        // get minimax values for each child and find lowest value
        for (Node* child : children) {
            // act as maximizing player player (isMaxPlayer = true)
            // reduce depth by 1
            int value = minimax(child, true, depth - 1);
            bestValue = min(bestValue, value);
        }

        // update nodes value
        node->setValue(bestValue);
        return bestValue;
    }
}

#endif // MINIMAX_H
