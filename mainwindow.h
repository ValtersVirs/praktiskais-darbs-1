#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "state.h"
#include "tree.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void initializeSettings();
    void startGame();
    void updateState();
    void updateIndex();
    void gameOver();
    void actionRemove();
    void indexLeft();
    void indexRight();
    void actionDivide();
    void setLength(int);
    void changePlayer();
    void computerMove();

private:
    Ui::MainWindow *ui;

    int points, curIndex, curPlayer;
    vector<int> numbers;
    State state;
    int firstPlayer, algorithmType, length;

    struct {
        int points, bank;
        vector<int> numbers;
    } shownState;
};
#endif // MAINWINDOW_H
