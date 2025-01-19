#ifndef CLI_H
#define CLI_H

#include <ncurses.h>  // ncurses library for CLI
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../brick_game/common/utils.h"
#include "../../brick_game/tetris/figures.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define WINDOW_WIDTH 60
#define WINDOW_HEIGHT 22
#define WINDOW_X 0
#define WINDOW_Y 0
#define RECTANGLE_CHAR '#'
#define BORDER_VCHAR '|'
#define BORDER_HCHAR '-'
#define ANGLE_CHAR '@'
#define SCORE_HEIGHT 3
#define SCORE_Y 1
#define LEVEL_Y (2 * SCORE_Y + 2 * SCORE_HEIGHT + 1)
#define LEVEL_HEIGHT 3
#define BRICK_IN_FIGURE 4
#define NUMBER_OF_COORDINATES 2

/**
 * Prints a rectangle on the specified window.
 *
 * @param win The window to print on.
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 */
void print_rectangle(WINDOW *win, int x, int y, int width, int height);

/**
 * Clears a rectangle on the specified window.
 *
 * @param win The window to clear on.
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 */
void clear_rectangle(WINDOW *win, int x, int y, int width, int height);

/**
 * Prints the game field on the specified window.
 *
 * @param win The window to print on.
 * @param field The game field represented as a 2D array of integers.
 */
void print_field(WINDOW *win, const int field[FIELD_HEIGHT][FIELD_WIDTH]);

/**
 * Prints the score on the specified window.
 *
 * @param win The window to print on.
 * @param score The current score.
 */
void print_score(WINDOW *win, int score);

/**
 * Prints the number of lines cleared on the specified window.
 *
 * @param win The window to print on.
 * @param lines The number of lines cleared.
 */
void print_lines(WINDOW *win, int lines);

/**
 * Prints the current level on the specified window.
 *
 * @param win The window to print on.
 * @param level The current level.
 */
void print_level(WINDOW *win, int level);

/**
 * Prints the game interface on the specified window.
 *
 * @param win The window to print on.
 * @param score The current score.
 * @param lines The number of lines cleared.
 * @param level The current level.
 * @param next The next figure to be displayed.
 */
void print_interface(WINDOW *win, int score, int lines, int level,
                     Figure *next);

/**
 * Prints the game over screen on the specified window.
 *
 * @param win The window to print on.
 * @param score The final score.
 * @param highscore The highest score achieved.
 */
void print_gameover(WINDOW *win, int score, int highscore);

/**
 * Prints the next figure on the specified window.
 *
 * @param win The window to print on.
 * @param next The next figure to be displayed.
 */
void print_next_figure(WINDOW *win, Figure *next);

/**
 * Initializes the ncurses window and returns it.
 *
 * @return The initialized ncurses window.
 */
WINDOW init_window();

/**
 * Closes the ncurses window.
 */
void close_window();

/**
 * Prints the start screen on the specified window.
 *
 * @param win The window to print on.
 * @param highscore The highest score achieved.
 */
void print_start_screen(WINDOW *win, int highscore);

#endif  // CLI_H