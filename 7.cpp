/* # Advent of Code: Day 7
 *
 * Input for this program is `7.txt`
 *
 * ## Part 1
 *
 * 1. Parse equations
 * 2. Create the equations results by incrementally performing adding and
 *    multiplication on a branching set of numbers for each operand
 * 3. Search through the final equation results for the expected result
 * 4. If the expected result is found, this equation is valid
 * 5. Take the sum of all valid equations
 *
 * ## Part 2
 *
 * 1. Parse equations
 * 2. Create the equations results as above, only with another concatenation
 *    operator (explained below)
 * 3. Search for expected result; if found, equation is valid
 * 4. Take the sum of all valid equations
 *
 * Concatenation:
 * concat(10, 345) -> 10345
 *
 * I have implemented concatenation by iteratively dividing a temporary copy of
 * the second operand by 10 and multiplying the first operand by 10; then adding
 * the first and second operands. So above, 345 can be divided by 10 three times,
 * so the first operand is multiplied by 10 three times to become 10000, then
 * added to 345, yielding 10345.
 *
 * ## Answers
 *
 * <details>
 *  <summary>Spoilers</summary>
 *  Part 1: 1620690235709
 *
 *  Part 2: 145397611075341
 * </details>
 */

#include "share.h"

#include <fstream>
#include <sstream>
#include <ranges>
#include <charconv>
#include <string_view>
#include <utility>
#include <vector>
#include <algorithm>
#include <iterator>

static constexpr const char * INPUT_FILE = "7.txt";

static constexpr const char * TEST_INPUT = R"(190: 10 19
3267: 81 40 27
83: 17 5
156: 15 6
7290: 6 8 6 15
161011: 16 10 13
192: 17 8 14
21037: 9 7 18 13
292: 11 6 16 20)";

static constexpr const uint64_t PART_ONE_ANSWER = 3749;
static constexpr const uint64_t PART_TWO_ANSWER = 11387;

struct Equation
{
  uint64_t result;
  std::vector<uint64_t> operands;
};

Equation parseEquationFromString(std::string_view sv_equation)
{
  namespace views = std::views;
  namespace ranges = std::ranges;

  auto [sv_result, sv_operands] = svSplitFirst(sv_equation, ':');
  auto vw_operands = sv_operands | views::split(' ') | views::drop(1); // first split empty

  auto result = Equation{std::get<uint64_t>(svFormatToInt(sv_result)), {}};
  ranges::transform(vw_operands, std::back_inserter(result.operands), [](auto vw_operand) {
      return std::get<uint64_t>(svFormatToInt(std::string_view{vw_operand}));
      });

  return result;
}

std::vector<Equation> parseEquations(std::basic_istream<char>& stream)
{
  auto result = std::vector<Equation>{};
  auto str_line = std::string{};
  while (stream) {
    stmNext(stream, &str_line);
    auto equation = parseEquationFromString(std::string_view{str_line});
    result.push_back(std::move(equation));
  }
  return result;
}

std::vector<uint64_t> equationResultsAddOperand(const std::vector<uint64_t>& equation_results, uint64_t operand)
{
  if (equation_results.empty()) {
    return { operand };
  }

  auto new_result = std::vector<uint64_t>{};
  for (const auto& value : equation_results) {
    new_result.push_back(value + operand);
    new_result.push_back(value * operand);
  }

  return new_result;
}

uint64_t concat(uint64_t x, uint64_t y)
{
  uint64_t tmp = y;
  while (tmp > 0) {
    x *= 10;
    tmp /= 10;
  }
  return x + y;
}

std::vector<uint64_t> equationResultsAddOperandWithConcat(const std::vector<uint64_t>& equation_results, uint64_t operand)
{
  if (equation_results.empty()) {
    return { operand };
  }

  auto new_result = std::vector<uint64_t>{};
  for (const auto& value : equation_results) {
    new_result.push_back(value + operand);
    new_result.push_back(value * operand);
    new_result.push_back(concat(value, operand));
  }

  return new_result;
}

using namespace std::string_view_literals; // ""sv

template <typename T>
void vecPrint(const std::vector<T>& vec, std::string_view sep = ", "sv)
{
  for (auto it = std::begin(vec); it != std::end(vec); ++it) {
    std::cout << (*it);
    if (it + 1 != std::end(vec)) {
      std::cout << sep;
    }
  }
}

uint64_t calculateAnswerPart1(std::basic_istream<char>& stm_input)
{
  auto equations = parseEquations(stm_input);

  uint64_t answer = 0;
  for (const auto& equation: equations) {
    auto equation_results = std::vector<uint64_t>{};
    for (const auto& operand: equation.operands) {
      equation_results = std::move(equationResultsAddOperand(equation_results, operand));
    }

    auto it_result = std::ranges::find(equation_results, equation.result);
    if (it_result != std::end(equation_results)) {
      answer += equation.result;
    }
  }

  return answer;
}

uint64_t calculateAnswerPart2(std::basic_istream<char>& stm_input)
{
  auto equations = parseEquations(stm_input);

  uint64_t answer = 0;
  for (const auto& equation: equations) {
    auto equation_results = std::vector<uint64_t>{};
    for (const auto& operand: equation.operands) {
      equation_results = std::move(equationResultsAddOperandWithConcat(equation_results, operand));
    }

    auto it_result = std::ranges::find(equation_results, equation.result);
    if (it_result != std::end(equation_results)) {
      answer += equation.result;
    }
  }

  return answer;
}

int main()
{
  std::cout << "Part 1\n======\n";

  {
    auto str_test_input_storage = std::string{TEST_INPUT};
    auto stm_test_input = std::istringstream{str_test_input_storage};

    uint64_t answer = calculateAnswerPart1(stm_test_input);

    std::cout << "Test result: " << answer << "\n";
    if (answer != PART_ONE_ANSWER) {
      std::cout << "Test was not successful\n";
      return 0;
    }
  }

  {
    auto stm_file_input = std::ifstream{INPUT_FILE};
    if (!stm_file_input) {
      std::cout << "Unable to read input file\n";
      return -1;
    }

    uint64_t answer = calculateAnswerPart1(stm_file_input);

    std::cout << "Final answer: " << answer << "\n";
  }

  std::cout << "Part 2\n======\n";

  {
    auto str_test_input_storage = std::string{TEST_INPUT};
    auto stm_test_input = std::istringstream{str_test_input_storage};

    uint64_t answer = calculateAnswerPart2(stm_test_input);

    std::cout << "Test result: " << answer << "\n";
    if (answer != PART_TWO_ANSWER) {
      std::cout << "Test was not successful\n";
      return 0;
    }
  }

  {
    auto stm_file_input = std::ifstream{INPUT_FILE};
    if (!stm_file_input) {
      std::cout << "Unable to read input file\n";
      return -1;
    }

    uint64_t answer = calculateAnswerPart2(stm_file_input);

    std::cout << "Final answer: " << answer << "\n";
  }
}

