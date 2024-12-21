/* # Advent of Code: Day 4
 *
 * Input for this program is `4.txt`
 *
 * ## Part 1
 *
 * 1. Read the input into a grid of characters
 * 2. For every instance of `X` in the grid, search in all 8 directions for `M`
 * 3. If `M` is found, continue in that direction and check for `A` and `S`
 * 4. Count the number of successful searches
 *
 * Observations:
 * - The input grid is a square
 *
 * ## Part 2
 *
 * 1. Read the input into a grid of characters
 * 2. For every instance of `A` in the grid, set a diagonal count of 0
 * 3. For each diagonal, check for `M`
 * 4. If `M` is found, check for `S` in the opposite direction
 * 5. If `S` is found, increment the diagonal count for the associated `A`
 * 6. Count the number of `A` with a diagonal count of 2
 *
 * Observations:
 * - The orientation of the X is unimportant, it just needs to read `MAS` on both
 *   diagonals, forwards or backwards
 *
 * ## Answer
 *
 * <details>
 *  <summary>Spoilers</summary>
 *  Part 1: 2575
 *
 *  Part 2: 2041
 * </details>
 */

#include "share.h"

#include <fstream>
#include <iostream>
#include <utility>
#include <ranges>
#include <vector>
#include <sstream>

#ifdef USE_INPUT_FILE
static constexpr const char * INPUT_FILE = "4.txt";
static constexpr const size_t INPUT_LENGTH = 140;

#else // USE_INPUT_FILE
static constexpr const char * TEST_INPUT = R"(MMMSXXMASM
MSAMXMSMSA
AMXSXMAAMM
MSAMASMSMX
XMASAMXAMM
XXAMMXXAMA
SMSMSASXSS
SAXAMASAAA
MAMMMXMMMM
MXMXAXMASX)";

static constexpr const size_t INPUT_LENGTH = 10;

#ifndef PART_TWO
static constexpr const long TEST_ANSWER = 18;
#else // !PART_TWO
static constexpr const long TEST_ANSWER = 9;
#endif // !PART_TWO
#endif // USE_INPUT_FILE

enum Direction {
#ifndef PART_TWO
  NORTH = 0, NORTH_EAST, EAST, SOUTH_EAST, SOUTH, SOUTH_WEST, WEST, NORTH_WEST
#else // !PART_TWO
  NORTH = 0, EAST, SOUTH, WEST,
  NORTH_EAST, SOUTH_EAST, SOUTH_WEST, NORTH_WEST
#endif // !PART_TWO
};

static constexpr const char EMPTY_CHAR = '\0';

struct WordsearchCoords
{
  size_t line, col;
  char ch = EMPTY_CHAR; // EMPTY_CHAR is error
};

using Wordsearch = std::vector<std::string, std::allocator<std::string>>;

WordsearchCoords wordsearchCoordsNextInDirection(WordsearchCoords wc, Direction d, const Wordsearch& wordsearch)
{
  WordsearchCoords next_coords{wc.line, wc.col, EMPTY_CHAR};
  switch (d) {
    case NORTH:
      if (wc.line != 0) {
        --(next_coords.line);
        next_coords.ch = wordsearch.at(wc.line - 1)[wc.col];
      }
      break;
    case NORTH_EAST:
      if (wc.line != 0 && wc.col != INPUT_LENGTH - 1) {
        --(next_coords.line);
        ++(next_coords.col);
        next_coords.ch = wordsearch.at(wc.line - 1)[wc.col + 1];
      }
      break;
    case EAST:
      if (wc.col != INPUT_LENGTH - 1) {
        ++(next_coords.col);
        next_coords.ch = wordsearch.at(wc.line)[wc.col + 1];
      }
      break;
    case SOUTH_EAST:
      if (wc.line != INPUT_LENGTH - 1 && wc.col != INPUT_LENGTH - 1) {
        ++(next_coords.line);
        ++(next_coords.col);
        next_coords.ch = wordsearch.at(wc.line + 1)[wc.col + 1];
      }
      break;
    case SOUTH:
      if (wc.line != INPUT_LENGTH -1) {
        ++(next_coords.line);
        next_coords.ch = wordsearch.at(wc.line + 1)[wc.col];
      }
      break;
    case SOUTH_WEST:
      if (wc.line != INPUT_LENGTH - 1 && wc.col != 0) {
        ++(next_coords.line);
        --(next_coords.col);
        next_coords.ch = wordsearch.at(wc.line + 1)[wc.col - 1];
      }
      break;
    case WEST:
      if (wc.col != 0) {
        --(next_coords.col);
        next_coords.ch = wordsearch.at(wc.line)[wc.col - 1];
      }
      break;
    case NORTH_WEST:
      if (wc.line != 0 && wc.col != 0) {
        --(next_coords.line);
        --(next_coords.col);
        next_coords.ch = wordsearch.at(wc.line - 1)[wc.col - 1];
      }
      break;
  }

  return next_coords;
}

