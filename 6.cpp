/* # Advent of Code: Day 6
 *
 * Input for this program is `6.txt`
 *
 * ## Part 1
 *
 * 1. Parse input into a grid of tiles, which are either ground or obstruction,
 *    recording also the starting position and orientation of the guard
 * 2. While the guard is still within the grid and has not return to their
 *    starting position and orientation:
 *    - If the guard is facing an obstruct, turn right 90 degrees
 *    - Otherwise the guard moves in the direction they are facing
 * 3. Find the number of positions within the grid in the guard's path
 *
 * ## Part 2
 *
 * Observations:
 *
 * ## Answer
 *
 * <details>
 *  <summary>Spoilers</summary>
 *  Part 1: 4433
 *
 *  Part 2:
 * </details>
 */

#include "share.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <array>
#include <set>
#include <vector>

#ifdef USE_INPUT_FILE
static constexpr const char * INPUT_FILE = "6.txt";
static constexpr const int GRID_SIZE = 130;

#else // USE_INPUT_FILE
static constexpr const char * TEST_INPUT = R"(....#.....
.........#
..........
..#.......
.......#..
..........
.#..^.....
........#.
#.........
......#...)";

static constexpr const int GRID_SIZE = 10;

#ifndef PART_TWO
static constexpr const long TEST_ANSWER = 41;

#else // !PART_TWO
static constexpr const long TEST_ANSWER = 6;

#endif // !PART_TWO

#endif // USE_INPUT_FILE

enum Tile { GROUND = 0, OBSTRUCTION };

enum Direction { NORTH = 0, EAST, SOUTH, WEST };

Direction directionTurn(Direction d)
{
  Direction next_direction{};
  switch (d) {
    case NORTH:
      next_direction = EAST;
      break;
    case EAST:
      next_direction = SOUTH;
      break;
    case SOUTH:
      next_direction = WEST;
      break;
    case WEST:
      next_direction = NORTH;
      break;
  }
  return next_direction;
}

static constexpr size_t NOT_IN_GRID = GRID_SIZE;

struct Position {
  long x, y;

  constexpr auto operator<=>(const Position& other) const = default;
};

Position positionNextInDirection(Position pos, Direction d)
{
  Position next_pos{pos};
  switch (d) {
    case NORTH:
      (next_pos.y)--;
      break;
    case EAST:
      (next_pos.x)++;
      break;
    case SOUTH:
      (next_pos.y)++;
      break;
    case WEST:
      (next_pos.x)--;
      break;
  }

  if (next_pos.x < 0 || next_pos.x >= GRID_SIZE)
    next_pos.x = NOT_IN_GRID;

  if (next_pos.y < 0 || next_pos.y >= GRID_SIZE)
    next_pos.y = NOT_IN_GRID;

  return next_pos;
}

bool positionIsInGrid(Position pos)
{
  return pos.x != NOT_IN_GRID && pos.y != NOT_IN_GRID;
}

Position positionFromGridIndex(size_t idx)
{
  return { (long)idx % GRID_SIZE, (long)idx / GRID_SIZE };
}

size_t positionToGridIndex(Position pos)
{
  return pos.y * GRID_SIZE + pos.x;
}

struct Guard {
  Position position;
  Direction facing;
};

#ifdef PART_TWO
struct VisitedTileOrientation {
  bool is_visited = false;
  Direction orientation = NORTH;
};

bool guardDoesLoopWhenTurning(
    const std::array<Tile, GRID_SIZE * GRID_SIZE>& grid,
    const std::array<VisitedTileOrientation, GRID_SIZE * GRID_SIZE>& visited_grid,
    Guard guard
    )
{
  Guard theoretical_guard{guard};
  bool does_theoretical_guard_loop = false;
  while (positionIsInGrid(guard.position)) {
    const auto& visited_tile = visited_grid[positionToGridIndex(guard.position)];
    if (visited_tile.is_visited && visited_tile.orientation == guard.facing) {
      does_theoretical_guard_loop = true;
      break;
    }

    // Continue from here
  }
}
#endif

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

  std::array<Tile, GRID_SIZE * GRID_SIZE> grid{};
  Guard guard_start{};
  {
    auto it = std::begin(grid);
    for (char ch{}; stm_input.get(ch); ) {
      switch (ch) {
        case '#':
          *it = OBSTRUCTION;
          it++;
          break;
        case '^':
          guard_start = { positionFromGridIndex(std::distance(std::begin(grid), it)), NORTH };
          *it = GROUND;
          it++;
          break;
        case '\n':
          break;
        default:
          *it = GROUND;
          it++;
          break;
      }
    }
  }

#ifndef PART_TWO
  Guard guard{guard_start};
#endif // PART_TWO
  auto fn_is_guard_finished = [guard_start](const Guard& g) -> bool {
    if (g.position == guard_start.position && g.facing == guard_start.facing)
      return true;

    return !positionIsInGrid(g.position);
  };

#ifndef PART_TWO
  std::set<Position> visited_positions{guard.position};

#else // !PART_TWO
  std::array<VisitedTileOrientation, GRID_SIZE * GRID_SIZE> visited_grid{};
  std::vector<Guard> stack_guards{guard_start};
  long answer = 0;

#endif // !PART_TWO
  do {
#ifdef PART_TWO
    const auto& guard = stack_guards.back();
#endif // PART_TWO
    Position next_pos = positionNextInDirection(guard.position, guard.facing);

    bool is_next_tile_obstruction = positionIsInGrid(next_pos) ? grid[positionToGridIndex(next_pos)] == OBSTRUCTION : false;
    if (is_next_tile_obstruction) {
      guard.facing = directionTurn(guard.facing);
      continue;
    }

#ifndef PART_TWO
    visited_positions.insert(guard.position);
    guard.position = next_pos;

#else // !PART_TWO
    bool is_theoretical_guard = stack_guards.size() > 1;
    if (!is_theoretical_guard) {
      visited_grid[positionToGridIndex(guard.position)] = { true, guard.facing };
      stack_guards.emplace_back(guard.position, directionTurn(guard.facing));
      guard.position = next_pos;
      continue;
    }

    if (is_theoretical_guard && !positionIsInGrid(next_pos)) {
      stack_guards.pop_back();
      continue;
    }

    const auto& check_visited_tile = visited_grid[positionToGridIndex(next_pos)];
    if (is_theoretical_guard && check_visited_tile.is_visited && check_visited_tile.orientation == guard.facing) {
      ++answer;
      stack_guards.pop_back();
      continue;
    }

#endif // !PART_TWO
  } while (!fn_is_guard_finished(guard));

#ifndef PART_TWO
  long answer = visited_positions.size();
#endif // !PART_TWO

  std::cout << "Answer: " << answer << "\n";

#ifndef USE_INPUT_FILE
  std::cout << (answer == TEST_ANSWER
      ? "Test succeeded\n"
      : "Test failed\n"
      );
#endif // !USE_INPUT_FILE
}
