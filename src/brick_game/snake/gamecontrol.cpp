#include "gamecontrol.h"

#include <iostream>

using namespace s21;

GameControl::GameControl() : field_(std::make_unique<Field>()) {
    game_state_ = GameState::kStart;
    score_ = 1;
    highscore_ = read_highscore(HIGHSCORE_FILE);
}

void GameControl::FiniteStateAutomaton(UserInput input) {
    switch (game_state_) {
        case GameState::kStart:
            StartGame();
            break;
        case GameState::kSpawnFood:
            field_->SpawnFood();
            field_->DrawFood();
            game_state_ = GameState::kTurning;
            // FiniteStateAutomaton(input);
            break;
        case GameState::kTurning:
            if (input == UserInput::kPause) {
                game_state_ = GameState::kPause;
                break;
            }
            if (input == UserInput::kQuit) {
                game_state_ = GameState::kEnd;
                break;
            }
            if (CheckRightInputDirection(input)) {
                TurnSnake(static_cast<Direction>(input));
            }
            if (std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - last_move_time_)
                    .count() > GetMoveInterval()) {
                game_state_ = GameState::kMoving;
            }
            break;
        case GameState::kMoving:
            MoveSnake();
            game_state_ = GameState::kTurning;
            if (field_->snake_->СheckCollision()) {
                game_state_ = GameState::kEnd;
            } else if (field_->CheckSnakeOnFood()) {
                game_state_ = GameState::kGrowing;
            } else {
                game_state_ = GameState::kTurning;
            }
            break;
        case GameState::kGrowing:
            GrowSnake();
            game_state_ = GameState::kSpawnFood;
            break;
        case GameState::kPause:
            if (input == UserInput::kPause) {
                last_move_time_ = std::chrono::steady_clock::now();
                game_state_ = GameState::kTurning;
            }
            break;
        case GameState::kEnd:
            std::cout << "End" << std::endl;
            EndGame();
            break;
    }
}

void GameControl::StartGame() {
    field_.reset(new Field());
    game_state_ = GameState::kTurning;
    field_->DrawSnake();
    field_->DrawFood();
    last_move_time_ = std::chrono::steady_clock::now();
    highscore_ = read_highscore(HIGHSCORE_FILE);
}

void GameControl::EndGame() { write_highscore(HIGHSCORE_FILE, highscore_); }

void GameControl::MoveSnake(Direction direction) {
    field_->EraseSnake();
    field_->snake_->Move(direction);
    field_->DrawSnake();
    last_move_time_ = std::chrono::steady_clock::now();
}

void GameControl::MoveSnake() {
    GameControl::MoveSnake(field_->snake_->GetDirection());
}

void GameControl::TurnSnake(Direction direction) {
    field_->snake_->Turn(direction);
}

void GameControl::GrowSnake() {
    std::cout << "Growing" << std::endl;
    field_->EraseFood();
    field_->EraseSnake();
    field_->snake_->Grow();
    field_->DrawSnake();
    game_state_ = GameState::kSpawnFood;
    ++score_;
    if (score_ > highscore_) {
        highscore_ = score_;
    }
}

long GameControl::GetMoveInterval() { return 601 - 3 * score_; }