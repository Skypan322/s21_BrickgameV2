#include "mainwindow.h"

#include <QTimer>
#include <iostream>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &MainWindow::updateGame);
    timer_->start(1000);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_initSnakeButton_clicked() {
    current_game_type_ = CurrentGameType::kSnake;
    cur_widget_ = WindowType::kSnakeGame;
    initGame();
}

void MainWindow::on_initTetrisButton_clicked() {
    current_game_type_ = CurrentGameType::kTetris;
    cur_widget_ = WindowType::kTetrisGame;
    initGame();
}

void MainWindow::initGame() {
    ui->stackedWidget->setCurrentIndex(static_cast<int>(cur_widget_));
    if (current_game_type_ == CurrentGameType::kTetris) {
        Tetris *tetris_;
        // tetris_init(tetris_, 0, 0);
    } else if (current_game_type_ == CurrentGameType::kSnake) {
        snake_game_.FiniteStateAutomaton(s21::UserInput::kNone);
    }
}

void MainWindow::on_exitButton_clicked() { QApplication::quit(); }

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (cur_widget_ == WindowType::kSnakeGame) {
        if (current_game_type_ == CurrentGameType::kSnake) {
            last_user_input_ = convertKeyToInput(event->key());
            snake_game_.FiniteStateAutomaton(last_user_input_);
        }
    } else if (cur_widget_ == WindowType::kTetrisGame) {
        if (current_game_type_ == CurrentGameType::kTetris) {
            UserAction action = convertKeyToAction(event->key());
            // game_tick(tetris_, action);
        }
    }
}

UserAction MainWindow::convertKeyToAction(int key) {
    switch (key) {
        case Qt::Key_Left:
            return ROTATE_LEFT;
        case Qt::Key_Right:
            return ROTATE_RIGHT;
        case Qt::Key_Up:
            return MOVE_HARD_DOWN;
        case Qt::Key_Down:
            return MOVE_DOWN;
        case Qt::Key_P:
            return PAUSE_ACTION;
        case Qt::Key_Escape:
            return EXIT_ACTION;
        default:
            return NONE;
    }
}

s21::UserInput MainWindow::convertKeyToInput(int key) {
    switch (key) {
        case Qt::Key_Left:
            return s21::UserInput::kLeft;
        case Qt::Key_Right:
            return s21::UserInput::kRight;
        case Qt::Key_Up:
            return s21::UserInput::kUp;
        case Qt::Key_Down:
            return s21::UserInput::kDown;
        case Qt::Key_P:
            return s21::UserInput::kPause;
        case Qt::Key_Escape:
            return s21::UserInput::kQuit;
        default:
            return s21::UserInput::kNone;
    }
}

void MainWindow::DrawField(
    const std::vector<std::vector<s21::Field::CellType>> &field) {
    for (int i = 0; i < s21::MAX_Y; ++i) {
        for (int j = 0; j < s21::MAX_X; ++j) {
            if (field[i][j] == s21::Field::CellType::kEmpty) {
                std::cout << ".";
            } else if (field[i][j] == s21::Field::CellType::kSnake) {
                std::cout << "O";
            } else if (field[i][j] == s21::Field::CellType::kFood) {
                std::cout << "F";
            }
        }
        std::cout << std::endl;
    }
}

void MainWindow::updateGame() {
    if (current_game_type_ == CurrentGameType::kSnake) {
        snake_game_.FiniteStateAutomaton(last_user_input_);
        DrawField(snake_game_.GetField());
        last_user_input_ = s21::UserInput::kNone;
    }
}

void MainWindow::DrawField() {
    if (current_game_type_ == CurrentGameType::kSnake) {
        DrawField(snake_game_.GetField());
    }
}