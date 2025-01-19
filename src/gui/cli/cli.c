#include "cli.h"

void print_field(WINDOW *win, const int field[FIELD_HEIGHT][FIELD_WIDTH]) {
    // draw border of the field
    mvwaddch(win, 0, 0, ANGLE_CHAR);
    mvwaddch(win, 0, 2 * FIELD_WIDTH + 1, ANGLE_CHAR);
    for (int i = 1; i < FIELD_HEIGHT + 1; i++) {
        mvwaddch(win, i, 0, BORDER_VCHAR);
        mvwaddch(win, i, 2 * FIELD_WIDTH + 1, BORDER_VCHAR);
    }
    mvwaddch(win, FIELD_HEIGHT + 1, 0, ANGLE_CHAR);
    mvwaddch(win, FIELD_HEIGHT + 1, 2 * FIELD_WIDTH + 1, ANGLE_CHAR);
    for (int i = 1; i < 2 * FIELD_WIDTH + 1; i++) {
        mvwaddch(win, 0, i, BORDER_HCHAR);
        mvwaddch(win, FIELD_HEIGHT + 1, i, BORDER_HCHAR);
    }

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            if (field[i][j] == 1) {
                mvwaddch(win, i + 1, 2 * j + 1, RECTANGLE_CHAR);
            } else if (field[i][j] == 2) {
                mvwaddch(win, i + 1, 2 * j + 1, 'O');
            } else {
                mvwaddch(win, i + 1, 2 * j + 1, '.');
            }
        }
    }
    return;
}

void print_rectangle(WINDOW *win, int x, int y, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                mvwaddch(win, y + i, x + j, RECTANGLE_CHAR);
            }
        }
    }
    //   wrefresh(win);
    return;
}

void clear_rectangle(WINDOW *win, int x, int y, int width, int height) {
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            mvwaddch(win, y + i, x + j, ' ');
        }
    }
    //   wrefresh(win);
    return;
}

void print_score(WINDOW *win, int score) {
    int offset = WINDOW_WIDTH - count_digits(score) - 10;
    print_rectangle(win, offset, 1, count_digits(score) + 9,
                    3);  // 9 is the length of the string "Score: "
    mvwprintw(win, SCORE_Y + 1, offset + 1, "Score: %d", score);
    //   wrefresh(win);
    return;
}

void print_lines(WINDOW *win, int lines) {
    int offset = WINDOW_WIDTH - count_digits(lines) - 10;
    print_rectangle(win, offset, SCORE_Y + SCORE_HEIGHT + 1,
                    count_digits(lines) + 9, 3);
    mvwprintw(win, SCORE_Y + SCORE_HEIGHT + 2, offset + 1, "Lines: %d", lines);
    //   wrefresh(win);
    return;
}

void print_level(WINDOW *win, int level) {
    int offset = WINDOW_WIDTH - count_digits(level) - 10;
    print_rectangle(win, offset, LEVEL_Y, count_digits(level) + 9, 3);
    mvwprintw(win, 2 * SCORE_Y + 2 * SCORE_HEIGHT + 2, offset + 1, "Level: %d",
              level);
    //   wrefresh(win);
    return;
}

void print_next_figure(WINDOW *win, Figure *next) {
    int offset = WINDOW_WIDTH - 10;
    clear_rectangle(win, offset, LEVEL_Y + LEVEL_HEIGHT + 1, 10, 8);
    print_rectangle(win, offset, LEVEL_Y + LEVEL_HEIGHT + 1, 10, 8);
    for (int i = 0; i < 4; i++) {
        mvwaddch(win, LEVEL_Y + LEVEL_HEIGHT + 3 + next->bricks[i].y,
                 offset + 2 * next->bricks[i].x + 2, RECTANGLE_CHAR);
    }
    //   wrefresh(win);
    return;
}

void print_interface(WINDOW *win, int score, int lines, int level,
                     Figure *next) {
    print_score(win, score);
    print_lines(win, lines);
    print_level(win, level);
    print_next_figure(win, next);
    wrefresh(win);
    return;
}

void print_gameover(WINDOW *win, int score, int highscore) {
    wclear(win);
    box(win, 0, 0);
    int message_length = strlen("Highscore 1000000000");
    int message_count = 3;
    int start_x = WINDOW_WIDTH / 2 - message_length / 2 + 10;
    int start_y = WINDOW_HEIGHT / 2 - message_count;
    print_rectangle(win, start_x, start_y, message_length + 2,
                    2 * message_count + 2);
    mvwprintw(win, start_y + 1, start_x + 5, "Game Over");
    mvwprintw(win, start_y + 3, start_x + 1 + message_length / 4, "Score: %d",
              score);
    mvwprintw(win, start_y + 5, start_x + message_length / 4, "Highscore: %d",
              highscore);
    wrefresh(win);
    return;
}

void print_start_screen(WINDOW *win, int highscore) {
    int message_length = strlen("Press key 0-9  to start the level");
    int message_count = 5;
    int start_x = WINDOW_WIDTH / 2 - message_length / 2;
    int start_y = WINDOW_HEIGHT / 2 - message_count;
    print_rectangle(win, start_x, start_y, message_length + 2,
                    2 * message_count);
    mvwprintw(win, start_y + 1, start_x + 1,
              "Press key 0-9 to start the level");
    mvwprintw(win, start_y + 3, start_x + 1 + message_length / 4,
              "Highscore: %d", highscore);
    mvwprintw(win, start_y + 5, start_x + 1 + message_length / 4,
              "WASD to move the figure");
    mvwprintw(win, start_y + 6, start_x + 1 + message_length / 8,
              "K and L to rotate the figure");
    mvwprintw(win, start_y + 7, start_x + 1 + message_length / 4,
              "Press 'q' to quit");
    mvwprintw(win, start_y + 8, start_x + 1 + message_length / 4,
              "Press 'p' to pause");
    wrefresh(win);
}

void close_window() {
    endwin();
    return;
}