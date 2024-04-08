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
    nodeCount++;

    // if leaf node or set depth has been reached, return heuristic function value
    if (node->getState().hasFinished() || depth == 0) {
        int value = node->getState().heuristicValue();
        node->setValue(value);

        return value;
    }

    // if maximizing players turn
    if (isMaxPlayer) {
        int bestValue = MIN;
        vector<Node*> children = node->getChildNodes();

        // get minimax values for each child and find max value
        for (Node* child : children) {
            int value = minimax(child, false, depth - 1);
            bestValue = max(bestValue, value);
        }

        node->setValue(bestValue);
        return bestValue;
    }
    // if minimizing players turn
    else {
        int bestValue = MAX;
        vector<Node*> children = node->getChildNodes();

        // get minimax values for each child and find min value
        for (Node* child : children) {
            int value = minimax(child, true, depth - 1);
            bestValue = min(bestValue, value);
        }

        node->setValue(bestValue);
        return bestValue;
    }
}

#endif // MINIMAX_H
