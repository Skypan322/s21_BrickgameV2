#pragma once
#include <chrono>

#include "field.h"
#include "snake.h"

namespace s21 {
enum class UserInput { kUp, kLeft, kDown, kRight, kPause, kQuit, kNone };

class GameControl {
    enum class GameState {
        kStart,
        kSpawnFood,
        kMoving,
        kGrowing,
        kChecking,
        kPause,
        kEnd
    };

   public:
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

    std::deque<Point> GetSnakeBody() const { return field_->snake_->GetBody(); }

   private:
    std::unique_ptr<Field> field_;
    GameState game_state_;
};

}  // namespace s21