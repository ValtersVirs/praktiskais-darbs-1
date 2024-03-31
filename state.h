#ifndef STATE_H
#define STATE_H

#include <vector>

using std::vector;

class State {
private:
    int points, bank;
    vector<int> numbers;

public:
    State() : points(0), bank(0) {}

    State(vector<int> numbers) : points(0), bank(0), numbers(numbers) {}

    State(int length) : points(0), bank(0) {
        // random row of [1;4]
        srand(time(0));
        for (int i = 0; i < length; i++) {
            numbers.push_back(rand() % 4 + 1);
        }
    }

    // completes a player action
    void doAction(int index, bool divide = false) {
        // divide number
        if (divide) {
            // number is 2
            if (this->numbers[index] == 2) {
                this->numbers.insert(this->numbers.begin() + index, 2, 1);
                this->numbers.erase(this->numbers.begin() + index + 2);

                bank++;
            }
            // number is 4
            else if (this->numbers[index] == 4) {
                this->numbers.insert(this->numbers.begin() + index, 2, 2);
                this->numbers.erase(this->numbers.begin() + index + 2);

                points += 2;
            }
        }
        // remove number
        else {
            points += this->numbers[index];

            this->numbers.erase(this->numbers.begin() + index);
        }
    }

    int getPoints() {
        return points;
    }

    int getBank() {
        return bank;
    }

    int getNumber(int index) {
        return numbers[index];
    }

    vector<int> getNumbers() {
        return numbers;
    }

    bool validateIndex(int index) {
        if (index < 0 || index > this->numbers.size() - 1) {
            return false;
        }
        else {
            return true;
        }
    }

    bool isEmpty() {
        if (this->numbers.size() == 0) {
            return true;
        }
        else {
            return false;
        }
    }

    // compare states
    bool operator==(const State& state) const {
        return (
            this->points == state.points &&
            this->bank == state.bank &&
            this->numbers == state.numbers
            );
    }
};

#endif // STATE_H
