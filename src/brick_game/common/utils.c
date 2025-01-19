#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Counts the number of digits in a given integer.
 *
 * This function calculates the number of digits in the provided integer.
 * If the number is 0, it returns 1 since 0 has one digit.
 * For other numbers, it uses the logarithm base 10 to determine the number of
 * digits.
 *
 * @param number The integer whose digits are to be counted.
 * @return The number of digits in the given integer.
 */
int count_digits(int number) {
  return number == 0 ? 1 : (int)log10(number) + 1;
}

/**
 * @brief Calculates the difference between two timeval structures in
 * milliseconds.
 *
 * This function takes two timeval structures, `start` and `end`, and computes
 * the difference between them in milliseconds. The result is rounded to the
 * nearest millisecond.
 *
 * @param start The starting timeval structure.
 * @param end The ending timeval structure.
 * @return The difference between the two timeval structures in milliseconds.
 */
long time_diff(struct timeval start,
               struct timeval end) {  // returns the difference between two
  // timeval structs in milliseconds
  return (end.tv_sec - start.tv_sec) * 1000 +
         (end.tv_usec - start.tv_usec) / 1000.0 + 0.5;
}

/**
 * @brief Writes the high score to a specified file.
 *
 * This function opens the specified file in write mode, writes the high score
 * to the file, and then closes the file.
 *
 * @param highscore_file The path to the file where the high score will be
 * written.
 * @param highscore The high score to be written to the file.
 */
void write_highscore(const char *highscore_file, long highscore) {
  FILE *file = fopen(highscore_file, "w");
  fprintf(file, "%ld", highscore);
  fclose(file);
}

/**
 * @brief Reads the high score from a specified file.
 *
 * This function attempts to open the specified high score file in read mode.
 * If the file does not exist or cannot be opened, it initializes the high score
 * by writing a score of 0 to the file and returns 0. If the file is
 * successfully opened, it reads the high score from the file, closes the file,
 * and returns the high score.
 *
 * @param highscore_file The path to the high score file.
 * @return The high score read from the file, or 0 if the file does not exist or
 * cannot be opened.
 */
long read_highscore(const char *highscore_file) {
  FILE *file = fopen(highscore_file, "r");
  if (file == NULL) {
    write_highscore(highscore_file, 0);
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