Direction directionOpposite(Direction direction)
{
  Direction opposite;
  switch (direction) {
    case NORTH:
      opposite = SOUTH;
      break;
    case NORTH_EAST:
      opposite = SOUTH_WEST;
      break;
    case EAST:
      opposite = WEST;
      break;
    case SOUTH_EAST:
      opposite = NORTH_WEST;
      break;
    case SOUTH:
      opposite = NORTH;
      break;
    case SOUTH_WEST:
      opposite = NORTH_EAST;
      break;
    case WEST:
      opposite = EAST;
      break;
    case NORTH_WEST:
      opposite = SOUTH_EAST;
      break;
  }
  return opposite;
}

int main()
{
#ifdef USE_INPUT_FILE
  auto stm_input = std::ifstream(INPUT_FILE);
  if (!stm_input) {
    std::cout << "Unable to read input file\n";
  }

#else // USE_INPUT_FILE
  auto str_input_storage = std::string(TEST_INPUT);
  auto stm_input = std::istringstream(str_input_storage);
#endif // USE_INPUT_FILE

  auto wordsearch = Wordsearch{};
  for (size_t idx_line = 0; idx_line < INPUT_LENGTH; ++idx_line) {
    std::string& str_line = wordsearch.emplace_back();
    stmNext(stm_input, &str_line);
  }

  namespace view = std::views;

#ifndef PART_TWO
  auto x_coords = std::vector<WordsearchCoords>{};
  for (const auto& [idx_line, str_line]: view::enumerate(wordsearch)) {
    for (const auto& [idx_col, ch]: view::enumerate(str_line)) {
      if (ch == 'X') x_coords.emplace_back(idx_line, idx_col, ch);
    }
  }

  long answer = 0;
  for (const auto& coords: x_coords) {
    for (int direction = NORTH; direction <= NORTH_WEST; ++direction) {
      auto next_coord =  wordsearchCoordsNextInDirection(coords, (Direction)direction, wordsearch);
      if (next_coord.ch != 'M')
        continue;

      next_coord = wordsearchCoordsNextInDirection(next_coord, (Direction)direction, wordsearch);
      if (next_coord.ch != 'A')
        continue;

      next_coord = wordsearchCoordsNextInDirection(next_coord, (Direction)direction, wordsearch);
      if (next_coord.ch == 'S')
        ++answer;
    }
  }

#else // !PART_TWO
  auto a_coords = std::vector<WordsearchCoords>{};
  for (const auto& [idx_line, str_line]: view::enumerate(wordsearch)) {
    for (const auto& [idx_col, ch]: view::enumerate(str_line)) {
      if (ch == 'A') a_coords.emplace_back(idx_line, idx_col, ch);
    }
  }

  long answer = 0;
  for (const auto& coords: a_coords) {
    int mas_count = 0; // number of MAS occurences
    for (int diagonal = NORTH_EAST; diagonal <= NORTH_WEST; ++diagonal) {
      auto next_coord = wordsearchCoordsNextInDirection(coords, (Direction)diagonal, wordsearch);
      if (next_coord.ch != 'M')
        continue;

      next_coord = wordsearchCoordsNextInDirection(coords, directionOpposite((Direction)diagonal), wordsearch);
      if (next_coord.ch == 'S')
        ++mas_count;
    }

    if (mas_count == 2)
      ++answer;
  }
#endif // !PART_TWO

  std::cout << "Answer: " << answer << "\n";

#ifndef USE_INPUT_FILE
  std::cout << (answer == TEST_ANSWER
      ? "Test succeeded\n"
      : "Test failed\n"
      );
#endif // !USE_INPUT_FILE
}

