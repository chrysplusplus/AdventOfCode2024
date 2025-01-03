/* # Advent of Code: Day 8
 *
 * Input for this program is `8.txt`
 *
 * ## Part 1
 *
 * 1. Parse grid of grid spaces with antennas
 * 2. For each pair of spaces:
 *    - Check that the spaces are not the same space
 *    - Check that the second space has an antenna
 *    - Calculate the far space, which is twice the distance from the first
 *      space to the second space
 *    - If the far space has the same antenna frequency as the second space,
 *      mark the first space as having an antinode
 * 3. Take the number of spaces having antinodes as the asnwer
 *
 * ## Part 2
 *
 * 1. Parse grid of grid spaces with antenna
 * 2. For each pair of spaces:
 *    - Check that the spaces are not the same space
 *    - Check that the first space has an antenna
 *    - Check that the first and second space both have the same antenna
 * 3. For each successful pair, find the distance between them
 * 4. Simplify this distance to small integer step, similar to simplifying a
 *    fraction
 * 5. Mark every multiple of this step distance from the first space as having
 *    an antinode (this will mark the second space; since every pair is visited
 *    twice, the inverse will happen when the second space and first space are
 *    swapped)
 * 6. Take the number of spaces having antinodes as the answer
 *
 * ## Answers
 *
 * <details>
 *  <summary>Spoilers</summary>
 *  Part 1: 409
 *
 *  Part 2: 1308
 * </details>
 */

#include "share.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ranges>
#include <iostream>

static constexpr const char * INPUT_FILE = "8.txt";

static const auto STR_TEST_INPUT = std::string{R"(............
........0...
.....0......
.......0....
....0.......
......A.....
............
............
........A...
.........A..
............
............)"};

static constexpr const uint64_t PART_ONE_ANSWER = 14;
static constexpr const uint64_t PART_TWO_ANSWER = 34;

struct GridSize
{
  size_t width, height;

  constexpr auto operator<=>(const GridSize&) const = default;
};

static constexpr const GridSize TEST_GRID_SIZE = {12, 12};
static constexpr const GridSize BIG_GRID_SIZE = {50, 50};

struct Grid
{
  struct GridSpace
  {
    char antenna;
    bool has_antinode;
  };

  GridSize grid_size;
  std::vector<GridSpace> grid_spaces;
};

static constexpr const char NO_ANTENNA = '\0';

struct GridCoord
{
  GridSize grid_size;
  size_t x, y;
};

size_t gridCoordToIdx(GridCoord coord)
{
  return coord.y * coord.grid_size.width + coord.x;
}

GridCoord idxToGridCoord(size_t idx, GridSize grid_size)
{
  return {grid_size, idx % grid_size.width, idx / grid_size.width};
}

struct PossiblyInvalidGridCoord
{
  bool is_valid;
  GridCoord grid_coord;
};

PossiblyInvalidGridCoord gridCoordFarAntenna(GridCoord antinode, GridCoord check_space)
{
  auto [grid_size, antinode_x, antinode_y] = antinode;
  auto [check_grid_size, check_x, check_y] = check_space;
  auto [width, height] = grid_size;

  auto fn_far_coord = [](size_t coord_here, size_t coord_there, size_t max_val) {
    size_t far_coord = coord_here + 2 * (coord_there - coord_here);
    return far_coord < max_val ? far_coord : max_val;
  };

  if (grid_size != check_grid_size)
    return {false, {}};

  size_t far_x = fn_far_coord(antinode_x, check_x, width);
  if (far_x == width)
    return {false, {}};

  size_t far_y = fn_far_coord(antinode_y, check_y, height);
  if (far_y == height)
    return {false, {}};

  return {true, {grid_size, far_x, far_y}};
}

uint64_t abs(int64_t x)
{
  return x < 0 ? x * -1 : x;
}

uint64_t greatestCommonDivisor(uint64_t a, uint64_t b)
{
  uint64_t min = a < b ? a : b;
  uint64_t max = a > b ? a : b;
  uint64_t divisor = min;
  uint64_t remainder = max % min;

  while (remainder != 0) {
    uint64_t tmp = remainder;
    remainder = divisor % remainder;
    divisor = tmp;
  }

  return divisor;
}

struct GridCoordDistance
{
  int64_t x, y;
};

GridCoordDistance gridCoordSmallestDistance(GridCoord here, GridCoord there)
{
  int64_t there_x = (int64_t)there.x, there_y = (int64_t)there.y;
  int64_t here_x = (int64_t)here.x, here_y = (int64_t)here.y;
  int64_t x = there_x - here_x;
  int64_t y = there_y - here_y;
  int64_t gcd = greatestCommonDivisor(abs(x), abs(y));
  return {x / gcd, y / gcd};
}

