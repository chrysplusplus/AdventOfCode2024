/* # Advent of Code: Day 9 (Incomplete)
 *
 * Input for this program is `9.txt`
 *
 * ## Part 1
 *
 * 1. Parse the input character stream into a contiguous memory
 * 2. Take a pointer to the first free space in the memory and another to the last
 *    non-free space in memory
 * 3. Swap them
 * 4. Increment first pointer to next free space and decrement second to the next
 *    non-free space
 * 5. Repeat 3-4 until the position of the first pointer in the memory is after after
 *    the second
 * 6. Take the sum of the products of every non free space value in the memory and
 *    its index in the memory
 *
 * ## Part 2
 *
 * 1. Parse the input disk into contiguous memory as above, also storing the
 *    distance to the next block
 * 2. For each non-free space block, starting from the end of memory, take its
 *    block size
 * 3. Find a pointer to a block of free space from the beginning of memory that
 *    has a block size greater than or equal to the block size from 2
 * 4. If no block is found, continue to next non-free block from the end at 2
 * 5. Swap the blocks
 * 6. Repeat 2-5 for all non-free space blocks
 * 7. Calculate the checksum as above
 *
 * ## Answers
 *
 * <details>
 *  <summary>Spoilers</summary>
 *  Part 1: 6421128769094
 *
 *  Part 2:
 * </details>
 */

#include "share.h"

#include <fstream>
#include <sstream>
#include <memory>
#include <utility>
#include <string>
#include <iterator>
#include <ranges>
#include <algorithm>

static constexpr const char * INPUT_FILE = "9.txt";
static constexpr size_t BIG_INPUT_LENGTH = 95186; // obtained with calculateInputLength

static const auto STR_TEST_INPUT = std::string{R"(2333133121414131402)"};
static constexpr size_t TEST_INPUT_LENGTH = 42;

static constexpr const uint64_t PART_ONE_ANSWER = 1928;
static constexpr const uint64_t PART_TWO_ANSWER = 2858;

static constexpr const uint64_t FREE_SPACE = UINT64_MAX;

template <size_t Buffer_Size>
struct FilesystemSizeHint {};

static constexpr FilesystemSizeHint<BIG_INPUT_LENGTH> big_filesystem{};
static constexpr FilesystemSizeHint<TEST_INPUT_LENGTH> test_filesystem{};

template <size_t Buffer_Size>
struct Filesystem
{
  static constexpr size_t buffer_size = Buffer_Size;

  uint64_t max_file_id;

  std::array<uint64_t, buffer_size> memory;
  std::array<uint64_t, buffer_size> space_until_next; // part 2 added to part 1 parsing
};

constexpr inline std::pair<bool, uint64_t> charToPossibleInt(char ch) noexcept
{
  uint64_t digit = (uint64_t)ch - (uint64_t)'0';
  return digit < 10 ? std::pair<bool, uint64_t>{true, digit} : std::pair<bool,
         uint64_t>{false, 0};
}

template <size_t Buffer_Size>
uint64_t calculateInputLength(std::basic_istream<char>& stm_input)
{
  uint64_t length = 0;
  for (char ch{}; stm_input.get(ch); ) {
    auto [is_digit_valid, digit] = charToPossibleInt(ch);
    if (is_digit_valid)
      length += digit;
  }

  return length;
}

template <size_t B_Sz>
Filesystem<B_Sz> parseDisk(std::basic_istream<char>& stm_input)
{
  using filesystem_t = Filesystem<B_Sz>;
  filesystem_t filesystem{};

  size_t char_count = 0, idx = 0;
  uint64_t file_id_count = 0;
  for (char ch{}; stm_input.get(ch); ) {
    auto [is_block_size_valid, block_size] = charToPossibleInt(ch);
    if (!is_block_size_valid)
      continue;

    uint64_t file_id = char_count % 2 ? FREE_SPACE : file_id_count;
    if (file_id != FREE_SPACE)
      file_id_count++;

    uint64_t space_until_next = block_size;
    for (size_t idx_block = 0; idx_block != block_size; ++idx_block) {
      filesystem.memory.at(idx + idx_block) = file_id;
      filesystem.space_until_next.at(idx + idx_block) = space_until_next;
      --space_until_next;
    }

    idx += block_size;
    ++char_count;
  }

  filesystem.max_file_id = file_id_count - 1;

  return filesystem;
}

