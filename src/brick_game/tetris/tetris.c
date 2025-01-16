#include "tetris.h"

#ifdef __cplusplus
extern "C" {
#endif

void handle_spawn(Tetris *tetris) {
    free_figure(tetris->figure);
    tetris->figure = tetris->next;
    if (can_spawn_figure(tetris) == false) {
        handle_gameover(tetris);
        return;
    }
    place_figure(tetris);
    randomize_next_figure(tetris);
    tetris->state = MOVING;
    return;
}

void handle_fall(Tetris *tetris) {
    // printf("Falling\n");
    remove_figure(tetris);
    move_figure(MOVE_DOWN, tetris);
    place_figure(tetris);
    gettimeofday(&tetris->last_fall, NULL);
}

void handle_action(Tetris *tetris, UserAction action) {
    remove_figure(tetris);
    move_figure(action, tetris);
    place_figure(tetris);
}

void handle_attach(Tetris *tetris) {
    attach_figure(tetris);
    clear_lines(tetris);
    handle_spawn(tetris);
}

void handle_gameover(Tetris *tetris) {
    tetris->state = GAMEOVER;
    return;
}

void game_tick(Tetris *tetris, UserAction action) {
    switch (tetris->state) {
        case START:
            tetris->state = SPAWN;
            break;
        case SPAWN:
            handle_spawn(tetris);
            tetris->state = MOVING;
            break;
        case MOVING:
            struct timeval now;
            gettimeofday(&now, NULL);
            handle_action(tetris, action);
            if (time_diff(tetris->last_fall, now) > tetris->fall_interval) {
                handle_fall(tetris);
            }
            break;
        case ATTACHING:
            handle_attach(tetris);
            break;
        case GAMEOVER:
            if (tetris->score > tetris->highscore) {
                tetris->highscore = tetris->score;
            }
            // print_gameover(win);
            return;
        case PAUSE:
            if (action == PAUSE_ACTION) tetris->state = MOVING;
            break;
    }
    if (tetris->score > tetris->highscore) {
        tetris->highscore = tetris->score;
    }
}

void tetris_free(Tetris *tetris) {
    free_figure(tetris->figure);
    free_figure(tetris->next);
}

#ifdef __cplusplus
}
#endif