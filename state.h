#ifndef STATE_H
#define STATE_H

#include <vector>
#include <map>

using namespace std;

class State {
private:
    int points, bank;
    map<int, int> numbers;

public:

    State() : points(0), bank(0) {
        numbers[1] = 0;
        numbers[2] = 0;
        numbers[3] = 0;
        numbers[4] = 0;
    }

    State(vector<int> numbers) : points(0), bank(0) {
        for (int number : numbers) {
            if (number >= 1 && number <= 4)
                this->numbers[number]++;
        }
    }

    State(int length) : points(0), bank(0) {
        int number;
        // random row of [1;4]
        srand(time(0));
        for (int i = 0; i < length; i++) {
            number = rand() % 4 + 1;
            numbers[number]++;
        }
    }

    // completes a player action
    void doAction(int number, bool divide = false) {
        // divide number
        if (divide) {
            // number is 2
            if (number == 2) {
                numbers[2]--;
                numbers[1] += 2;

                bank++;
            }
            // number is 4
            else if (number == 4) {
                numbers[4]--;
                numbers[2] += 2;

                points += 2;
            }
        }
        // remove number
        else {
            numbers[number]--;

            points += number;
        }
    }

    /*
    returns winner
    1 = player 1
    2 = player 2
    3 = draw
    0 = game hasn't finished
    */
    int getWinner() {
        if (!this->hasFinished()) return 0;

        // points and bank is even, winner player 1
        if (!(points % 2) && !(bank % 2)) {
            return 1;
        }
        // points and bank is not even, winner player 2
        if (points % 2 && bank % 2) {
            return 2;
        }
        // draw
        return 3;
    }

    // returns all numbers as a vector
    vector<int> getNumbers() {
        vector<int> tempNumbers;

        for (const auto& pair : numbers) {
            for (int i = 0; i < pair.second; i++) {
                tempNumbers.push_back(pair.first);
            }
        }

        return tempNumbers;
    }

    // returns only unique numbers
    vector<int> getUniqueNumbers() {
        vector<int> tempNumbers;

        for (const auto& pair : numbers) {
            if (pair.second > 0) {
                tempNumbers.push_back(pair.first);
            }
        }

        return tempNumbers;
    }

    map<int, int> getNumberMap() {
        return numbers;
    }

    int getPoints() { return points; }
    int getBank() { return bank; }

    bool validateNumber(int number) {
        for (const auto& pair : numbers) {
            if (pair.first == number && pair.second > 0) {
                return true;
            }
        }

        return false;
    }

    bool hasFinished() {
        for (const auto& pair : numbers) {
            if (pair.second > 0) {
                return false;
            }
        }

        return true;
    }

    // compare operator
    bool operator==(const State& state) const {
        return (
            this->points == state.points &&
            this->bank == state.bank &&
            this->numbers == state.numbers
            );
    }

    // less than operator
    bool operator<(const State& state) const {
        if (points != state.points) {
            return this->points < state.points;
        }
        if (bank != state.bank) {
            return this->bank < state.bank;
        }
        return this->numbers < state.numbers;
    }
};

#endif // STATE_H
