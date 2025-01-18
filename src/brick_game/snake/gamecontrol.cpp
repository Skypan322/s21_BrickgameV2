#include "gamecontrol.h"

#include <iostream>

using namespace s21;

GameControl::GameControl() : field_(std::make_unique<Field>()) {
    game_state_ = GameState::kStart;
    score_ = 1;
    highscore_ = read_highscore(HIGHSCORE_FILE);
}

void GameControl::FiniteStateAutomaton(UserInput input) {
    std::cout << "Gamestate: " << static_cast<int>(game_state_) << std::endl;
    switch (game_state_) {
        case GameState::kStart:
            StartGame();
            break;
        case GameState::kSpawnFood:
            field_->SpawnFood();
            game_state_ = GameState::kMoving;
            FiniteStateAutomaton(input);
            break;
        case GameState::kTurning:
            if (input != UserInput::kNone) {
                TurnSnake(static_cast<Direction>(input));
            }
            game_state_ = GameState::kMoving;
            break;
        case GameState::kMoving:
            MoveSnake();
            game_state_ = GameState::kChecking;
            break;
        case GameState::kGrowing:
            GrowSnake();
            game_state_ = GameState::kTurning;
            break;
        case GameState::kChecking:
            if (input == UserInput::kPause) {
                game_state_ = GameState::kPause;
                break;
            }
            if (field_->snake_->СheckCollision()) {
                game_state_ = GameState::kEnd;
            } else {
                if (field_->snake_->CheckFood(field_->food_)) {
                    game_state_ = GameState::kGrowing;
                } else {
                    game_state_ = GameState::kTurning;
                }
            }

            break;
        case GameState::kPause:
            if (input == UserInput::kPause) {
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
    game_state_ = GameState::kSpawnFood;
    field_->DrawSnake();
    field_->DrawFood();
    last_move_time_ = std::chrono::steady_clock::now();
}

void GameControl::EndGame() { write_highscore(HIGHSCORE_FILE, highscore_); }

void GameControl::MoveSnake(Direction direction) {
    if (game_state_ != GameState::kPause &&
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - last_move_time_)
                .count() > 500) {
        field_->EraseSnake();
        field_->snake_->Move(direction);
        field_->DrawSnake();
        last_move_time_ = std::chrono::steady_clock::now();
    }
}

void GameControl::MoveSnake() {
    GameControl::MoveSnake(field_->snake_->GetDirection());
}

void GameControl::TurnSnake(Direction direction) {
    field_->snake_->Turn(direction);
}

void GameControl::GrowSnake() {
    field_->EraseFood();
    field_->snake_->Grow();
    field_->DrawSnake();
    field_->SpawnFood();
    field_->DrawFood();
    ++score_;
    if (score_ > highscore_) {
        highscore_ = score_;
    }
}
