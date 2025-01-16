#include "figures.h"

#ifdef __cplusplus
extern "C" {
#endif

const Brick figures_init_bricks[FIGURES_COUNT][4] = {
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}},  // O
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}},  // I
    {{0, 0}, {1, 0}, {1, 1}, {2, 1}},  // S
    {{1, 0}, {2, 0}, {0, 1}, {1, 1}},  // Z
    {{0, 0}, {0, 1}, {0, 2}, {1, 2}},  // L
    {{1, 0}, {1, 1}, {1, 2}, {0, 2}},  // J
    {{0, 0}, {1, 0}, {2, 0}, {1, 1}},  // T
};

const Brick figures_init_rotation_centers[FIGURES_COUNT] = {
    {0, 0},  // O
    {0, 1},  // I
    {0, 0},  // S
    {2, 1},  // Z
    {0, 1},  // L
    {0, 1},  // J
    {1, 0},  // T
};

/**
 * Initializes a figure with the given width and height.
 *
 * @param width The width of the figure.
 * @param height The height of the figure.
 * @return A pointer to the initialized figure.
 */
Figure init_figure(FigureType type) {
    Figure figure;
    figure.rotation_center = figures_init_rotation_centers[type];
    figure.x = 4;
    figure.y = 0;
    figure.rotation = 0;
    figure.type = type;
    for (int i = 0; i < 4; i++) {
        figure.bricks[i] = figures_init_bricks[type][i];
    }
    return figure;
}

bool can_spawn_figure(Tetris *tetris) {
    Figure *figure = tetris->figure;
    bool ans = true;
    for (int i = 0; i < 4; i++) {
        int x = figure->bricks[i].x +
                figure->x;  // actual x coordinate of the brick
        int y = figure->bricks[i].y +
                figure->y;  // actual y coordinate of the brick
        if (tetris->field[y][x] == 1) {
            ans = false;
            break;
        }
    }
    return ans;
}

bool if_legal_placement(Tetris *tetris) {
    if (tetris->figure == NULL) {
        return false;
    }
    Figure *figure = tetris->figure;
    bool ans = true;
    for (int i = 0; i < 4; i++) {
        int x = figure->bricks[i].x + figure->x;
        int y = figure->bricks[i].y + figure->y;
        if (x < 0 || x > 9 || y < 0 || y > 19 || tetris->field[y][x] == 1) {
            ans = false;
            break;
        }
    }
    return ans;
}

bool check_collide(Tetris *tetris) {
    Figure *figure = tetris->figure;
    if (figure == NULL) {
        return false;
    }
    bool ans = false;
    for (int i = 0; i < 4; i++) {
        int x = figure->bricks[i].x + figure->x;
        int y = figure->bricks[i].y + figure->y;
        if (y == 19 || tetris->field[y + 1][x] == 1) {
            ans = true;
            break;
        }
    }
    return ans;
}

void move_figure(UserAction action, Tetris *tetris) {
    Figure *figure = tetris->figure;
    switch (action) {
        case MOVE_DOWN:
            figure->y++;
            if (!if_legal_placement(tetris)) {
                tetris->state = ATTACHING;
                figure->y--;
                //   } else {
                // for (int i = 0; i < 4; i++) {
                //   int x = figure->bricks[i].x + figure->x;
                //   int y = figure->bricks[i].y + figure->y;
                //   if (y == 19 || tetris->field[y + 1][x] == 1) {
                // tetris->state = ATTACHING;
                //   }
                // }
            }
            break;
        case MOVE_HARD_DOWN:
            remove_figure(tetris);
            move_down(tetris);
            place_figure(tetris);
            tetris->state = ATTACHING;
            break;
        case MOVE_LEFT:
            figure->x--;
            if (!if_legal_placement(tetris)) {
                figure->x++;
            }
            break;
        case MOVE_RIGHT:
            figure->x++;
            if (!if_legal_placement(tetris)) {
                figure->x--;
            }
            break;
        case ROTATE_LEFT:
            rotate_figure(LEFT, tetris);
            if (!if_legal_placement(tetris)) {
                rotate_figure(RIGHT, tetris);
            }
            break;
        case ROTATE_RIGHT:
            rotate_figure(RIGHT, tetris);
            if (!if_legal_placement(tetris)) {
                rotate_figure(LEFT, tetris);
            }
            break;
        case PAUSE_ACTION:
            tetris->state = PAUSE;
            break;
        case EXIT_ACTION:
            tetris->state = GAMEOVER;
            break;
        case NONE:
            break;
    }
    //   if (check_collide(tetris)) {
    // tetris->state = ATTACHING;
    //   }
    return;
}

