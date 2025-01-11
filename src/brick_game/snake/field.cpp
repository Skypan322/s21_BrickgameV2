#include "field.h"

using namespace s21;

Field::Field() : snake_(std::make_unique<Snake>()) {
    field_.resize(MAX_Y);
    for (int i = 0; i < MAX_Y; ++i) {
        field_[i].resize(MAX_X);
    }
    SpawnFood();
}

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

void Field::EraseFood() { field_[food_.y][food_.x] = CellType::kEmpty; };

void Field::EraseSnake() {
    for (auto it = snake_->GetBody().begin(); it != snake_->GetBody().end();
         ++it) {
        field_[it->y][it->x] = CellType::kEmpty;
    }
};

void Field::DrawFood() { field_[food_.y][food_.x] = CellType::kFood; };
void Field::DrawSnake() {
    for (auto it = snake_->GetBody().begin(); it != snake_->GetBody().end();
         ++it) {
        field_[it->y][it->x] = CellType::kSnake;
    }
};