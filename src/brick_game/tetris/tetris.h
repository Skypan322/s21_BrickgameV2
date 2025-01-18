#ifndef TETRIS_H
#define TETRIS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#include "../common/utils.h"
#include "figures.h"

/**
 * Handles the spawning of a new tetromino in the Tetris game.
 *
 * @param tetris The Tetris game instance.
 */
void handle_spawn(Tetris *tetris);

/**
 * Handles the falling of the current tetromino in the Tetris game.
 *
 * @param tetris The Tetris game instance.
 */
void handle_fall(Tetris *tetris);

/**
 * Handles the user action in the Tetris game.
 *
 * @param tetris The Tetris game instance.
 * @param action The user action to handle.
 */
void handle_action(Tetris *tetris, UserAction action);

/**
 * Handles the attachment of the current tetromino to the game board in the
 * Tetris game.
 *
 * @param tetris The Tetris game instance.
 */
void handle_attach(Tetris *tetris);

/**
 * Handles the game over condition in the Tetris game.
 *
 * @param tetris The Tetris game instance.
 */
void handle_gameover(Tetris *tetris);

/**
 * Performs a game tick in the Tetris game, updating the game state based on the
 * user action.
 *
 * @param tetris The Tetris game instance.
 * @param action The user action to handle.
 */
void game_tick(Tetris *tetris, UserAction action);

/**
 * Frees the memory allocated for the Tetris game instance.
 *
 * @param tetris The Tetris game instance to free.
 */
void tetris_free(Tetris *tetris);

#ifdef __cplusplus
}
#endif

#endif  // TETRIS_H