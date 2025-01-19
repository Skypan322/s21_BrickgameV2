#include "gamecontrol.h"

using namespace s21;

GameControl::GameControl() : field_(std::make_unique<Field>()) {
    game_state_ = GameState::kStart;
    score_ = 1;
    highscore_ = read_highscore(HIGHSCORE_FILE);
}

/**
 * @brief Handles the finite state automaton for the game control based on user
 * input.
 *
 * This function transitions the game through various states such as starting
 * the game, spawning food, turning the snake, moving the snake, growing the
 * snake, pausing the game, and ending the game. The state transitions are
 * determined by the current game state and the user input.
 *
 * @param input The user input which can be one of the following:
 *              - UserInput::kPause: Pause or resume the game.
 *              - UserInput::kQuit: Quit the game.
 *              - Direction inputs (e.g., UserInput::kUp, UserInput::kDown,
 * etc.) to turn the snake.
 */
void GameControl::FiniteStateAutomaton(UserInput input) {
    // Функция довольно длинная, но это единственный способ сохранить все
    // переходы конечного автомата в ней
    switch (game_state_) {
        case GameState::kStart:
            StartGame();
            break;
        case GameState::kSpawnFood:
            field_->SpawnFood();
            game_state_ = GameState::kTurning;
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

/**
 * @brief Moves the snake in the specified direction.
 *
 * This function erases the current position of the snake from the field,
 * moves the snake in the given direction, and then redraws the snake on the
 * field. It also updates the timestamp of the last move.
 *
 * @param direction The direction in which to move the snake.
 */
void GameControl::MoveSnake(Direction direction) {
    field_->EraseSnake();
    field_->snake_->Move(direction);
    field_->DrawSnake();
    last_move_time_ = std::chrono::steady_clock::now();
}

void GameControl::MoveSnake() {
    GameControl::MoveSnake(field_->snake_->GetDirection());
}

/**
 * @brief Moves the snake in the specified direction.
 *
 * This function erases the current position of the snake from the field,
 * moves the snake in the given direction, and then redraws the snake on the
 * field. It also updates the timestamp of the last move. Turning in the
 * opposite direction is not allowed, it's do nothing.
 *
 * @param direction The direction in which to move the snake.
 */
void GameControl::TurnSnake(Direction direction) {
    field_->snake_->Turn(direction);
}

/**
 * @brief Handles the logic for growing the snake when it eats food.
 *
 * This function performs the following actions:
 * - Erases the food from the field.
 * - Erases the current snake from the field.
 * - Grows the snake by one segment.
 * - Draws the updated snake on the field.
 * - Updates the game state to spawn new food.
 * - Increments the player's score.
 * - Updates the high score if the current score exceeds it.
 * - Checks if the player has won the game by filling the entire field with the
 * snake. If so, it sets the game state to end the game.
 */
void GameControl::GrowSnake() {
    field_->EraseFood();
    field_->EraseSnake();
    field_->snake_->Grow();
    field_->DrawSnake();
    game_state_ = GameState::kSpawnFood;
    ++score_;
    if (score_ > highscore_) {
        highscore_ = score_;
    }
    if (score_ == MAX_X * MAX_Y) {  // score to win
        game_state_ = GameState::kEnd;
    }
}

long GameControl::GetMoveInterval() { return 601 - 3 * score_; }