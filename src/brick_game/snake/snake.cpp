#include "snake.h"

#include <iostream>

using namespace s21;

Snake::Snake() : max_x_(MAX_X), max_y_(MAX_Y) {
    body_.push_back(Point(max_x_ / 2, max_y_ / 2));
    body_.push_back(Point(max_x_ / 2 - 1, max_y_ / 2));
    body_.push_back(Point(max_x_ / 2 - 2, max_y_ / 2));
    head_ = body_.front();
    direction_ = Direction::kUp;
};

bool Snake::СheckCollision() {
    if (head_.x < 0 || head_.x >= max_x_ || head_.y < 0 || head_.y >= max_y_) {
        return true;  // check of input, no breaking structural programming
    }

    bool ans = false;
    for (auto it = body_.begin() + 1; it != body_.end(); ++it) {
        if (head_.x == it->x && head_.y == it->y) {
            ans = true;
            break;
        }
    }
    return ans;
}

void Snake::Turn(Direction direction) {
    if (direction !=
        static_cast<Direction>((static_cast<int>(direction_) + 2) %
                               4)) {  // +2 % 4 is opposite direction
        direction_ = direction;
    }
}

void Snake::Move() {
    Point new_head = head_;
    if (direction_ == Direction::kUp) {
        new_head.y--;
    } else if (direction_ == Direction::kDown) {
        new_head.y++;
    } else if (direction_ == Direction::kLeft) {
        new_head.x--;
    } else if (direction_ == Direction::kRight) {
        new_head.x++;
    }

    body_.push_front(new_head);
    head_ = new_head;
    last_tail_ = body_.back();
    body_.pop_back();
}

void Snake::Move(Direction direction) {
    Turn(direction);
    Move();
}

void Snake::Grow(Point food) {
    std::cout << "Food:" << food.x << " " << food.y << std::endl;
    body_.push_back(last_tail_);
}