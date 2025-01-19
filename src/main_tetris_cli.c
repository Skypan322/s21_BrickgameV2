#include <unistd.h>

#include "brick_game/common/utils.h"
#include "brick_game/tetris/figures.h"
#include "brick_game/tetris/tetris.h"
#include "gui/cli/cli.h"

UserAction get_user_action(char ch) {
  // int ch = getch();
  switch (ch) {
    case 'k':
      return ROTATE_LEFT;
    case 'w':
      return MOVE_HARD_DOWN;
    case 'l':
      return ROTATE_RIGHT;
    case 's':
      return MOVE_DOWN;
    case 'a':
      return MOVE_LEFT;
    case 'd':
      return MOVE_RIGHT;
    case 'p':
      return PAUSE_ACTION;
    case 'q':
      return EXIT_ACTION;
    default:
      return NONE;
  }
}

void game_loop(WINDOW *win, Tetris *tetris) {
  while (tetris->state != GAMEOVER) {
    char ch = wgetch(win);
    game_tick(tetris, get_user_action(ch));
    print_interface(win, tetris->score, tetris->lines, tetris->level,
                    tetris->next);
    print_next_figure(win, tetris->next);
    print_field(win, tetris->field);
    wrefresh(win);
  }
  if (tetris->state == GAMEOVER) {
    tetris->highscore =
        tetris->score > tetris->highscore ? tetris->score : tetris->highscore;
    print_gameover(win, tetris->score, tetris->highscore);
  }
  print_field(win, tetris->field);
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
  long highscore = read_highscore(TETRIS_HIGHSCORE_FILE);
  print_start_screen(win, highscore);
  wrefresh(win);
  int level = -1;
  while (1) {
    level = getch() - '0';
    if (level >= 0 && level <= 9) {
      wclear(win);
      break;
    }
  }
  Tetris tetris;
  tetris_init(&tetris, level, highscore);
  tetris.highscore = read_highscore(TETRIS_HIGHSCORE_FILE);
  game_loop(win, &tetris);
  write_highscore(TETRIS_HIGHSCORE_FILE, tetris.highscore);
  getch();
  endwin();
  return 0;
}