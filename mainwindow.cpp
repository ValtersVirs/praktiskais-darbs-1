#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QThread>
#include "minimax.h"
#include "alfabeta.h"

const int MAX = numeric_limits<int>::max();  // highest possible value
const int MIN = numeric_limits<int>::min();  // lowest possible value
int nodeCount;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initializeSettings();

    connect(ui->btnStartGame, SIGNAL (clicked()), this, SLOT (startGame()));
    connect(ui->btnRemove, SIGNAL (clicked()), this, SLOT (actionRemove()));
    connect(ui->btnDivide, SIGNAL (clicked()), this, SLOT (actionDivide()));
    connect(ui->btnLeft, SIGNAL (clicked()), this, SLOT (indexLeft()));
    connect(ui->btnRight, SIGNAL (clicked()), this, SLOT (indexRight()));
    connect(ui->btnNewGame, SIGNAL (clicked()), this, SLOT (initializeSettings()));
    connect(ui->sliderLength, SIGNAL (valueChanged(int)), this, SLOT (setLength(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeSettings() {
    setLength(15);

    ui->lblWinner->setVisible(false);
    ui->btnStartGame->setVisible(true);
    ui->btnNewGame->setVisible(false);
    ui->stackedWidget->setCurrentWidget(ui->pageStart);

    ui->radioUser->setChecked(true);
    ui->radioMinimax->setChecked(true);
}

void MainWindow::startGame() {
    shownState.bank = 0;
    shownState.points = 0;

    srand(time(0));
    for (int i = 0; i < length; i++) {
        shownState.numbers.push_back(rand() % 4 + 1);
    }

    state = State(shownState.numbers);

    nodeCount = 0;
    totalNodeCount = 0;
    curIndex = 0;

    if (ui->radioUser->isChecked()) {
        firstPlayer = 1;
        ui->lblCurrentPlayer->setText(QString("Lietotājs"));
    } else {
        firstPlayer = 2;
        ui->lblCurrentPlayer->setText(QString("Dators"));
    }

    curPlayer = firstPlayer;

    if (ui->radioMinimax->isChecked()) {
        algorithmType = 1;
    } else {
        algorithmType = 2;
    }

    depth = ui->spnDepth->value();

    ui->stackedWidget->setCurrentWidget(ui->pageMain);
    ui->lblWinner->setVisible(false);
    ui->btnRemove->setVisible(true);
    ui->btnDivide->setVisible(true);
    ui->btnLeft->setVisible(true);
    ui->btnRight->setVisible(true);
    ui->btnStartGame->setVisible(false);
    ui->lblCurrentPlayer->setVisible(true);
    ui->lblTurn->setVisible(true);
    ui->lblTurnTime->setVisible(true);
    ui->lblTime->setVisible(true);
    ui->lblNode->setVisible(true);
    ui->lblNodeCount->setVisible(true);
    ui->lblNodeEnd->setVisible(false);
    ui->lblNodeCountEnd->setVisible(false);

    ui->lblTime->setText("");
    ui->lblNodeCount->setText("");

    updateState();

    if (firstPlayer == 2) {
        computerMove();
    }
}

void MainWindow::updateState() {
    if (curPlayer == 1) {
        ui->btnLeft->setDisabled(false);
        ui->btnRight->setDisabled(false);
        ui->btnRemove->setDisabled(false);
        ui->btnDivide->setDisabled(false);
    }

    ui->lblPointsNum->setText(QString::number(state.getPoints()));
    ui->lblBankNum->setText(QString::number(state.getBank()));

    if (state.hasFinished()) {
        gameOver();
        ui->lblNumbers->setText("");
        return;
    }

    if (curIndex >= shownState.numbers.size()) {
        curIndex = shownState.numbers.size() - 1;
    }

    QString numberString = "";
    for (int i = 0; i < shownState.numbers.size(); ++i) {
        if (curIndex == i) {
            numberString += "<font color='blue'><b>[";
            numberString += QString::number(shownState.numbers[i]);
            numberString += "]</b></font>";
        } else {
            numberString += QString::number(shownState.numbers[i]);
        }
    }
    ui->lblNumbers->setText(numberString);

    if (shownState.numbers[curIndex] == 2 || shownState.numbers[curIndex] == 4) {
        ui->btnDivide->setDisabled(false);
    } else {
        ui->btnDivide->setDisabled(true);
    }
}

void MainWindow::gameOver() {
    ui->btnRemove->setVisible(false);
    ui->btnDivide->setVisible(false);
    ui->btnLeft->setVisible(false);
    ui->btnRight->setVisible(false);
    ui->btnNewGame->setVisible(true);
    ui->lblCurrentPlayer->setVisible(false);
    ui->lblTurn->setVisible(false);
    ui->lblTurnTime->setVisible(false);
    ui->lblTime->setVisible(false);
    ui->lblNode->setVisible(false);
    ui->lblNodeCount->setVisible(false);
    ui->lblNodeEnd->setVisible(true);
    ui->lblNodeCountEnd->setVisible(true);

    ui->lblNodeCountEnd->setText(QString::number(totalNodeCount));

    int winner = state.getWinner();

    if (winner == 1) {
        if (firstPlayer == 1) {
            ui->lblWinner->setText("Uzvarēja lietotājs");
        } else {
            ui->lblWinner->setText("Uzvarēja dators");
        }
    } else if (winner == 2) {
        if (firstPlayer == 1) {
            ui->lblWinner->setText("Uzvarēja dators");
        } else {
            ui->lblWinner->setText("Uzvarēja lietotājs");
        }
    } else {
        ui->lblWinner->setText("Neizšķirts");
    }

    ui->lblWinner->setVisible(true);
}

void MainWindow::actionRemove() {
    if (state.hasFinished() || curPlayer == 2) return;

    int number = shownState.numbers[curIndex];
    shownState.numbers.erase(shownState.numbers.begin() + curIndex);

    state.doAction(number, false);

    shownState.points = state.getPoints();

    updateState();
    changePlayer();
}

void MainWindow::actionDivide() {
    if (state.hasFinished() || curPlayer == 2) return;

    int number = shownState.numbers[curIndex];

    if (number == 2) {
        shownState.numbers.insert(shownState.numbers.begin() + curIndex, 2, 1);
        shownState.numbers.erase(shownState.numbers.begin() + curIndex + 2);
    } else if (number == 4) {
        shownState.numbers.insert(shownState.numbers.begin() + curIndex, 2, 2);
        shownState.numbers.erase(shownState.numbers.begin() + curIndex + 2);
    } else {
        return;
    }

    state.doAction(number, true);

    shownState.points = state.getPoints();
    shownState.bank = state.getBank();

    updateState();
    changePlayer();
}

void MainWindow::computerMove() {
    // set screen for computer move
    ui->btnLeft->setDisabled(true);
    ui->btnRight->setDisabled(true);
    ui->btnRemove->setDisabled(true);
    ui->btnDivide->setDisabled(true);

    QString numberString = "";
    for (int number : shownState.numbers) {
        numberString += QString::number(number);
    }
    ui->lblNumbers->setText(numberString);
    ui->stackedWidget->repaint();
    //qApp->processEvents();

    clock_t startTime, endTime;

    startTime = clock();
    // generate tree
    Tree tree(state);
    tree.generateTree(depth);
    endTime = clock();

    double treeTime = double(endTime - startTime) / CLOCKS_PER_SEC;

    bool isMaxPlayer = (firstPlayer == 2);

    int optimalValue;
    nodeCount = 0;

    startTime = clock();
    if (algorithmType == 1) {
        optimalValue = minimax(tree.getRoot(), isMaxPlayer, depth);
    } else {
        optimalValue = alfabeta(tree.getRoot(), isMaxPlayer, depth, MIN, MAX);
    }
    endTime = clock();

    double algorithmTime = double(endTime - startTime) / CLOCKS_PER_SEC;
    double totalTime = treeTime + algorithmTime;

    totalNodeCount += nodeCount;
    State bestState;

    // get next state
    for (Node* node : tree.getRoot()->getChildNodes()) {
        if (node->getValue() == optimalValue) {
            bestState = node->getState();
            break;
        }
    }

    int actionNumber = 0;       // computer picked number
    bool actionOption = false;  // computer picked option
    State tempState;

    // find action that led to state
    for (int number : state.getUniqueNumbers()) {
        actionNumber = number;
        actionOption = false;
        tempState = state;
        tempState.doAction(actionNumber, actionOption);
        if (tempState == bestState) break;

        if (actionNumber == 2 || actionNumber == 4) {
            actionOption = true;
            tempState = state;
            tempState.doAction(actionNumber, actionOption);
            if (tempState == bestState) break;
        }
    }
    state = bestState;

    // find number position
    int index = 0;
    for (int i = 0; i < shownState.numbers.size(); i++) {
        if (shownState.numbers[i] == actionNumber) {
            index = i;
            break;
        }
    }

    // show computer picked number
    numberString = "";
    for (int i = 0; i < shownState.numbers.size(); ++i) {
        if (index == i) {
            numberString += "<font color='red'><b>[";
            numberString += QString::number(shownState.numbers[i]);
            numberString += "]</b></font>";
        } else {
            numberString += QString::number(shownState.numbers[i]);
        }
    }
    ui->lblNumbers->setText(numberString);
    ui->lblTime->setText(QString::number(totalTime) + "s");
    ui->lblNodeCount->setText(QString::number(nodeCount));
    ui->stackedWidget->repaint();

    // wait to display computer move
    QThread::sleep(1);

    // reflect next state on screen state
    if (actionOption) {
        if (actionNumber == 2) {
            shownState.numbers.insert(shownState.numbers.begin() + index, 2, 1);
            shownState.numbers.erase(shownState.numbers.begin() + index + 2);
        } else if (actionNumber == 4) {
            shownState.numbers.insert(shownState.numbers.begin() + index, 2, 2);
            shownState.numbers.erase(shownState.numbers.begin() + index + 2);
        }
    } else {
        shownState.numbers.erase(shownState.numbers.begin() + index);
    }

    shownState.points = state.getPoints();
    shownState.bank = state.getBank();

    changePlayer();
    updateState();
}

void MainWindow::computerState(int number, bool option) {}

void MainWindow::indexLeft() {
    if (curIndex > 0) {
        curIndex--;
    }
    updateState();
}

void MainWindow::indexRight() {
    if (curIndex < shownState.numbers.size() - 1) {
        curIndex++;
    }
    updateState();
}

void MainWindow::setLength(int value) {
    length = value;
    QString text = "Virknes garums ";
    text += QString::number(value);
    ui->lblLength->setText(text);
}

void MainWindow::changePlayer() {
    curPlayer = curPlayer == 1 ? 2 : 1;

    if (curPlayer == 1) {
        ui->lblCurrentPlayer->setText(QString("<font color='blue'>Lietotājs</font>"));
    } else {
        ui->lblCurrentPlayer->setText(QString("<font color='red'>Dators</font>"));
    }

    if (!state.hasFinished() && curPlayer == 2) {
        computerMove();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (ui->stackedWidget->currentWidget() == ui->pageMain) {
        if (event->key() == Qt::Key_A) {
            indexLeft();
            return;
        }
        if (event->key() == Qt::Key_D) {
            indexRight();
            return;
        }
        if (event->key() == Qt::Key_Return) {
            actionRemove();
            return;
        }
        if (event->key() == Qt::Key_Backspace) {
            actionDivide();
            return;
        }
    }

    QMainWindow::keyPressEvent(event);
}
