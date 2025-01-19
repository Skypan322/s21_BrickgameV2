#pragma once
#include <chrono>

#include "../common/utils.h"
#include "field.h"
#include "snake.h"

namespace s21 {
enum class UserInput { kUp, kLeft, kDown, kRight, kPause, kQuit, kNone };

class GameControl {
 public:
  enum class GameState {
    kStart,
    kSpawnFood,
    kMoving,
    kGrowing,
    kTurning,
    kPause,
    kEnd
  };

  GameControl();
  ~GameControl() = default;
  void StartGame();
  void EndGame();
  void MoveSnake(Direction direction);
  void MoveSnake();
  void TurnSnake(Direction direction);
  void FiniteStateAutomaton(UserInput input);
  void GrowSnake();
  std::chrono::time_point<std::chrono::steady_clock> last_move_time_;
  std::vector<std::vector<Field::CellType>> GetField() const {
    return field_->field_;
  }
  long GetScore() const { return score_; }
  long GetHighScore() const { return highscore_; }
  GameState GetGameState() const { return game_state_; }
  const char* GetHighScoreFile() const { return HIGHSCORE_FILE; }
  bool CheckRightInputDirection(UserInput input) {
    return ((input == UserInput::kUp) || (input == UserInput::kDown) ||
            (input == UserInput::kLeft) || (input == UserInput::kRight));
  }
  long GetMoveInterval();

  std::deque<Point> GetSnakeBody() const { return field_->snake_->GetBody(); }

 private:
  std::unique_ptr<Field> field_;
  GameState game_state_;
  long score_;
  long highscore_;
  const char* HIGHSCORE_FILE = "highscore_snake.txt";
};

}  // namespace s21