PossiblyInvalidGridCoord gridCoordStepDistance(GridCoord start, GridCoordDistance step_distance, uint64_t steps)
{
  auto [grid_size, x, y] = start;
  x = x + steps * step_distance.x;
  if (x >= grid_size.width)
    return {false, {}};

  y = y + steps * step_distance.y;
  if (y >= grid_size.height)
    return {false, {}};

  return {true, {grid_size, x, y,}};
}

Grid parseGrid(std::basic_istream<char>& stm_input, GridSize size_hint)
{
  auto [width, height] = size_hint;
  auto grid = Grid{{width, height}, {width * height, Grid::GridSpace{NO_ANTENNA, false}}};

  size_t idx = 0;
  while (stm_input) {
    auto str_line = std::string{};
    stmNext(stm_input, &str_line);
    for (auto it = std::begin(str_line); it != std::end(str_line); ++it) {
      if (*it != '.') {
        grid.grid_spaces.at(idx).antenna = *it;
      }

      ++idx;
    }
  }

  return grid;
}

uint64_t calculateAnswerPart1(std::basic_istream<char>& stm_input, GridSize size_hint)
{
  auto grid = parseGrid(stm_input, size_hint);
  const auto& grid_size = grid.grid_size;

  for (auto [tpl_antinode, tpl_check] : std::views::cartesian_product(std::views::enumerate(grid.grid_spaces), std::views::enumerate(grid.grid_spaces))) {
    auto [idx_antinode, antinode_space] = tpl_antinode;
    auto [idx_check, check_space] = tpl_check;

    if (idx_antinode == idx_check)
      continue;

    if (check_space.antenna == NO_ANTENNA)
      continue;

    auto grid_coord_antinode = idxToGridCoord(idx_antinode, grid_size);
    auto grid_coord_check = idxToGridCoord(idx_check, grid_size);
    auto [is_valid, far_grid_coord] = gridCoordFarAntenna(grid_coord_antinode, grid_coord_check);
    if (!is_valid)
      continue;

    auto& this_grid_space = grid.grid_spaces.at(idx_antinode);
    const auto& far_grid_space = grid.grid_spaces.at(gridCoordToIdx(far_grid_coord));
    if (far_grid_space.antenna == check_space.antenna) {
      this_grid_space.has_antinode = true;
    }
  }

  uint64_t answer = 0;
  for (auto grid_space : grid.grid_spaces) {
    if (grid_space.has_antinode) {
      ++answer;
    }
  }

  return answer;
}

uint64_t calculateAnswerPart2(std::basic_istream<char>& stm_input, GridSize size_hint)
{
  auto grid = parseGrid(stm_input, size_hint);
  const auto& grid_size = grid.grid_size;

  for (auto [tpl_first, tpl_second] : std::views::cartesian_product(std::views::enumerate(grid.grid_spaces), std::views::enumerate(grid.grid_spaces))) {
    auto [idx_first, first_space] = tpl_first;
    auto [idx_second, second_space] = tpl_second;

    if (idx_first == idx_second)
      continue;

    if (first_space.antenna == NO_ANTENNA)
      continue;

    if (first_space.antenna != second_space.antenna)
      continue;

    auto grid_coord_first = idxToGridCoord(idx_first, grid_size);
    auto grid_coord_second = idxToGridCoord(idx_second, grid_size);
    auto distance = gridCoordSmallestDistance(grid_coord_first, grid_coord_second);

    int64_t step_count = 1;
    auto maybe_next_grid_coord = gridCoordStepDistance(grid_coord_first, distance, step_count);
    while (maybe_next_grid_coord.is_valid) {
      auto& next_grid_space = grid.grid_spaces.at(gridCoordToIdx(maybe_next_grid_coord.grid_coord));
      next_grid_space.has_antinode = true;
      ++step_count;
      maybe_next_grid_coord = gridCoordStepDistance(grid_coord_first, distance, step_count);
    }
  }

  uint64_t answer = 0;
  for (const auto& grid_space : grid.grid_spaces) {
    if (grid_space.has_antinode) {
      ++answer;
    }
  }

  return answer;
}

int main()
{
  std::cout << "Part 1\n======\n";

  {
    auto stm_test_input = std::istringstream{STR_TEST_INPUT};

    uint64_t answer = calculateAnswerPart1(stm_test_input, TEST_GRID_SIZE);

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

    uint64_t answer = calculateAnswerPart1(stm_file_input, BIG_GRID_SIZE);
    std::cout << "Final answer: " << answer << "\n";
  }

  std::cout << "Part 2\n======\n";

  {
    auto stm_test_input = std::istringstream{STR_TEST_INPUT};

    uint64_t answer = calculateAnswerPart2(stm_test_input, TEST_GRID_SIZE);

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

    uint64_t answer = calculateAnswerPart2(stm_file_input, BIG_GRID_SIZE);
    std::cout << "Final answer: " << answer << "\n";
  }
}

