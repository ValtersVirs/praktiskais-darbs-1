#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <queue>
#include <map>
#include <algorithm>
#include "state.h"

#include <ctime>

using namespace std;

// tree node class
class Node {
private:
    vector<Node*> parentNodes;  // parent nodes
    vector<Node*> childNodes;   // child nodes
    State state;                // state
    int depth;                  // nodes depth
    int value;                  // states heuristic value

public:
    Node(State state) {
        this->state = state;
        this->depth = 0;
        this->value = 0;
    }

    Node(Node* parentNode, State state, int depth) {
        this->parentNodes.push_back(parentNode);
        this->state = state;
        this->depth = depth;
        this->value = 0;
    }

    // creates a new child node
    Node* addNewChild(State state) {
        Node* childNode = new Node(this, state, depth + 1);
        childNodes.push_back(childNode);
        return childNode;
    }

    // adds an existing child node
    void addChild(Node* child) {
        childNodes.push_back(child);
    }

    // adds a parent
    void addParent(Node* parent) {
        parentNodes.push_back(parent);
    }

    State getState() const { return state; }
    vector<Node*> getParentNode() const { return parentNodes; }
    vector<Node*> getChildNodes() const { return childNodes; }
    int getDepth() const { return depth; }
    int getValue() const { return value; }
    void setValue(int value) { this->value = value; }

};

// tree class
class Tree {
private:
    Node* rootNode;

public:
    Tree() {}

    Tree(State state) {
        rootNode = new Node(state);
    }

    ~Tree() {
        vector<Node*> nodes = getAllNodes();
        for (Node* node : nodes) {
            delete node;
        }
    }

    // generate tree, takes in tree depth as argument, if not given, generates full tree
    void generateTree(int depth = -1) {
        queue<Node*> curLevel;         // nodes at current depth
        map<State, Node*> nextLevel;   // unique nodes at next depth
        vector<State> states;          // nodes possible child states
        Node* curNode;                 // currently looked at node
        int curDepth = 0;              // current depth

        curLevel.push(rootNode);

        // iterate while there are nodes in current level or until depth is reached
        while (!curLevel.empty() && (depth == -1 || curDepth < depth)) {
            curNode = curLevel.front();
            curLevel.pop();

            // generate current node's possible child states
            states = generateChildStates(curNode->getState());
            for (State state : states) {
                auto result = nextLevel.find(state);
                // if state not found in next level, create new node and add it to next level
                if (result == nextLevel.end()) {
                    nextLevel.emplace(state, curNode->addNewChild(state));
                }
                // if state found in next level, connect them
                else {
                    result->second->addParent(curNode);
                    curNode->addChild(result->second);
                }
            }

            // if current level is completed, go to next level
            if (curLevel.empty()) {
                for (const auto& pair : nextLevel) {
                    curLevel.push(pair.second);
                }
                nextLevel.clear();

                curDepth++;
            }
        }
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
