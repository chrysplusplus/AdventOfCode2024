/* # Advent of Code: Day 5
 *
 * Input for this program is `5.txt`
 *
 * ## Part 1
 *
 * 1. Parse pages order rules and update lists
 * 2. Parse order rules into a lookup map with ascending and descending order
 *    rules
 * 3. For each update list, find the pairs of every page with every following page
 * 4. Lookup these pairs in the lookup map, if any yield a descending order, then
 *    the associated update is invalid
 * 5. Take the sum of the middle value of all valid updates
 *
 * Observations:
 * - Page updates seem to always contain an odd amount of pages
 *
 * ## Part 2
 *
 * 1. Parse page order rules and updates lists as in part 1
 * 2. For each update list, search for a pair of any two pages in order that
 *    yields a descending order in the lookup map
 * 3. Swap the values in the update list
 * 4. Reattempt a search for a desending pair of pages
 * 5. Take the middle value of all modified update lists
 *
 * ## Answer
 *
 * <details>
 *  <summary>Spoilers</summary>
 *  Part 1: 6505
 *
 *  Part 2: 6897
 * </details>
 */

#include "share.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <ranges>
#include <algorithm>

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

#ifndef PART_TWO
static constexpr const long TEST_ANSWER = 143;
#else // !PART_TWO
static constexpr const long TEST_ANSWER = 123;
#endif // !PART_TWO
#endif // USE_INPUT_FILE

enum OrderBy { ASCENDING, DESCENDING };

using PagesPair = std::pair<int, int>;

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

  auto graph_pages_pairs = std::map<PagesPair, OrderBy>{};
  for (int idx = 0; idx < ORDERS_LENGTH; idx++) {
    char skip_char{};
    int before, after;
    stm_input >> before;
    stm_input.get(skip_char); // skip `|`
    stm_input >> after;
    stmNext(stm_input);

    auto ascending_order = PagesPair{before, after};
    auto descending_order = PagesPair{after, before};
    graph_pages_pairs[ascending_order] = ASCENDING;
    graph_pages_pairs[descending_order] = DESCENDING;
  }

  {
    char skip_char{};
    stm_input.get(skip_char); // skip blank line
  }

  namespace views = std::views;
  namespace ranges = std::ranges;

  static constexpr const int PAGE_UPDATE_SEPARATOR = 0;
  auto list_separated_page_updates = std::vector<int>{};
  for (int idx = 0; idx < UPDATES_LENGTH; idx++) {
    std::string str_line{}, str_page_number{};
    stmNext(stm_input, &str_line);
    for (const auto& vw_pair : str_line | views::split(',')) {
      str_page_number.clear();
      ranges::copy(vw_pair, std::begin(str_page_number));
      list_separated_page_updates.push_back(std::stoi(str_page_number));
    }

    list_separated_page_updates.push_back(PAGE_UPDATE_SEPARATOR);
  }

#ifndef PART_TWO
  long answer = 0;
  auto vw_updates = list_separated_page_updates | views::split(PAGE_UPDATE_SEPARATOR);
  const auto sentinel_not_in_graph = std::end(graph_pages_pairs);

  for (const auto& update : vw_updates) {
    bool is_update_valid = !ranges::empty(update);
    for (auto it_preceding_page = std::begin(update); it_preceding_page != std::end(update); ++it_preceding_page) {
      if (!is_update_valid)
        break;

      for (const auto& following_page : ranges::subrange(it_preceding_page + 1, std::end(update))) {
        const auto maybe_page_pair = graph_pages_pairs.find({*it_preceding_page, following_page});
        if (maybe_page_pair == sentinel_not_in_graph)
          continue;

        if (maybe_page_pair->second == DESCENDING) {
          is_update_valid = false;
          break;
        }
      }
    }

    if (is_update_valid) {
      int middle_value = ranges::cdata(update)[ranges::size(update)/2];
      answer += middle_value;
    }
  }

#else // !PART_TWO
  long answer = 0;
  auto vw_updates = list_separated_page_updates | views::split(PAGE_UPDATE_SEPARATOR);
  const auto sentinel_not_in_graph = std::end(graph_pages_pairs);

  for (auto update : vw_updates) {
    bool is_modified = false;
    for (auto it_preceding_page = std::begin(update); it_preceding_page != std::end(update); ++it_preceding_page) {
      if (ranges::empty(update))
        break;

      for (auto it_following_page = it_preceding_page + 1; it_following_page != std::end(update); ++it_following_page) {
        const auto maybe_page_pair = graph_pages_pairs.find({*it_preceding_page, *it_following_page});
        if (maybe_page_pair == sentinel_not_in_graph)
          continue;

        if (maybe_page_pair->second == DESCENDING) {
          std::iter_swap(it_preceding_page, it_following_page);
          is_modified = true;
          // ideally has a failsafe to prevent infinite loop
          it_preceding_page = std::begin(update);
          it_following_page = it_preceding_page + 1;
          continue;
        }
      }
    }

    if (is_modified) {
      int middle_value = ranges::cdata(update)[ranges::size(update)/2];
      answer += middle_value;
    }
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