template <size_t B_Sz>
constexpr void filesystemCompressMemory(Filesystem<B_Sz>& filesystem) noexcept
{
  auto it_front = std::begin(filesystem.memory);
  auto it_back = std::rbegin(filesystem.memory);
  while (std::distance(it_front, it_back.base()) - 1 > 0) {
    if (it_front == std::end(filesystem.memory)||it_back == std::rend(filesystem.memory))
      break;

    if (*it_front != FREE_SPACE) {
      ++it_front;
      continue;
    }

    if (*it_back == FREE_SPACE) {
      ++it_back;
      continue;
    }

    std::iter_swap(it_front, it_back);
    ++it_front;
    ++it_back;
  }
}

template <size_t B_Sz>
constexpr uint64_t filesystemChecksum(const Filesystem<B_Sz>& filesystem) noexcept
{
  uint64_t checksum = 0;
  auto it_mem_end = std::ranges::find(filesystem.memory, FREE_SPACE);
  for (auto [idx, file_id] :
      std::ranges::subrange(std::begin(filesystem.memory), it_mem_end) |
      std::views::enumerate)
  {
    checksum += idx * file_id;
  }

  return checksum;
}

template <size_t B_Sz>
uint64_t calculateAnswerPart1(std::basic_istream<char>& stm_input, FilesystemSizeHint<B_Sz>)
{
  auto filesystem = parseDisk<B_Sz>(stm_input);
  filesystemCompressMemory(filesystem);
  return filesystemChecksum(filesystem);
}

template <size_t B_Sz>
uint64_t calculateAnswerPart2(std::basic_istream<char>& stm_input, FilesystemSizeHint<B_Sz>)
{
  auto filesystem = parseDisk<B_Sz>(stm_input);

  auto file_id = filesystem.max_file_id;
  auto it_block = std::ranges::find(filesystem.memory, file_id);

  auto vw_filesystem_zip = std::views::zip(filesystem.memory, filesystem.space_until_next);

  auto it_free_space = std::ranges::find(vw_filesystem_zip, [file_id](auto tpl) {
      if (std::get<0>(tpl) != FREE_SPACE) return false;
      return std::get<1>(tpl) >= file_id;
      });

  if (it_free_space != std::end(vw_filesystem_zip)) {
    auto [f, s] = *it_free_space;
    std::cout << f << "\t" << s << "\n";
  }


  //auto it_free_space = std::ranges::find(filesystem.memory, FREE_SPACE);
  //auto sentinel = std::end(filesystem.memory);

  //while (it_block != sentinel && it_free_space != sentinel) {
  //  auto idx = std::distance(std::begin(filesystem.memory), it_block);
  //  auto block_size = filesystem.space_until_next[idx];

  //  break;
  //}

  return 0;
}

int main()
{
  std::cout << "Part 1\n======\n";

  {
    auto stm_test_input = std::istringstream{STR_TEST_INPUT};

    uint64_t answer = calculateAnswerPart1(stm_test_input, test_filesystem);

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

    uint64_t answer = calculateAnswerPart1(stm_file_input, big_filesystem);
    std::cout << "Final answer: " << answer << "\n";
  }

  std::cout << "Part 2\n======\n";

  {
    auto stm_test_input = std::istringstream{STR_TEST_INPUT};

    uint64_t answer = calculateAnswerPart2(stm_test_input, test_filesystem);

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

    uint64_t answer = calculateAnswerPart2(stm_file_input, big_filesystem);
    std::cout << "Final answer: " << answer << "\n";
  }

}

