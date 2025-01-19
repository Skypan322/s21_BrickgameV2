#include <ncurses.h>
#include <unistd.h>

#include "brick_game/common/utils.h"
#include "brick_game/snake/gamecontrol.h"
#include "gui/cli/cli.h"

s21::UserInput convertKeyToInput(char ch) {
  switch (ch) {
    case 'a':
      return s21::UserInput::kLeft;
    case 'd':
      return s21::UserInput::kRight;
    case 'w':
      return s21::UserInput::kUp;
    case 's':
      return s21::UserInput::kDown;
    case 'p':
      return s21::UserInput::kPause;
    case 'q':
      return s21::UserInput::kQuit;
    default:
      return s21::UserInput::kNone;
  }
}

void game_loop(WINDOW *win, s21::GameControl &snake) {
  using std::vector;

  while (snake.GetGameState() != s21::GameControl::GameState::kEnd) {
    char ch = wgetch(win);
    snake.FiniteStateAutomaton(convertKeyToInput(ch));
    int field[20][10];
    vector<vector<s21::Field::CellType>> vector_field = snake.GetField();
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 10; j++) {
        field[i][j] = static_cast<int>(vector_field[i][j]);
      }
    }
    print_field(win, field);
    wrefresh(win);
  }
  if (snake.GetGameState() == s21::GameControl::GameState::kEnd) {
    print_gameover(win, snake.GetScore(), snake.GetHighScore());
  }
  // print_field(win, field);
  wrefresh(win);
}

int main() {
  initscr();
  noecho();
  cbreak();
  WINDOW *win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_Y, WINDOW_X);
  nodelay(win, TRUE);
  curs_set(0);
  refresh();

  box(win, 0, 0);
  long highscore = read_highscore("highscore_snake.txt");
  print_start_screen(win, highscore);

  int level = -1;
  while (1) {
    level = getch() - '0';
    if (level >= 0 && level <= 9) {
      wclear(win);
      break;
    }
  }

  wrefresh(win);
  s21::GameControl snake;
  snake.StartGame();

  game_loop(win, snake);
  write_highscore("highscore_snake.txt", snake.GetHighScore());
  getch();
  endwin();
  return 0;
}