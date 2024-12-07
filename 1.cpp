/* # Advent of Code Day 1
 *
 * Input data is `1.txt`
 *
 * INPUT_FILE is passed by the compiler
 *
 * ## Part 1
 *
 * 1. Split input into two lists for numbers on the and numbers on the right
 * 2. Sort the two lists
 * 3. Find the sum of the differences of the two lists at each index position
 *
 * Observation:
 * - Each line is a sequence of five numeric characters, three spaces, then
 *   another sequence of five numeric characters
 * - There are 1000 lines of input
 *
 * ## Part 2
 *
 * 1. Split input into two lists for numbers on the left and numbers of the right
 * 2. Sort the right lists
 * 3. Find the sum of each number in the left list multiplied by the number of
 *    times it appears in the right list
 *
 * ## Answers
 *
 * <details>
 * <summary>Spoilers</summary>
 * Part 1: 2742123
 *
 * Part 2: 21328497
 * </details>
 */

#define STR(x) #x
#define STR_LIT(x) STR(x)

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <tuple>
#include <vector>
#include <algorithm>

#ifdef PART_TWO
static constexpr int TEST_ANSWER = 31;
#else // PART_TWO
static constexpr int TEST_ANSWER = 11;
#endif // PART_TWO

static constexpr const char * TEST_INPUT = R"(3   4
4   3
2   5
1   3
3   9
3   3)";

using Stm = std::basic_istream<char>;

Stm& stmNext(
    Stm& stream,
    std::string *str = nullptr,
    char delim = '\n')
{
  if (str) str->erase();
  for (char c{}; stream.get(c); )
  {
    if (c == delim) break;
    if (str) *(str) += c;
  }
  return stream;
}

using LinePair = std::tuple<long,long>;

LinePair stmNextPair(Stm& stream)
{
  long left, right;
  stream >> left >> right;
  stmNext(stream); // advance to next line
  return {left, right};
}

#ifdef PART_TWO
void progRunPartTwo()
{
#ifdef INPUT_FILE
  auto stm_input = std::ifstream(STR_LIT(INPUT_FILE));
  if (!stm_input) {
    std::cout << "Unable to read input file\n";
  }
  size_t count = 1000;

#else // INPUT_FILE
  auto str_storage_test_input = std::string(TEST_INPUT);
  auto stm_input = std::istringstream(str_storage_test_input);
  size_t count = 6;
#endif // INPUT_FILE

  auto left_side = std::vector<int>{};
  auto right_side = std::vector<int>{};
  for (size_t idx = 0; idx < count; ++idx) {
    auto [left, right] = stmNextPair(stm_input);
    left_side.push_back(left);
    right_side.push_back(right);
  }

  namespace rng = std::ranges;
  rng::sort(right_side);

  long answer = 0;
  for (long left: left_side) {
    size_t count = rng::count(right_side, left);
    answer += left * count;
  }

  std::cout << "Answer: " << answer << "\n";

#ifndef INPUT_FILE
  std::cout << (answer == TEST_ANSWER
      ? "Test succeeded\n"
      : "Test failed\n");
#endif // !INPUT_FILE
}

#else //PART_TWO
void progRunPartOne()
{
#ifdef INPUT_FILE
  auto stm_input = std::ifstream(STR_LIT(INPUT_FILE));
  if (!stm_input) {
    std::cout << "Unable to read input file\n";
  }
  size_t count = 1000;

#else // INPUT_FILE
  auto str_storage_test_input = std::string(TEST_INPUT);
  auto stm_input = std::istringstream(str_storage_test_input);
  size_t count = 6;
#endif // INPUT_FILE

  auto left_side = std::vector<int>{};
  auto right_side = std::vector<int>{};
  for (size_t idx = 0; idx < count; ++idx) {
    auto [left, right] = stmNextPair(stm_input);
    left_side.push_back(left);
    right_side.push_back(right);
  }

  namespace rng = std::ranges;
  rng::sort(left_side);
  rng::sort(right_side);

  long answer = 0;
  for (size_t idx = 0; idx < count; ++idx) {
    long left = left_side.at(idx), right = right_side.at(idx);
    answer += std::abs(left - right);
  }

  std::cout << "Answer: " << answer << "\n";

#ifndef INPUT_FILE
  std::cout << (answer == TEST_ANSWER
      ? "Test succeeded\n"
      : "Test failed\n");
#endif // !INPUT_FILE
}
#endif // PART_TWO

int main()
{
#ifdef PART_TWO
  progRunPartTwo();
#else // PART_TWO
  progRunPartOne();
#endif // PART_TWO
}

