#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <queue>
#include <map>
#include <algorithm>
#include "state.h"

#include <ctime>

using namespace std;

class Node {
private:
    vector<Node*> parentNodes;
    vector<Node*> childNodes;
    State state;
    int depth;

public:
    Node(State state) {
        this->state = state;
        this->depth = 0;
    }

    Node(Node* parentNode, State state, int depth) {
        this->parentNodes.push_back(parentNode);
        this->state = state;
        this->depth = depth;
    }


    ~Node() {
        for (Node* node : childNodes) {
            delete node;
        }
    }

    // adds child to node
    Node* addChild(State state) {
        Node* childNode = new Node(this, state, depth + 1);
        childNodes.push_back(childNode);
        return childNode;
    }

    // add parent to node
    void addParent(Node* parent) {
        parentNodes.push_back(parent);
    }

    State getState() const { return state; }
    vector<Node*> getParentNode() const { return parentNodes; }
    vector<Node*> getChildNodes() const { return childNodes; }
    int getDepth() const { return depth; }
};

class Tree {
private:
    Node* rootNode;

public:
    Tree(State state) {
        rootNode = new Node(state);
    }

    ~Tree() {
        delete rootNode;
    }

    // generate tree, takes in tree depth as argument, if not given, generates full tree
    void generateTree(int depth = -1) {
        queue<Node*> curLevel;         // nodes at current depth
        map<State, Node*> nextLevel;   // unique nodes at next depth
        vector<State> states;          // nodes possible child states
        Node* curNode;                 // currently looked at node
        int curDepth = 0;              // current depth

        // statistic variables
        int totalNodes = 1;
        int nodeCount = 0;
        int stateCount = 0;

        curLevel.push(rootNode);

        clock_t start = clock();

        // iterate while there are nodes or until depth is reached
        while (!curLevel.empty() && (depth == -1 || curDepth < depth)) {
            curNode = curLevel.front();
            curLevel.pop();

            // generate current nodes possible child states
            states = generateChildStates(curNode->getState());
            for (State state : states) {
                auto result = nextLevel.find(state);
                // if state not found in next level, create new node and add it to next level
                if (result == nextLevel.end()) {
                    nextLevel.emplace(state, curNode->addChild(state));
                    totalNodes++;
                }
                // if state found in next level, add parent to its node
                else {
                    result->second->addParent(curNode);
                }
                stateCount++;
            }

            // if current level completed, go to next level
            if (curLevel.empty()) {
                for (const auto& pair : nextLevel) {
                    curLevel.push(pair.second);
                    nodeCount++;
                }
                nextLevel.clear();

                curDepth++;

                /*
                qDebug() << "Depth " << curDepth << " generated";
                qDebug() << "\t" << totalNodes << " Total nodes";
                qDebug() << "\t" << nodeCount << " Nodes generated";
                qDebug() << "\t" << stateCount << " States created ";
                */

                nodeCount = 0;
                stateCount = 0;
            }
        }
        clock_t end = clock();
        double duration = double(end - start) / CLOCKS_PER_SEC;

        //qDebug() << "Tree generated " << duration;
    }

    // generates and returns all child states
    vector<State> generateChildStates(State parentState) {
        vector<State> states;  // generated states
        State state;           // current state

        // for all unique numbers do all actions
        vector<int> numbers = parentState.getUniqueNumbers();
        for (int number : numbers) {
            // action where number is removed
            state = parentState;
            state.doAction(number, false);

            // add only unique states
            if (find(states.begin(), states.end(), state) == states.end()) {
                states.push_back(state);
            }

            // action where number is divided
            if (number == 2 || number == 4) {
                state = parentState;
                state.doAction(number, true);

                // add only unique states
                if (find(states.begin(), states.end(), state) == states.end()) {
                    states.push_back(state);
                }
            }
        }

        return states;
    }

    Node* getRoot() const { return rootNode; }

    // retrieves all nodes
    vector<Node*> getAllNodes() {
        vector<Node*> nodes;	  // vector of all nodes
        queue<Node*> curLevel;    // nodes in current depth
        vector<Node*> nextLevel;  // nodes in next depth

        Node* curNode;

        curLevel.push(rootNode);
        nodes.push_back(rootNode);

        // iterate while there are nodes
        while (!curLevel.empty()) {
            curNode = curLevel.front();
            curLevel.pop();

            // get all child nodes of current node
            for (Node* node : curNode->getChildNodes()) {
                // add only new child nodes to the next level
                if (find(nextLevel.begin(), nextLevel.end(), node) == nextLevel.end()) {
                    nextLevel.push_back(node);
                }
            }

            // if current level is empty, set next level as current
            if (curLevel.empty()) {
                for (Node* node : nextLevel) {
                    curLevel.push(node);
                    nodes.push_back(node);
                }
                nextLevel.clear();
            }
        }

        return nodes;
    }
};

#endif // TREE_H
