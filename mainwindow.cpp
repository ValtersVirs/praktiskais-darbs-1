#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>

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
    //shownState.numbers = { 3, 2, 1, 4 };

    srand(time(0));
    for (int i = 0; i < length; i++) {
        shownState.numbers.push_back(rand() % 4 + 1);
    }

    state = State(shownState.numbers);

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

    ui->stackedWidget->setCurrentWidget(ui->pageMain);
    ui->lblWinner->setVisible(false);
    ui->btnRemove->setVisible(true);
    ui->btnDivide->setVisible(true);
    ui->btnLeft->setVisible(true);
    ui->btnRight->setVisible(true);
    ui->btnStartGame->setVisible(false);
    ui->lblCurrentPlayer->setVisible(true);

    updateState();
}

void MainWindow::updateState() {
    ui->lblPointsNum->setText(QString::number(state.getPoints()));
    ui->lblBankNum->setText(QString::number(state.getBank()));

    if (state.hasFinished()) {
        gameOver();
    }

    if (curIndex >= shownState.numbers.size()) {
        curIndex = shownState.numbers.size() - 1;
    }

    updateIndex();
}

void MainWindow::updateIndex() {
    if (state.hasFinished()) {
        ui->lblNumbers->setText("");
        return;
    }

    QString numberString;
    for (int i = 0; i < shownState.numbers.size(); ++i) {
        if (curIndex == i) {
            numberString += "<font color='red'>";
            numberString += QString::number(shownState.numbers[i]);
            numberString += "</font>";
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
    if (state.hasFinished()) return;

    int number = shownState.numbers[curIndex];
    shownState.numbers.erase(shownState.numbers.begin() + curIndex);

    state.doAction(number, false);

    shownState.points = state.getPoints();

    changePlayer();
    updateState();
    //computerMove();
}

void MainWindow::actionDivide() {
    if (state.hasFinished()) return;

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

    changePlayer();
    updateState();
    //computerMove();
}

void MainWindow::computerMove() {
    Tree tree(state);

    clock_t start = clock();

    tree.generateTree();

    clock_t end = clock();
    double duration = double(end - start) / CLOCKS_PER_SEC;

    //qDebug() << "Tree generated " << duration;

    updateState();
}

void MainWindow::indexLeft() {
    if (curIndex > 0) {
        curIndex--;
    }
    updateIndex();
}

void MainWindow::indexRight() {
    if (curIndex < shownState.numbers.size() - 1) {
        curIndex++;
    }
    updateIndex();
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
        ui->lblCurrentPlayer->setText(QString("Lietotājs"));
    } else {
        ui->lblCurrentPlayer->setText(QString("Dators"));
    }

    //ui->lblCurrentPlayer->setText(QString("Spēlētājs ").append(QString::number(curPlayer)));
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
