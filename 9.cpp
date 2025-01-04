/* # Advent of Code: Day 9
 *
 * Input for this program is `9.txt`
 *
 * ## Part 1
 *
 * 1. Parse the input character stream into a contiguous memory
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
#include <memory>
#include <utility>
#include <string>

static constexpr const char * INPUT_FILE = "9.txt";
static constexpr size_t BIG_INPUT_LENGTH = 95186;

static const auto STR_TEST_INPUT = std::string{R"(2333133121414131402)"};
static constexpr size_t TEST_INPUT_LENGTH = 42;

static constexpr const uint64_t PART_ONE_ANSWER = 1928;

static constexpr const uint64_t FREE_SPACE = UINT64_MAX;

template <size_t Buffer_Size>
struct FilesystemSizeHint {};

static constexpr FilesystemSizeHint<BIG_INPUT_LENGTH> big_filesystem{};
static constexpr FilesystemSizeHint<TEST_INPUT_LENGTH> test_filesystem{};

template <size_t Buffer_Size>
struct Filesystem
{
  static constexpr size_t buffer_size = Buffer_Size;

  std::array<uint64_t, buffer_size> memory;
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

    uint64_t file_id = char_count % 2 ? (file_id_count++) : FREE_SPACE;

    for (size_t idx_block = 0; idx_block != block_size; ++idx_block) {
      filesystem.memory.at(idx + idx_block) = file_id;
    }

    idx += block_size;
    ++char_count;
  }
}

template <size_t B_Sz>
uint64_t calculateAnswerPart1(std::basic_istream<char>& stm_input, FilesystemSizeHint<B_Sz>)
{
  return 0;
}

int main()
{
  std::cout << "Part 1\n======\n";

  //{
  //  auto stm_test_input = std::istringstream{STR_TEST_INPUT};

  //  uint64_t answer = calculateAnswerPart1(stm_test_input);

  //  std::cout << "Test result: " << answer << "\n";
  //  if (answer != PART_ONE_ANSWER) {
  //    std::cout << "Test was not successful\n";
  //    return 0;
  //  }
  //}

  {
    auto stm_file_input = std::ifstream{INPUT_FILE};
    if (!stm_file_input) {
      std::cout << "Unable to read input file\n";
      return -1;
    }

    uint64_t length = calculateInputLength(stm_file_input);
    std::cout << "Length: " << length << "\n";

    //uint64_t answer = calculateAnswerPart1(stm_file_input);
    //std::cout << "Final answer: " << answer << "\n";
  }

}

