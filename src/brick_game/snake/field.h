#pragma once
#include <memory>
#include <random>
#include <vector>

#include "snake.h"

namespace s21 {
class Field {
   public:
    enum class CellType { kEmpty, kSnake, kFood };

    Field();
    ~Field() = default;
    void SpawnFood();
    void EraseFood();
    void EraseSnake();
    void DrawSnake();
    void DrawFood();
    bool CheckSnakeOnFood();

   private:
    std::vector<std::vector<CellType>>
        field_;  // C-Array could do, but vector is more convenient
    std::unique_ptr<Snake> snake_;
    Point food_;

    friend class GameControl;
};

}  // namespace s21