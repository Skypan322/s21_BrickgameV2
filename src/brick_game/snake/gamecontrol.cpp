#include "gamecontrol.h"

using namespace s21;

GameControl::GameControl() : field_(std::make_unique<Field>()) {}

void GameControl::FiniteStateAutomaton(UserInput input) {
    switch (game_state_) {
        case GameState::kStart:
            StartGame();
            break;
        case GameState::kSpawnFood:
            field_->SpawnFood();
            game_state_ = GameState::kMoving;
            break;
        case GameState::kMoving:
            if (input != UserInput::kNone) {
                MoveSnake(static_cast<Direction>(input));
            } else {
                MoveSnake();
            }
            game_state_ = GameState::kChecking;
            break;
        case GameState::kGrowing:
            GrowSnake();
            break;
        case GameState::kChecking:
            if (field_->snake_->СheckCollision()) {
                game_state_ = GameState::kEnd;
            } else {
                if (field_->snake_->CheckFood(field_->food_)) {
                    game_state_ = GameState::kGrowing;
                } else {
                    game_state_ = GameState::kMoving;
                }
            }
            if (input == UserInput::kPause) {
                game_state_ = GameState::kPause;
            }
            break;
        case GameState::kPause:
            if (input == UserInput::kPause) {
                game_state_ = GameState::kMoving;
            }
            break;
        case GameState::kEnd:
            EndGame();
            break;
    }
}

void GameControl::StartGame() {
    game_state_ = GameState::kSpawnFood;
    field_->DrawSnake();
    field_->DrawFood();
    last_move_time_ = std::chrono::steady_clock::now();
}

void GameControl::EndGame() {
    field_->EraseSnake();
    field_->EraseFood();
}

void GameControl::MoveSnake(Direction direction) {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - last_move_time_)
            .count() > 10) {
        field_->EraseSnake();
        field_->snake_->Move(direction);
        field_->DrawSnake();
    }
}

void GameControl::MoveSnake() { MoveSnake(field_->snake_->GetDirection()); }

void GameControl::TurnSnake(Direction direction) {
    field_->snake_->Turn(direction);
}

void GameControl::GrowSnake() {
    field_->EraseFood();
    field_->snake_->Grow(field_->food_);
    field_->DrawSnake();
    field_->SpawnFood();
    field_->DrawFood();
}
