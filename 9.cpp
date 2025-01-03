/* # Advent of Code: Day 9 (Incomplete)
 *
 * Input for this program is `9.txt`
 *
 * ## Part 1
 *
 * 1. Parse the input into a linked list of the memory blocks
 *  - For non-free space, use the block_size specified
 *  - For free space, create the specified number of 1-blocks
 * 2. Find the first and last increments in the memory. An increment is an
 *    offset into a memory block
 * 3. Increment the first increment until it is pointing to free space
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

static const auto STR_TEST_INPUT = std::string{R"(2333133121414131402)"};

static constexpr const uint64_t PART_ONE_ANSWER = 1928;

static constexpr const uint64_t FREE_SPACE = UINT64_MAX;

struct Filesystem
{
  struct MemoryBlock
  {
    size_t block_size;
    uint64_t file_id;
    std::shared_ptr<MemoryBlock> p_next_block, p_prev_block;
  };

  std::shared_ptr<MemoryBlock> p_head;
};

struct MemoryBlockIncrement
{
  std::shared_ptr<Filesystem::MemoryBlock> p_block;
  size_t increment;

  void LinkNext()
  {
    do {
      p_block = p_block->p_next_block;
    } while (p_block && p_block->block_size != 0);
    increment = 0;
  }

  void LinkPrev()
  {
    do {
      p_block = p_block->p_prev_block;
    } while (p_block && p_block->block_size != 0);
    increment = p_block ? p_block->block_size - 1 : 0;
  }

  MemoryBlockIncrement& operator++()
  {
    if (!p_block) return *this;
    if (increment + 1 == p_block->block_size) {
      LinkNext();
      return *this;
    }

    ++increment;
    return *this;
  }

  MemoryBlockIncrement operator++(int)
  {
    if (!p_block) return {nullptr, 0};
    if (increment + 1 == p_block->block_size) {
      auto result = MemoryBlockIncrement{*this};
      LinkNext();
      return result;
    }

    auto result = MemoryBlockIncrement{*this};
    ++increment;
    return result;
  }

  MemoryBlockIncrement& operator--()
  {
    if (!p_block) return *this;
    if (increment == 0) {
      LinkPrev();
      return *this;
    }

    --increment;
    return *this;
  }

  MemoryBlockIncrement operator--(int)
  {
    if (!p_block) return *this;
    if (increment == 0) {
      auto result = MemoryBlockIncrement{*this};
      LinkPrev();
      return result;
    }

    auto result = MemoryBlockIncrement{*this};
    --increment;
    return result;
  }
};

constexpr inline std::pair<bool, size_t> charToPossibleInt(char ch)
{
  size_t result = (size_t)ch - (size_t)'0';
  return result < 10 ? std::pair<bool, size_t>{true, result} : std::pair<bool, size_t>{false, 0};
}

Filesystem parseDisk(std::basic_istream<char>& stm_input)
{
  using memory_block_t = Filesystem::MemoryBlock;

  auto filesystem = Filesystem{std::make_shared<memory_block_t>(0, FREE_SPACE, nullptr, nullptr)};

  auto p_next_block = filesystem.p_head;
  uint64_t file_id_counter = 0;
  for (char ch{}; stm_input.get(ch); ) {
    auto [is_block_size_valid, block_size] = charToPossibleInt(ch);
    if (!is_block_size_valid) {
      std::cout << "[MOAN] Invalid character: '" << ch << "'\n";
      continue;
    }

    if (block_size == 0)
      continue;

    p_next_block->block_size = block_size;
    p_next_block->file_id = file_id_counter;
    ++file_id_counter;

    if (!stm_input) {
      p_next_block->p_next_block = std::make_shared<memory_block_t>(0, FREE_SPACE, nullptr, p_next_block);
      p_next_block = p_next_block->p_next_block;
      continue;
    }

    stm_input.get(ch);
    auto [is_free_blocks_valid, free_blocks] = charToPossibleInt(ch);
    if (!is_free_blocks_valid) {
      std::cout << "[MOAN] Invalid character: '" << ch << "'\n";
      continue;
    }

    for (uint64_t block_count = 0; block_count != free_blocks + 1; ++block_count) {
      p_next_block->p_next_block = std::make_shared<memory_block_t>(1, FREE_SPACE, nullptr, p_next_block);
      p_next_block = p_next_block->p_next_block;
    }
  }

  return filesystem;
}

MemoryBlockIncrement filesystemLastIncrement(Filesystem& filesystem) noexcept
{
  auto p_block = filesystem.p_head;

  if (!p_block) return {nullptr, 0};

  while (p_block->p_next_block) {
    p_block = p_block->p_next_block;
  }

  while (p_block->block_size == 0) {
    p_block = p_block->p_prev_block;
  }

  return {p_block, p_block->block_size - 1};
}

MemoryBlockIncrement& incrementNextFreeSpace(MemoryBlockIncrement& increment) noexcept
{
  auto p_block = increment.p_block;
  while (p_block && p_block->file_id != FREE_SPACE) {
    p_block = p_block->p_next_block;
  }

  return {p_block, 0};
}

std::shared_ptr<Filesystem::MemoryBlock> blockInsert(
    std::shared_ptr<Filesystem::MemoryBlock>& p_block,
    size_t block_size,
    uint64_t file_id
    )
{
  using memory_block_t =  Filesystem::MemoryBlock;

  auto p_prev_block = p_block->p_prev_block;
  auto p_new_block = std::make_shared<memory_block_t>(block_size, file_id, p_block, p_prev_block);
  if (p_prev_block)
    p_prev_block->p_next_block = p_new_block;
  p_block->p_prev_block = p_new_block;
  return p_new_block;
}

void setIncrementFileID(MemoryBlockIncrement increment, uint64_t file_id)
{
  auto& block = *increment.p_block;

  // case 1: block file_id is the same
  if (block.file_id == file_id)
    return;

  // case 2: current block size is 1
  if (block.block_size == 1) {
    block.file_id = file_id;
    return;
  }

  // case 3: increment is at the beginning of the block and previous block has the desired file_id
  //    decrement size of current block
  //    increment size of previous block
  //    set increment to previous block, block size of previous block - 1
  if (increment.increment == 0 && block.p_prev_block->file_id == file_id) {
    auto& prev_block = *block.p_prev_block;
    --block.block_size;
    ++prev_block.block_size;
    increment.p_block = block.p_prev_block;
    increment.increment = prev_block.block_size - 1;
    return;
  }

  // case 4: increment is otherwise at the beginning of the block
  //    decrement size of current block
  //    insert new block element into linked list before the current block with block size 1 and desired file_id
  //    set increment to newly inserted block, 0
  if (increment.increment == 0) {
    --block.block_size;
    auto p_new_block = blockInsert(increment.p_block, 1, file_id);
    increment.p_block = p_new_block;
    increment.increment = 0;
    return;
  }

  // case 5: increment is at the end of the block and next block has desired file_id
  //    decrement size of current block
  //    increment size of next block
  //    set increment to next block, 0
  if (increment.increment == block.block_size - 1 && block.p_next_block->file_id == file_id) {
    auto& next_block = *block.p_next_block;
    --block.block_size;
    ++next_block.block_size;
    increment.p_block = block.p_next_block;
    increment.increment = 0;
    return;
  }

  // case 6: increment is otherwise at the end of the block
  //    decrement size of current block
  //    insert new block element into linked list after the current block with block size 1 and desired file_id
  //    set increment to newly inserted block, 0
  if (increment.increment == block.block_size - 1) {
    --block.block_size;
    auto p_new_block = blockInsert(block.p_next_block, 1, file_id);
    increment.p_block = p_new_block;
    increment.increment = 0;
    return;
  }

  // case 7: increment is elsewhere within a block
  //    size delta : current block size - increment value - 1
  //    subtract the delta + 1 from the current block size
  //    insert new block element after the current with block size of 1 and desired file_id
  //    set increment to newly inserted block, 0
  //    insert new block element after the new block with block size of delta and the old file_id
  {
    size_t delta = block.block_size - increment.increment - 1;
    block.block_size -= delta + 1;
    auto p_new_block = blockInsert(block.p_next_block, 1, file_id);
    blockInsert(p_new_block->p_next_block, delta, block.file_id);
    increment.p_block = p_new_block;
    increment.increment = 0;
    return;
  }
}

void incrementSetFreeSpaceFileID(MemoryBlockIncrement increment, uint64_t file_id)
{
  // case 1: previous block and next block have desired file_id
  //    set increment to the previous block size offset into the previous block
  //    add the next block size + 1 to previous block size
  //    remove current block
  //    remove next_block
  //
  // case 2: previous block has desired file_id
  //
  // case 3: next block has desired file_id
  //
  // case 4: neither adjacent block have desired file_id
}

uint64_t calculateAnswerPart1(std::basic_istream<char>& stm_input)
{
  auto filesystem = parseDisk(stm_input);
  auto inc_free_space = incrementNextFreeSpace(MemoryBlockIncrement{filesystem.p_head, 0});
  auto inc_last = filesystemLastIncrement(filesystem);

  return 0;
}

int main()
{
  std::cout << "Part 1\n======\n";

  {
    auto stm_test_input = std::istringstream{STR_TEST_INPUT};

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

}

