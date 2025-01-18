#pragma once
#include <deque>

namespace s21 {

constexpr int MAX_Y = 20;
constexpr int MAX_X = 10;

enum class Direction { kUp, kLeft, kDown, kRight };

struct Point {
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}
    Point() : x(0), y(0) {}
    ~Point() = default;
};

class Snake {
   public:
    Snake();
    ~Snake() = default;
    void Move(Direction direction);
    void Move();
    void Grow(Point food);
    void Turn(Direction direction);
    bool СheckCollision();
    bool CheckFood(Point food) const {
        return head_.x == food.x && head_.y == food.y;
    }
    std::deque<Point> GetBody() const { return body_; }
    Direction GetDirection() const { return direction_; }

   private:
    std::deque<Point> body_;
    Point head_;
    Point last_tail_;

    Direction direction_;
    const int max_x_ = MAX_X;
    const int max_y_ = MAX_Y;
};
}  // namespace s21