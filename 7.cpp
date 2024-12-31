/* # Advent of Code: Day 7
 *
 * Input for this program is `7.txt`
 *
 * ## Part 1
 *
 * ## Answers
 *
 * <details>
 *  <summary>Spoilers</summary>
 *  Part 1:
 *
 *  Part 2:
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

#ifdef USE_INPUT_FILE
static constexpr const char * INPUT_FILE = "7.txt";

#else // USE_INPUT_FILE
static constexpr const char * TEST_INPUT = R"(190: 10 19
3267: 81 40 27
83: 17 5
156: 15 6
7290: 6 8 6 15
161011: 16 10 13
192: 17 8 14
21037: 9 7 18 13
292: 11 6 16 20)";

#endif // USE_INPUT_FILE

struct Equation
{
  long result;
  std::vector<long> operands;
};

Equation parseEquationFromString(std::string_view sv_equation)
{
  namespace views = std::views;
  namespace ranges = std::ranges;

  auto [sv_result, sv_operands] = svSplitFirst(sv_equation, ':');
  auto vw_operands = sv_operands | views::split(' ') | views::drop(1); // first split empty

  auto result = Equation{std::get<long>(svFormatToInt(sv_result)), {}};
  ranges::transform(vw_operands, std::back_inserter(result.operands), [](auto vw_operand) {
      return std::get<long>(svFormatToInt(std::string_view{vw_operand}));
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

std::vector<long> equationResultsAddOperand(const std::vector<long>& equation_results, long operand)
{
  if (equation_results.empty()) {
    return { operand };
  }

  auto new_result = std::vector<long>{};
  for (const auto& value : equation_results) {
    new_result.push_back(value + operand);
    new_result.push_back(value * operand);
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

int main()
{
#ifdef USE_INPUT_FILE
  auto stm_input = std::ifstream(INPUT_FILE);
  if (!stm_input) {
    std::cout << "Unable to read input file\n";
  }
#else // USE_INPUT_FILE
  auto str_input_storage = std::string{TEST_INPUT};
  auto stm_input = std::istringstream{str_input_storage};
#endif // USE_INPUT_FILE

  auto equations = parseEquations(stm_input);

  long answer = 0;
  for (const auto& equation: equations) {
    //std::cout << "Operands: ";
    //vecPrint(equation.operands);
    //std::cout << "\n";

    auto equation_results = std::vector<long>{};
    for (const auto& operand: equation.operands) {
      equation_results = std::move(equationResultsAddOperand(equation_results, operand));
    }

    //std::cout << "Results: ";
    //vecPrint(equation_results);
    //std::cout << "\n";

    auto it_result = std::ranges::find(equation_results, equation.result);
    if (it_result != std::end(equation_results)) {
      //std::cout << equation.result << " is a valid result\n\n";
      answer += equation.result;
    }
    //else {
    //  std::cout << equation.result << " is not a valid result\n\n";
    //}
  }

  std::cout << "Final answer: " << answer << "\n";
}

