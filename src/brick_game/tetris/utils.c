#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

const char *HIGHSCORE_FILE = "highscore.txt";

int count_digits(int number) {
    return number == 0 ? 1 : (int)log10(number) + 1;
}

long time_diff(struct timeval start,
               struct timeval end) {  // returns the difference between two
    // timeval structs in milliseconds
    return (end.tv_sec - start.tv_sec) * 1000 +
           (end.tv_usec - start.tv_usec) / 1000.0 + 0.5;
}

void write_highscore(long highscore) {
    FILE *file = fopen(HIGHSCORE_FILE, "w");
    fprintf(file, "%ld", highscore);
    fclose(file);
}

long read_highscore() {
    FILE *file = fopen(HIGHSCORE_FILE, "r");
    if (file == NULL) {
        write_highscore(0);
        return 0;
    }
    long highscore;
    fscanf(file, "%ld", &highscore);
    fclose(file);
    return highscore;
}

#ifdef __cplusplus
}
#endif