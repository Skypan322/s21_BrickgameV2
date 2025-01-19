#include <gtest/gtest.h>

#include "../brick_game/common/utils.h"
#include "../brick_game/snake/field.h"
#include "../brick_game/snake/gamecontrol.h"
#include "../brick_game/snake/snake.h"

TEST(GameControl, StartGame) {
    write_highscore("highscore_snake.txt", 0);
    s21::GameControl game_control;
    game_control.StartGame();
    EXPECT_EQ(game_control.GetGameState(),
              s21::GameControl::GameState::kTurning);
    EXPECT_EQ(game_control.GetScore(), 1);
    EXPECT_EQ(game_control.GetHighScore(), 0);
    EXPECT_EQ(game_control.GetSnakeBody().size(), 3);
}

TEST(GameControl, EndGame) {
    s21::GameControl game_control;
    game_control.EndGame();
    EXPECT_EQ(game_control.GetHighScore(), 0);
}

TEST(GameControl, MoveSnake_1) {
    s21::GameControl game_control;
    game_control.StartGame();
    game_control.MoveSnake(s21::Direction::kRight);
    EXPECT_EQ(game_control.GetSnakeBody().size(), 3);
    game_control.MoveSnake(s21::Direction::kRight);
    EXPECT_EQ(game_control.GetSnakeBody().size(), 3);
}

TEST(GameControl, MoveSnake_2) {
    s21::GameControl game_control;
    game_control.StartGame();
    auto old_body = game_control.GetSnakeBody();
    game_control.MoveSnake(s21::Direction::kRight);
    auto new_body = game_control.GetSnakeBody();
    EXPECT_EQ(new_body[0].x, old_body[0].x + 1);
    EXPECT_EQ(new_body[0].y, old_body[0].y);
}

TEST(GameControl, TurnSnake) {
    s21::GameControl game_control;
    game_control.StartGame();
    game_control.TurnSnake(s21::Direction::kRight);
    EXPECT_EQ(game_control.GetSnakeBody().size(), 3);
    game_control.TurnSnake(s21::Direction::kLeft);
    EXPECT_EQ(game_control.GetSnakeBody().size(), 3);
}

TEST(GameControl, GrowSnake) {
    s21::GameControl game_control;
    game_control.StartGame();
    game_control.GrowSnake();
    EXPECT_EQ(game_control.GetSnakeBody().size(), 4);
}

TEST(GameControl, FiniteStateAutomaton) {
    s21::GameControl game_control;
    game_control.StartGame();
    game_control.FiniteStateAutomaton(s21::UserInput::kRight);
    EXPECT_EQ(game_control.GetGameState(),
              s21::GameControl::GameState::kTurning);
    game_control.FiniteStateAutomaton(s21::UserInput::kPause);
    EXPECT_EQ(game_control.GetGameState(), s21::GameControl::GameState::kPause);
    game_control.FiniteStateAutomaton(s21::UserInput::kPause);
    EXPECT_EQ(game_control.GetGameState(),
              s21::GameControl::GameState::kTurning);
    game_control.FiniteStateAutomaton(s21::UserInput::kQuit);
    EXPECT_EQ(game_control.GetGameState(), s21::GameControl::GameState::kEnd);
}

TEST(GameControl, CheckRightInputDirection) {
    s21::GameControl game_control;
    EXPECT_TRUE(game_control.CheckRightInputDirection(s21::UserInput::kUp));
    EXPECT_TRUE(game_control.CheckRightInputDirection(s21::UserInput::kDown));
    EXPECT_TRUE(game_control.CheckRightInputDirection(s21::UserInput::kLeft));
    EXPECT_TRUE(game_control.CheckRightInputDirection(s21::UserInput::kRight));
    EXPECT_FALSE(game_control.CheckRightInputDirection(s21::UserInput::kPause));
    EXPECT_FALSE(game_control.CheckRightInputDirection(s21::UserInput::kQuit));
    EXPECT_FALSE(game_control.CheckRightInputDirection(s21::UserInput::kNone));
}

TEST(Snake, Move) {
    s21::Snake snake;
    snake.Move(s21::Direction::kRight);
    EXPECT_EQ(snake.GetBody().size(), 3);
    snake.Move(s21::Direction::kRight);
    EXPECT_EQ(snake.GetBody().size(), 3);
    snake.Move(s21::Direction::kRight);
    EXPECT_EQ(snake.GetBody().size(), 3);
}

TEST(Snake, Turn) {
    s21::Snake snake;
    snake.Turn(s21::Direction::kRight);
    EXPECT_EQ(snake.GetDirection(), s21::Direction::kRight);
    snake.Turn(s21::Direction::kLeft);
    EXPECT_EQ(snake.GetDirection(), s21::Direction::kRight);
    snake.Turn(s21::Direction::kUp);
    EXPECT_EQ(snake.GetDirection(), s21::Direction::kUp);
    snake.Turn(s21::Direction::kDown);
    EXPECT_EQ(snake.GetDirection(), s21::Direction::kUp);
}

TEST(Snake, GetBody) {
    s21::Snake snake;
    EXPECT_EQ(snake.GetBody().size(), 3);
    snake.Move(s21::Direction::kRight);
    EXPECT_EQ(snake.GetBody().size(), 3);
    snake.Grow();
    EXPECT_EQ(snake.GetBody().size(), 4);
    snake.Move(s21::Direction::kRight);
    snake.Grow();
    EXPECT_EQ(snake.GetBody().size(), 5);
}

TEST(Field, DrawFood) {
    s21::Field field;
    field.SpawnFood();
    field.DrawFood();
    bool ans = false;
    for (int i = 0; i < s21::MAX_Y; ++i) {
        for (int j = 0; j < s21::MAX_X; ++j) {
            if (field.GetField()[i][j] == s21::Field::CellType::kFood) {
                ans = true;
                break;
            }
        }
    }

    EXPECT_TRUE(ans);
}

TEST(Field, DrawFood_2) {
    s21::Field field;
    EXPECT_FALSE(field.CheckSnakeOnFood());
}

TEST(Field, DrawFood_3) {
    s21::Field field;
    field.SpawnFood();
    bool ans = false;
    for (int i = 0; i < s21::MAX_Y; ++i) {
        for (int j = 0; j < s21::MAX_X; ++j) {
            if (field.GetField()[i][j] == s21::Field::CellType::kFood) {
                ans = true;
                break;
            }
        }
    }

    EXPECT_TRUE(ans);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}