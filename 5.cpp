/* # Advent of Code: Day 5
 *
 * Input for this program is `5.txt`
 *
 * ## Part 1
 *
 * 1. Parse pages order rules and update lists
 * 2. Parse order rules into a lookup map
 *
 * ## Part 2
 *
 * ## Answer
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
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

#ifdef USE_INPUT_FILE
static constexpr const char * INPUT_FILE = "5.txt";
static constexpr const int ORDERS_LENGTH = 1176;
static constexpr const int UPDATES_LENGTH = 223;

#else // USE_INPUT_FILE
static constexpr const char * TEST_INPUT = R"(47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13

75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47)";

static constexpr const int ORDERS_LENGTH = 21;
static constexpr const int UPDATES_LENGTH = 6;

static constexpr const long TEST_ANSWER = 143;
#endif // USE_INPUT_FILE

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

  auto orderings_graph = std::map<int, std::vector<int>>{};
  for (int idx = 0; idx < ORDERS_LENGTH; idx++) {
    int before, after;
    stm_input >> before;
    stmNext(stm_input, nullptr, '|');
    stm_input >> after;
    stmNext(stm_input);

    orderings_graph[before].push_back(after);
  }

  stmNext(stm_input); // consume blank line separator

  auto updates = std::vector<std::vector<int>>{};
  for (int idx = 0; idx < UPDATES_LENGTH; idx++) {
    std::string str_line{};
    stmNext(stm_input, &str_line);
    std::istringstream stm_line{str_line};

    auto& vec_update = updates.emplace_back();
    while (stm_line) {
      int page;
      stm_line >> page;
      stmNext(stm_line, nullptr, ',');
      vec_update.push_back(page);
    }
  }

  namespace ranges = std::ranges;
  namespace views = std::views;

  ranges::for_each(orderings_graph | views::values, ranges::sort);

  long answer = 0;
  for (const auto& update: updates) {
    for (auto it_before = std::begin(update); it_before != std::end(update); it_before++) {
      for (const auto& after: ranges::subrange(it_before, std::end(update))) {
        auto it_ordering = orderings_graph.find(*it_before);
        if (it_ordering == std::end(orderings_graph))
          continue;

        auto it_ascending = ranges(*it_ordering, after);
        // doesn't work
        // because I need to check if the ordering is disallowed, but I don't have the graph set up to do that
      }
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