void place_figure(Tetris *tetris) {
    Figure *figure = tetris->figure;
    for (int i = 0; i < 4; i++) {
        int x = figure->bricks[i].x + figure->x;
        int y = figure->bricks[i].y + figure->y;
        tetris->field[y][x] = 1;
    }
    return;
}

void remove_figure(Tetris *tetris) {
    Figure *figure = tetris->figure;
    for (int i = 0; i < 4; i++) {
        int x = figure->bricks[i].x + figure->x;
        int y = figure->bricks[i].y + figure->y;
        tetris->field[y][x] = 0;
    }
    return;
}

void move_down(Tetris *tetris) {
    Figure *figure = tetris->figure;
    while (1) {
        figure->y++;
        for (int i = 0; i < 4; i++) {
            int x = figure->bricks[i].x + figure->x;
            int y = figure->bricks[i].y + figure->y;
            if (y == 19 || tetris->field[y + 1][x] == 1) {
                return;
            }
        }
    }
}

void fall_figure(Tetris *tetris) {
    move_down(tetris);
    place_figure(tetris);
    tetris->figure = NULL;
    return;
}

void rotate_figure(RotateDirection direction, Tetris *tetris) {
    Figure *figure = tetris->figure;
    if (figure->type != O) {
        int sign = direction == LEFT ? -1 : 1;
        for (int i = 0; i < 4; i++) {
            int x = figure->bricks[i].x - figure->rotation_center.x;
            int y = figure->bricks[i].y - figure->rotation_center.y;
            figure->bricks[i].x = sign * y + figure->rotation_center.x;
            figure->bricks[i].y = -sign * x + figure->rotation_center.y;
        }
        figure->rotation = (figure->rotation + 4 + sign) % 4;
    }
    return;
}

void free_figure(Figure *figure) {
    if (figure != NULL) {
        free(figure);
        figure = NULL;
    }
}

void randomize_next_figure(Tetris *tetris) {
    tetris->next = malloc(sizeof(Figure));
    tetris->next->type = rand() % FIGURES_COUNT;
    tetris->next->rotation_center =
        figures_init_rotation_centers[tetris->next->type];
    tetris->next->x = 4;
    tetris->next->y = 0;
    tetris->next->rotation = 0;
    for (int i = 0; i < 4; i++) {
        tetris->next->bricks[i] = figures_init_bricks[tetris->next->type][i];
    }
    return;
}

void tetris_init(Tetris *tetris, int level, long highscore) {
    tetris->state = START;
    tetris->score = 0;
    tetris->level = level;
    tetris->lines = 0;
    srand(time(NULL));
    gettimeofday(&tetris->last_fall, NULL);
    tetris->highscore = highscore;
    tetris->fall_interval = 2000 / (level + 1);  //
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            tetris->field[i][j] = 0;
        }
    }
    tetris->figure = NULL;
    randomize_next_figure(tetris);
}

void attach_figure(Tetris *tetris) {
    Figure *figure = tetris->figure;
    for (int i = 0; i < 4; i++) {
        int x = figure->bricks[i].x + figure->x;
        int y = figure->bricks[i].y + figure->y;
        tetris->field[y][x] = 1;
    }
    free_figure(tetris->figure);
    tetris->figure = NULL;
    return;
}

void clear_lines(Tetris *tetris) {
    int lines = 0;
    for (int i = 0; i < 20; i++) {
        bool full = true;
        for (int j = 0; j < 10; j++) {
            if (tetris->field[i][j] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            lines++;
            for (int j = i; j > 0; j--) {
                for (int k = 0; k < 10; k++) {
                    tetris->field[j][k] = tetris->field[j - 1][k];
                }
            }
        }
    }
    tetris->lines += lines;
    tetris->score += lines * lines * 100;
    return;
}

#ifdef __cplusplus
}
#endif