#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColor>
#include <QKeyEvent>
#include <QMainWindow>
#include <QPainter>
#include <QQueue>
#include <QRect>
#include <QTimer>

#include "../../brick_game/snake/gamecontrol.h"
#include "drawing.h"

extern "C" {
#include "../../brick_game/common/utils.h"
#include "../../brick_game/tetris/figures.h"
#include "../../brick_game/tetris/tetris.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum GameType { kSnake, kTetris, kNone };
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
  void paintEvent(QPaintEvent *event);

  void on_gm_play_again_clicked();

  void on_gm_to_menu_clicked();

 private:
  Ui::MainWindow *ui;
  GameType current_game_type_;
  GameType last_game_type_;
  WindowType cur_widget_;
  QTimer *timer_;
  s21::UserInput last_user_input_;
  UserAction last_tetris_action_;
  QQueue<s21::UserInput> input_queue_;
  QQueue<UserAction> tetris_input_queue_;

  void initGame();
  UserAction convertKeyToAction(int key);
  s21::UserInput convertKeyToInput(int key);
  void keyPressEvent(QKeyEvent *event) override;
  Tetris tetris_;
  s21::GameControl snake_game_;
  void DrawField();

  void DrawMatrix(const int field[s21::MAX_Y][s21::MAX_X],
                  const QRect &ui_element);

  void DrawMatrix(const std::vector<std::vector<s21::Field::CellType>> &field,
                  const QRect &ui_element);
  void updateGame();
  void HandleSnakeGameover();
  void HandleTetrisGameover();

  const QColor colors_[3] = {
      QColor(0, 0, 0),      // black
      QColor(200, 0, 200),  // purple
      QColor(255, 255, 0)   // yellow
  };
};
#endif  // MAINWINDOW_H
