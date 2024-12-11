/* # Advent of Code: Day 2
 *
 * Input for this program is `3.txt`
 *
 * ## Part 1
 *
 * 1. Parse the input into a sequence of valid tokens `mul(XX,YY)` where XX and
 *    YY are an n-lenth sequence of numeric characters
 * 2. Take the sum of all the multiplications
 *
 * Observations:
 * - There are size lines of text in the program data file, but newlines will be
 *   treated like any other character
 *
 * ## Part 2
 *
 * 1. Same as above, handling extra tokens `do()` and `don't` which enable and
 *    disable multiplication respectively
 *
 * ## Answers
 *
 * <details>
 *  <summary>Spoilers</summary>
 *  Part 1: 174103751
 *  Part 2: 100411201
 * </details>
 */

#include "share.h"

#include <fstream>
#include <sstream>
#include <utility>
#include <string>
#include <string_view>

#ifdef USE_INPUT_FILE
static constexpr const char * INPUT_FILE = "3.txt";

#else // USE_INPUT_FILE
#ifdef PART_TWO
static constexpr const char * TEST_INPUT = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";
static constexpr const long TEST_ANSWER = 48;

#else // PART_TWO
static constexpr const char * TEST_INPUT = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
static constexpr const long TEST_ANSWER = 161;

#endif // PART_TWO
#endif // USE_INPUT_FILE

enum ParserState {
  START_TOKEN, FIRST_VALUE, SECOND_VALUE,
#ifdef PART_TWO
  ENABLE_TOKEN,
#endif //PART_TWO
};

int main()
{
#ifdef USE_INPUT_FILE
  std::string str_input{};
  {
    auto stm_file_input = std::ifstream(INPUT_FILE);
    if (!stm_file_input) {
      std::cout << "Unable to read input file\n";
    }

    for (char c{}; stm_file_input.get(c); ) {
      str_input.push_back(c);
    }
  }

#else // USE_INPUT_FILE
  auto str_input = std::string(TEST_INPUT);
#endif // USE_INPUT_FILE

  long answer = 0;

  using namespace std::literals::string_view_literals;
#ifdef PART_TWO
  auto sv_do_token    = "do()"sv;
  auto sv_dont_token  = "don't()"sv;
#endif // PART_TWO
  auto sv_start_token = "mul("sv;
  auto ch_delimiter = ',';
  auto ch_end_token  = ')';

  auto it_parser = str_input.begin();
  auto val1 = std::string{};
  auto val2 = std::string{};
  ParserState state = START_TOKEN;
  while (it_parser != str_input.end()) {
    switch (state) {
      case START_TOKEN:
        {
#ifdef PART_TWO
          auto vw_token = std::string_view(it_parser, it_parser + sv_dont_token.length());
#else // PART_TWO
          auto vw_token = std::string_view(it_parser, it_parser + sv_start_token.length());
#endif // PART_TWO
          if (vw_token.starts_with(sv_start_token)) {
            state = FIRST_VALUE;
            it_parser += sv_start_token.length();
          }
#ifdef PART_TWO
          else if (vw_token.starts_with(sv_dont_token)) {
            state = ENABLE_TOKEN;
            it_parser += sv_dont_token.length();
          }
#endif // PART_TWO
          else {
            ++it_parser;
          }
          break;
        }
      case FIRST_VALUE:
        if (std::isdigit(*it_parser)) {
          val1.push_back(*it_parser);
          ++it_parser;
        }
        else if (*it_parser == ch_delimiter) {
          state = SECOND_VALUE;
          ++it_parser;
        }
        else {
          val1.clear();
          state = START_TOKEN;
          ++it_parser;
        }
        break;

      case SECOND_VALUE:
        if (std::isdigit(*it_parser)) {
          val2.push_back(*it_parser);
          ++it_parser;
        }
        else if (*it_parser == ch_end_token) {
          answer += std::stol(val1) * std::stol(val2);
          val1.clear();
          val2.clear();
          state = START_TOKEN;
          ++it_parser;
        }
        else {
          val1.clear();
          val2.clear();
          state = START_TOKEN;
          ++it_parser;
        }
        break;

#ifdef PART_TWO
      case ENABLE_TOKEN:
        {
          auto sv_token = std::string_view(it_parser, it_parser + sv_do_token.length());
          if (sv_token.starts_with(sv_do_token)) {
            state = START_TOKEN;
            it_parser += sv_do_token.length();
          }
          else {
            ++it_parser;
          }
          break;
        }
#endif // PART_TWO
    }
  }

  std::cout << "Answer: " << answer << "\n";

#ifndef USE_INPUT_FILE
  std::cout << (answer == TEST_ANSWER
      ? "Test succeeded\n"
      : "Test failed\n"
      );
#endif // !USE_INPUT_FILE
}

