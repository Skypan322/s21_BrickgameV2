#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>

#include "../../brick_game/snake/gamecontrol.h"
#include "drawing.h"

extern "C" {
#include "../../brick_game/tetris/tetris.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum CurrentGameType { kSnake, kTetris, kNone };
enum WindowType { kMenu, kGameover, kTetrisGame, kSnakeGame };

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private slots:
    void on_initSnakeButton_clicked();

    void on_initTetrisButton_clicked();

    void on_exitButton_clicked();

   private:
    Ui::MainWindow *ui;
    CurrentGameType current_game_type_;
    WindowType cur_widget_;
    QTimer *timer_;
    s21::UserInput last_user_input_;

    void initGame();
    UserAction convertKeyToAction(int key);
    s21::UserInput convertKeyToInput(int key);
    void keyPressEvent(QKeyEvent *event) override;
    Tetris *tetris_;
    s21::GameControl snake_game_;
    void DrawField(const std::vector<std::vector<s21::Field::CellType>> &field);
    void DrawField(const int field[20][10]);
    void DrawField();
    void updateGame();
};
#endif  // MAINWINDOW_H
