#include "field.h"

#include <iostream>

using namespace s21;

/**
 * @brief Constructs a Field object and initializes the game field.
 *
 * This constructor initializes the game field by resizing it to the maximum
 * dimensions defined by MAX_Y and MAX_X. Each cell in the field is set to
 * CellType::kEmpty. Additionally, a Snake object is created and assigned to
 * the snake_ member variable. Finally, the SpawnFood() function is called to
 * place food on the field.
 */
Field::Field() : snake_(std::make_unique<Snake>()) {
  field_.resize(MAX_Y);
  for (int i = 0; i < MAX_Y; ++i) {
    field_[i].resize(MAX_X);
    for (int j = 0; j < MAX_X; ++j) {
      field_[i][j] = CellType::kEmpty;
    }
  }
  SpawnFood();
}

/**
 * @brief Spawns food at a random empty location on the field.
 *
 * This function generates random coordinates within the field's boundaries
 * and places food at an empty cell. It ensures that the food is not placed
 * on a cell that is already occupied by another object.
 */
void Field::SpawnFood() {
  int x = rand() % MAX_X;
  int y = rand() % MAX_Y;
  while (field_[y][x] != CellType::kEmpty) {
    x = rand() % MAX_X;
    y = rand() % MAX_Y;
  }
  food_ = Point(x, y);
  field_[y][x] = CellType::kFood;
}
/**
 * @brief Erases food.
 */
void Field::EraseFood() { field_[food_.y][food_.x] = CellType::kEmpty; };

/**
 * @brief Erases the snake from the field.
 *
 * This function iterates through the body of the snake and sets the
 * corresponding cells in the field to empty. If any part of the snake's body is
 * out of the field boundaries, the function stops erasing.
 */
void Field::EraseSnake() {
  std::deque<Point> body = snake_->GetBody();
  for (auto it = body.begin(); it != body.end(); ++it) {
    if (it->x < 0 || it->x >= MAX_X || it->y < 0 || it->y >= MAX_Y) {
      break;
    }
    field_[it->y][it->x] = CellType::kEmpty;
  }
};

/**
 * @brief Draws food.
 */
void Field::DrawFood() { field_[food_.y][food_.x] = CellType::kFood; };

/**
 * @brief Draws the snake on the field.
 *
 * This function retrieves the body of the snake from the snake object and
 * iterates through each segment of the snake's body. For each segment, it
 * checks if the segment is within the bounds of the field. If the segment
 * is within bounds, it sets the corresponding cell in the field to the
 * snake cell type.
 *
 * @note If any segment of the snake is out of bounds, the drawing process
 *       stops immediately.
 */
void Field::DrawSnake() {
  std::deque<Point> body = snake_->GetBody();
  for (auto it = body.begin(); it != body.end(); ++it) {
    if (it->x < 0 || it->x >= MAX_X || it->y < 0 || it->y >= MAX_Y) {
      break;
    }
    field_[it->y][it->x] = CellType::kSnake;
  }
};

/**
 * @brief Checks if the snake is on the food.
 *
 * This function checks if the head of the snake is on the food. If the
 * coordinates of the snake's head match the coordinates of the food, the
 * function returns true. Otherwise, it returns false.
 *
 * @return true if the snake is on the food, false otherwise.
 */
bool Field::CheckSnakeOnFood() {
  Point head = snake_->GetSnakeHead();
  return (head.x == food_.x && head.y == food_.y);
}