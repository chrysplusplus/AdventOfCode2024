/* # Advent of Code: Day 2
 *
 * Input for this program is `2.txt`
 *
 * ## Part 1
 *
 * 1. Read input into a list of reports, each containing 5 numbers
 * 2. Find the number of safe reports. A report is safe if both:
 *  - The numbers in sequence are all increasing or all decreasing
 *  - Any two adjacent numbers differ by at least 1 and at most 3
 *
 * Observations:
 * - Example data has reports of length 5, but actual input uses a varying
 *   length
 *
 * ## Part 2
 *
 * Method worked out on paper then implemented below.
 *
 * I'd like to assume that each report could only have one issue, but I wrote a
 * guard for this just in case.
 *
 * ## Answers
 *
 * <details>
 *  <summary>Spoilers</summary>
 *  Part 1: 282
 *  Part 2:
 * </details>
 */

#include "share.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <ranges>
#include <algorithm>

#ifdef USE_INPUT_FILE
static constexpr const char * INPUT_FILE = "2.txt";
static constexpr const size_t INPUT_LENGTH = 1000;

#else // USE_INPUT_FILE
static constexpr const char * TEST_INPUT = R"(7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9)";

static constexpr const size_t INPUT_LENGTH = 6;

#ifdef PART_TWO
static constexpr const long TEST_ANSWER = 4;
#else // PART_TWO
static constexpr const long TEST_ANSWER = 2;
#endif // PART_TWO

#endif // USE_INPUT_FILE

//using Report = std::array<long, 5>;
using Report = std::vector<long>;

enum ReportCondition {
  SAFE_INCREASING, SAFE_DECREASING, UNSAFE
};

struct ReportSummary {
  size_t count_increasing, count_decreasing, count_errors;
};

int main()
{
#ifdef USE_INPUT_FILE
  auto stm_input = std::ifstream(INPUT_FILE);
  if (!stm_input) {
    std::cout << "Unable to read input file\n";
  }

#else // USE_INPUT_FILE
  auto str_storage_test_input = std::string(TEST_INPUT);
  auto stm_input = std::istringstream(str_storage_test_input);
#endif // USE_INPUT_FILE

  auto reports = std::vector<Report>{};
  for (size_t idx = 0; idx < INPUT_LENGTH; ++idx) {
    auto line = std::string{};
    stmNext(stm_input, &line);
    auto stm_line = std::istringstream(line);

    auto& report = reports.emplace_back();
    long value;
    while (stm_line >> value)
      report.push_back(value);
  }

  namespace view = std::views;
  namespace rng = std::ranges;

  auto fn_pair_condition = [](auto&& r) -> ReportCondition {
    long difference = r[0] - r[1];
    return (
        difference < -3 ? UNSAFE :
        difference < 0 ? SAFE_INCREASING :
        difference == 0 ? UNSAFE :
        difference <= 3 ? SAFE_DECREASING :
        UNSAFE
        );
  };

#ifdef PART_TWO

  auto fn_pair_difference = [](auto&& r) -> long {
    return r[0] - r[1];
  };

  auto fn_fold_summary = [](ReportSummary summary, auto&& r) -> ReportSummary {
    long difference = r[0] - r[1];
    if (difference < -3)
      ++(summary.count_errors);
    else if (difference < 0)
      ++(summary.count_increasing);
    else if (difference == 0)
      ++(summary.count_errors);
    else if (difference <= 3)
      ++(summary.count_decreasing);
    else
      ++(summary.count_errors);
    return summary;
  };

  long answer = 0;
  for (auto& report: reports) {
    auto summary = rng::fold_left(report | view::slide(2), ReportSummary{0, 0, 0}, fn_fold_summary);
    bool is_decreasing = summary.count_decreasing > summary.count_increasing;

    int n_of_errors;
    if (is_decreasing)
      n_of_errors = summary.count_errors + summary.count_increasing;
    else
      n_of_errors = summary.count_errors + summary.count_decreasing;

    if (n_of_errors == 0) {
      ++answer;
      continue;
    }
    else if (n_of_errors > 1) {
      continue;
    }





    bool has_multiple_errors = false;
    size_t sentinel = report.size();
    size_t unsafe_idx = sentinel;
    for (auto [idx, condition]: report | view::slide(2) | view::transform(fn_pair_difference) | view::enumerate) {
      if (condition == UNSAFE && unsafe_idx != sentinel) {
        has_multiple_errors = true;
        break;
      }
      else if (condition == UNSAFE) {
        unsafe_idx = idx;
      }
    }

    if (has_multiple_errors) continue;
    if (unsafe_idx == sentinel) {
      ++answer;
      continue;
    }

}

#else // PART_TWO

  auto fn_fold_report_condition = [](ReportCondition a, ReportCondition b) -> ReportCondition {
    return a == b ? a : UNSAFE;
  };

  long answer = 0;
  for (auto &report: reports) {
    auto report_conditions = report | view::slide(2) | view::transform(fn_pair_condition);
    auto maybe_condition = rng::fold_left_first(report_conditions, fn_fold_report_condition);
    if (maybe_condition.value() != UNSAFE)
      ++answer;
  }

#endif // PART_TWO

  std::cout << "Answer: " << answer << "\n";

#ifndef USE_INPUT_FILE
  std::cout << (answer == TEST_ANSWER
      ? "Test succeeded\n"
      : "Test failed\n"
      );
#endif // !USE_INPUT_FILE
}

