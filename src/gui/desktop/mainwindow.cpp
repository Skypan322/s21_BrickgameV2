#include "mainwindow.h"

#include <QTimer>
#include <iostream>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    timer_ = new QTimer(this);
    current_game_type_ = GameType::kNone;
    last_game_type_ = GameType::kNone;
    connect(timer_, &QTimer::timeout, this, &MainWindow::updateGame);
    timer_->start(50);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_initSnakeButton_clicked() {
    last_user_input_ = s21::UserInput::kNone;
    current_game_type_ = GameType::kSnake;
    cur_widget_ = WindowType::kSnakeGame;
    initGame();
}

void MainWindow::on_initTetrisButton_clicked() {
    current_game_type_ = GameType::kTetris;
    cur_widget_ = WindowType::kSnakeGame;
    initGame();
}

void MainWindow::initGame() {
    ui->stackedWidget->setCurrentIndex(static_cast<int>(cur_widget_));
    if (current_game_type_ == GameType::kTetris) {
        last_tetris_action_ = NONE;
        this->tetris_ = {};
        tetris_init(&tetris_, 1, read_highscore(TETRIS_HIGHSCORE_FILE));
        game_tick(&tetris_, NONE);
    } else if (current_game_type_ == GameType::kSnake) {
        last_user_input_ = s21::UserInput::kNone;
        snake_game_.StartGame();
        snake_game_.FiniteStateAutomaton(s21::UserInput::kNone);
    }
}

void MainWindow::on_exitButton_clicked() { QApplication::quit(); }

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (current_game_type_ == GameType::kSnake) {
        s21::UserInput input = convertKeyToInput(event->key());
        input_queue_.enqueue(input);
    } else if (current_game_type_ == GameType::kTetris) {
        UserAction action = convertKeyToAction(event->key());
        last_tetris_action_ = action;
    }
}

UserAction MainWindow::convertKeyToAction(int key) {
    switch (key) {
        case Qt::Key_K:
            return ROTATE_LEFT;
        case Qt::Key_L:
            return ROTATE_RIGHT;
        case Qt::Key_Left:
            return MOVE_LEFT;
        case Qt::Key_Right:
            return MOVE_RIGHT;
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

void MainWindow::updateGame() {
    if (current_game_type_ == GameType::kSnake) {
        if (!input_queue_.isEmpty()) {
            last_user_input_ = input_queue_.dequeue();
        }
        snake_game_.FiniteStateAutomaton(last_user_input_);
        last_user_input_ = s21::UserInput::kNone;
        ui->level->setText(QString::number(snake_game_.GetMoveInterval()));
        ui->score->setText(QString::number(snake_game_.GetScore()));
        ui->bestScore->setText(QString::number(snake_game_.GetHighScore()));
        if (snake_game_.GetGameState() == s21::GameControl::GameState::kEnd) {
            HandleSnakeGameover();
        }

    } else if (current_game_type_ == GameType::kTetris) {
        if (!tetris_input_queue_.isEmpty()) {
            last_tetris_action_ = tetris_input_queue_.dequeue();
        }
        game_tick(&tetris_, last_tetris_action_);
        ui->score->setText(QString::number(tetris_.score));
        ui->bestScore->setText(QString::number(tetris_.highscore));
        ui->level->setText(QString::number(tetris_.level));
        last_tetris_action_ = NONE;
        if (tetris_.state == GAMEOVER) {
            HandleTetrisGameover();
        }
    }
    repaint();
}

void MainWindow::HandleTetrisGameover() {
    current_game_type_ = GameType::kNone;
    cur_widget_ = WindowType::kGameover;
    ui->stackedWidget->setCurrentIndex(static_cast<int>(cur_widget_));
    ui->gm_best_score->setText(QString::number(tetris_.highscore));
    ui->gm_score->setText(QString::number(tetris_.score));
    write_highscore(TETRIS_HIGHSCORE_FILE, tetris_.highscore);

    last_game_type_ = GameType::kTetris;
}

void MainWindow::HandleSnakeGameover() {
    current_game_type_ = GameType::kNone;
    cur_widget_ = WindowType::kGameover;
    ui->stackedWidget->setCurrentIndex(static_cast<int>(cur_widget_));
    ui->gm_best_score->setText(QString::number(snake_game_.GetHighScore()));
    ui->gm_score->setText(QString::number(snake_game_.GetScore()));
    write_highscore(snake_game_.GetHighScoreFile(), snake_game_.GetHighScore());

    last_game_type_ = GameType::kSnake;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    if (current_game_type_ == GameType::kSnake ||
        current_game_type_ == GameType::kTetris) {
        DrawField();
    }
}

void MainWindow::DrawField() {
    if (current_game_type_ == GameType::kSnake) {
        auto field = snake_game_.GetField();
        DrawMatrix(field, ui->SnakeField->geometry());
    } else if (current_game_type_ == GameType::kTetris) {
        auto field = tetris_.field;
        DrawMatrix(field, ui->SnakeField->geometry());
    }
}

void MainWindow::DrawMatrix(const int field[s21::MAX_Y][s21::MAX_X],
                            const QRect &ui_element) {
    QPainter painter(this);
    int pixel_size = s21::sizes::pixel_size;
    for (int i = 0; i < s21::MAX_Y; ++i) {
        for (int j = 0; j < s21::MAX_X; ++j) {
            int color_num = field[i][j];
            painter.setBrush(colors_[color_num]);
            painter.drawRect(ui_element.x() + j * pixel_size,
                             ui_element.y() + i * pixel_size, pixel_size,
                             pixel_size);
        }
    }
}

void MainWindow::DrawMatrix(
    const std::vector<std::vector<s21::Field::CellType>> &field,
    const QRect &ui_element) {
    QPainter painter(this);
    int pixel_size = s21::sizes::pixel_size;
    for (int i = 0; i < field.size(); ++i) {
        for (int j = 0; j < field[i].size(); ++j) {
            int color_num = static_cast<int>(field[i][j]);
            painter.setBrush(colors_[color_num]);
            painter.drawRect(ui_element.x() + j * pixel_size,
                             ui_element.y() + i * pixel_size, pixel_size,
                             pixel_size);
        }
    }
}

void MainWindow::on_gm_play_again_clicked() {
    current_game_type_ = last_game_type_;
    cur_widget_ = WindowType::kSnakeGame;
    initGame();
}

void MainWindow::on_gm_to_menu_clicked() {
    cur_widget_ = WindowType::kMenu;
    ui->stackedWidget->setCurrentIndex(static_cast<int>(cur_widget_));
}
