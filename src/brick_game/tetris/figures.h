#ifndef FIGURES_H
#define FIGURES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define FIGURES_COUNT 7

typedef enum Gamestate {
    START,
    SPAWN,
    PAUSE,
    MOVING,
    // ROTATING,
    ATTACHING,
    GAMEOVER
} Gamestate;

typedef enum RotateDirection { LEFT, RIGHT } RotateDirection;

typedef enum UserAction {
    ROTATE_LEFT,
    ROTATE_RIGHT,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_DOWN,
    MOVE_HARD_DOWN,
    PAUSE_ACTION,
    EXIT_ACTION,
    NONE
} UserAction;

typedef enum FigureType { O, I, S, Z, L, J, T } FigureType;

typedef struct Brick {
    int x;
    int y;
} Brick;

typedef struct Figure {
    int x;  // x coordinate of the left top corner
    int y;  // y coordinate of the left top corner
    int rotation;
    Brick rotation_center;
    FigureType type;
    Brick bricks[4];
} Figure;

typedef struct Tetris {
    int field[20][10];
    Figure *figure;
    Figure *next;
    Gamestate state;
    struct timeval last_fall;
    long highscore;
    long fall_interval;
    int score;
    int level;
    int lines;  // number of lines cleared
} Tetris;

/**
 * Initializes a new Figure with default values.
 *
 * @return The initialized Figure.
 */
Figure init_figure();

/**
 * Initializes a new Tetris game with the specified level and highscore.
 *
 * @param tetris The Tetris game instance to initialize.
 * @param level The starting level of the game.
 * @param highscore The highscore to set for the game.
 */
void tetris_init(Tetris *tetris, int level, long highscore);

/**
 * Moves the current Figure based on the specified UserAction.
 *
 * @param action The UserAction to perform.
 * @param tetris The Tetris game instance.
 */
void move_figure(UserAction action, Tetris *tetris);

/**
 * Moves the current Figure down by one row.
 *
 * @param tetris The Tetris game instance.
 */
void move_down(Tetris *tetris);

/**
 * Rotates the current Figure in the specified direction.
 *
 * @param direction The direction to rotate the Figure (LEFT or RIGHT).
 * @param tetris The Tetris game instance.
 */
void rotate_figure(RotateDirection direction, Tetris *tetris);

/**
 * Makes the current Figure fall down until it collides with another Figure or
 * the bottom of the field.
 *
 * @param tetris The Tetris game instance.
 */
void fall_figure(Tetris *tetris);

/**
 * Attaches the current Figure to the field, making it a permanent part of the
 * game.
 *
 * @param tetris The Tetris game instance.
 */
void attach_figure(Tetris *tetris);

/**
 * Clears any completed lines in the field and updates the score and level
 * accordingly.
 *
 * @param tetris The Tetris game instance.
 */
void clear_lines(Tetris *tetris);

/**
 * Places the current Figure on the field.
 *
 * @param tetris The Tetris game instance.
 */
void place_figure(Tetris *tetris);

/**
 * Removes the current Figure from the field.
 *
 * @param tetris The Tetris game instance.
 */
void remove_figure(Tetris *tetris);

/**
 * Randomizes the next Figure to be spawned in the game.
 *
 * @param tetris The Tetris game instance.
 */
void randomize_next_figure(Tetris *tetris);

/**
 * Frees the memory allocated for a Figure.
 *
 * @param figure The Figure to free.
 */
void free_figure(Figure *figure);

/**
 * Checks if the current Figure can be legally placed on the field.
 *
 * @param tetris The Tetris game instance.
 * @return True if the placement is legal, false otherwise.
 */
bool if_legal_placement(Tetris *tetris);

/**
 * Checks if a new Figure can be spawned in the game.
 *
 * @param tetris The Tetris game instance.
 * @return True if a new Figure can be spawned, false otherwise.
 */
bool can_spawn_figure(Tetris *tetris);

/**
 * Checks if the current Figure collides with any other Figures on the field.
 *
 * @param tetris The Tetris game instance.
 * @return True if there is a collision, false otherwise.
 */
bool check_collide(Tetris *tetris);

#ifdef __cplusplus
}
#endif

#endif  // FIGURES_H