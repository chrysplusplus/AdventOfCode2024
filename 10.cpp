/* # Advent of Code: Day 10
 *
 * Input for this program is `10.txt`
 *
 * Some code provided by Pablo
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
#include <iostream>
#include <string>
#include <utility>
#include <array>
#include <ranges>

static constexpr const char * INPUT_FILE = "10.txt";

static constexpr size_t BIG_MAP_SIZE = 45;

static const auto STR_TEST_INPUT = std::string{R"(89010123
78121874
87430965
96549874
45678903
32019012
01329801
10456732)"};

static constexpr const size_t TEST_INPUT_SIZE = 8;

static constexpr const uint64_t PART_ONE_ANSWER = 36;

template <size_t Map_Row_Size>
struct MapSizeInfo
{
  static constexpr const size_t row_size = Map_Row_Size;
  static constexpr const size_t col_size = row_size;
  static constexpr const size_t buffer_size = row_size * row_size;
};

static constexpr const MapSizeInfo<BIG_MAP_SIZE> big_map_size_info{};
static constexpr const MapSizeInfo<TEST_INPUT_SIZE> test_map_size_info{};

template <typename T>
struct Map { };

template <size_t Map_Sz>
struct Map<MapSizeInfo<Map_Sz>>
{
  using map_size_info_t = MapSizeInfo<Map_Sz>;

  std::array<unsigned short, map_size_info_t::buffer_size> data;
};

template <typename T = void>
struct Vector2
{
  int64_t x, y;
};

template <size_t Map_Sz>
struct Vector2<MapSizeInfo<Map_Sz>> : Vector2<void>
{
  using map_size_info_t = MapSizeInfo<Map_Sz>;
};

enum Direction
{
  NORTH = 0, EAST, SOUTH, WEST
};

using OffsetVector2 = Vector2<void>;

constexpr inline std::pair<bool, unsigned short> MaybeCharToUShort(char c) noexcept
{
  unsigned short digit = (unsigned short)c - (unsigned short)'0';
  return digit < 10
    ? std::pair<bool, unsigned short>{true, digit}
    : std::pair<bool, unsigned short>{false, 0};
}

constexpr inline OffsetVector2 DirectionToOffsetVector2(Direction d) noexcept
{
  OffsetVector2 vec{};
  switch (d) {
    case NORTH:
      vec.y = -1;
      break;
    case EAST:
      vec.x = 1;
      break;
    case SOUTH:
      vec.y = 1;
      break;
    case WEST:
      vec.x = -1;
      break;
  }
  return vec;
}

template <size_t M_sz>
constexpr inline std::pair<bool, Vector2<MapSizeInfo<M_sz>>> CalcBoundedOffset(Vector2<MapSizeInfo<M_sz>> position, OffsetVector2 offset) noexcept
{
  int x = position.x + offset.x, y = position.y + offset.y;
  if (x < 0 || (unsigned)x > decltype(position)::map_size_info_t::row_size)
    return {false, {0, 0}};
  if (y < 0 || (unsigned)y > decltype(position)::map_size_info_t::col_size)
    return {false, {0, 0}};
  return {true, {x, y}};
}

template <size_t M_sz>
Map<MapSizeInfo<M_sz>> ReadMap(std::basic_istream<char>& stm_input)
{
  using map_t = Map<MapSizeInfo<M_sz>>;
  map_t map{};

  auto it = std::begin(map.data);
  while (stm_input) {
    char c{};
    stm_input.get(c);

    if (c != '\n') {
      auto [maybe_digit, digit] = MaybeCharToUShort(c);
      if (!maybe_digit) continue;
      *it = digit;
      ++it;
    }
  }

  return map;
}

template <size_t M_sz>
void PrintMap(const Map<MapSizeInfo<M_sz>>& map)
{
  using map_size_info_t = MapSizeInfo<M_sz>;

  for (auto [idx, height] : std::views::enumerate(map.data)) {
    if (idx != 0 && idx % map_size_info_t::row_size == 0)
      std::cout << "\n";

    std::cout << height;
  }
}

using MapLocation = unsigned short *;

template <size_t M_sz>
constexpr Vector2<MapSizeInfo<M_sz>> MapLocationToPosition(Map<MapSizeInfo<M_sz>>& map, MapLocation location) noexcept
{
  using map_size_info_t = MapSizeInfo<M_sz>;
  size_t idx = std::distance(std::begin(map.data), location);
  return Vector2<MapSizeInfo<M_sz>>{int64_t(idx % map_size_info_t::row_size), int64_t(idx / map_size_info_t::row_size)};
}

template <size_t M_sz>
constexpr MapLocation PositionToMapLocation(Map<MapSizeInfo<M_sz>>& map, Vector2<MapSizeInfo<M_sz>> position) noexcept
{
  using map_size_info_t = MapSizeInfo<M_sz>;
  size_t idx = position.y * map_size_info_t::row_size + position.x;
  return std::begin(map.data) + idx;
}

template <size_t M_sz>
std::vector<MapLocation> FindTrailheads(Map<MapSizeInfo<M_sz>>& map)
{
  std::vector<MapLocation> trailheads{};
  for (unsigned short& height : map.data)
    if (height == 0)
      trailheads.push_back(&height);
  return trailheads;
}

template <size_t M_sz>
uint64_t CalculateTrailheadScore(Map<MapSizeInfo<M_sz>>& map, MapLocation trailhead_location) noexcept
{
  auto trailhead_position = MapLocationToPosition(map, trailhead_location);

  uint64_t score = 0;
  std::vector<decltype(trailhead_position)> search_stack{};

  search_stack.push_back(trailhead_position);

  while (!search_stack.empty()) {
    auto position = search_stack.back();
    auto location = PositionToMapLocation(map, position);
    search_stack.pop_back();

    for (Direction d : std::array{NORTH, EAST, SOUTH, WEST}) {
      auto [maybe_next_pos, next_pos] = CalcBoundedOffset(position, DirectionToOffsetVector2(d));
      if (!maybe_next_pos) continue;

      auto next_loc = PositionToMapLocation(map, next_pos);
      if (*next_loc == *location + 1 && *next_loc == 9) {
        ++score;
        continue;
      }

      if (*next_loc == *location + 1)
        search_stack.push_back(next_pos);
    }
  }

  return score;
}

template <size_t M_sz>
uint64_t CalculateAnswerPart1(std::basic_istream<char>& stm_input, MapSizeInfo<M_sz>)
{
  auto map = ReadMap<M_sz>(stm_input);
  auto trailheads = FindTrailheads(map);

  uint64_t answer = 0;
  for (MapLocation trailhead_loc : trailheads)
    answer += CalculateTrailheadScore(map, trailhead_loc);

  return answer;
}

void sayHelloToChrys();

int main()
{
  sayHelloToChrys();

  std::cout << "Part 1\n======\n";

  {
    auto stm_test_input = std::istringstream{STR_TEST_INPUT};

    uint64_t answer = CalculateAnswerPart1(stm_test_input, test_map_size_info);

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

    uint64_t answer = CalculateAnswerPart1(stm_file_input, big_map_size_info);
    std::cout << "Final answer: " << answer << "\n";
  }
}

void sayHelloToChrys(){
  if (1+1==2) {
    std::cout << "Hello Chrys\n";
    return;
  }
}
