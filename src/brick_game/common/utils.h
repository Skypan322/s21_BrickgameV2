#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

/**
 * Counts the number of digits in a given number.
 *
 * @param number The number to count the digits of.
 * @return The number of digits in the given number.
 */
int count_digits(int number);

/**
 * Calculates the time difference between two timeval structures.
 *
 * @param start The starting timeval structure.
 * @param end The ending timeval structure.
 * @return The time difference in milliseconds.
 */
long time_diff(struct timeval start, struct timeval end);

/**
 * Writes the highscore to a file.
 *
 * @param highscore The highscore to be written.
 */
void write_highscore(const char *highscore_file, long highscore);

/**
 * Reads the highscore from a file.
 *
 * @return The highscore read from the file.
 */
long read_highscore(const char *highscore_file);

#ifdef __cplusplus
}
#endif
#endif  // UTILS_H