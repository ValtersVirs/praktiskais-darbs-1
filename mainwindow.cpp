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
    state = State(length);

    curIndex = 0;

    if (ui->radioUser->isChecked()) {
        firstPlayer = 1;
    } else {
        firstPlayer = 2;
    }
    // todo: curPlayer = lietotājs vai dators
    curPlayer = 1;
    ui->lblCurrentPlayer->setText(QString("Spēlētājs ").append(QString::number(curPlayer)));

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

    if (state.isEmpty()) {
        winnerFound();
    }

    if (curIndex >= state.getNumbers().size()) {
        curIndex = state.getNumbers().size() - 1;
    }



    updateIndex();
}

void MainWindow::updateIndex() {
    if (state.isEmpty()) {
        ui->lblNumbers->setText("");
        return;
    }

    QString numberString;
    for (int i = 0; i < state.getNumbers().size(); ++i) {
        if (curIndex == i) {
            numberString += "<font color='red'>";
            numberString += QString::number(state.getNumber(i));
            numberString += "</font>";
        } else {
            numberString += QString::number(state.getNumber(i));
        }
    }
    ui->lblNumbers->setText(numberString);

    if (state.getNumber(curIndex) == 2 || state.getNumber(curIndex) == 4) {
        ui->btnDivide->setDisabled(false);
    } else {
        ui->btnDivide->setDisabled(true);
    }
}

void MainWindow::winnerFound() {
    ui->btnRemove->setVisible(false);
    ui->btnDivide->setVisible(false);
    ui->btnLeft->setVisible(false);
    ui->btnRight->setVisible(false);
    ui->btnNewGame->setVisible(true);
    ui->lblCurrentPlayer->setVisible(false);

    // to do player 1, player 2 -> user, computer
    if (!(state.getPoints() % 2) && !(state.getBank() % 2)) {
        ui->lblWinner->setText("Spēlētājs 1 uzvarēja");
    } else if ((state.getPoints() % 2) && (state.getBank() % 2)) {
        ui->lblWinner->setText("Spēlētājs 2 uzvarēja");
    } else {
        ui->lblWinner->setText("Neizšķirts");
    }

    ui->lblWinner->setVisible(true);
}

void MainWindow::actionRemove()
{
    if (state.isEmpty()) return;

    state.doAction(curIndex, false);

    changePlayer();
    updateState();
}

void MainWindow::actionDivide()
{
    if (state.isEmpty()) return;
    if (!(state.getNumber(curIndex) == 2 || state.getNumber(curIndex) == 4)) return;

    state.doAction(curIndex, true);

    changePlayer();
    updateState();
}

void MainWindow::indexLeft()
{
    if (curIndex > 0) {
        curIndex--;
    }
    updateIndex();
}

void MainWindow::indexRight()
{
    if (curIndex < state.getNumbers().size() - 1) {
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

    ui->lblCurrentPlayer->setText(QString("Spēlētājs ").append(QString::number(curPlayer)));
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
