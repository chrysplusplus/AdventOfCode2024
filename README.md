# Advent of Code Day 1

Input data is `1.txt`

INPUT_FILE is passed by the compiler

## Part 1

1. Split input into two lists for numbers on the and numbers on the right
2. Sort the two lists
3. Find the sum of the differences of the two lists at each index position

Observation:
- Each line is a sequence of five numeric characters, three spaces, then
  another sequence of five numeric characters
- There are 1000 lines of input

## Part 2

1. Split input into two lists for numbers on the left and numbers of the right
2. Sort the right lists
3. Find the sum of each number in the left list multiplied by the number of
   times it appears in the right list

## Answers

<details>
<summary>Spoilers</summary>
Part 1: 2742123

Part 2: 21328497
</details>

# Advent of Code: Day 2

Input for this program is `2.txt`

## Part 1

1. Read input into a list of reports, each containing 5 numbers
2. Find the number of safe reports. A report is safe if both:
 - The numbers in sequence are all increasing or all decreasing
 - Any two adjacent numbers differ by at least 1 and at most 3

Observations:
- Example data has reports of length 5, but actual input uses a varying
  length

## Part 2

1. Read input into a list of reports as above
2. Find the number of safe-ish reports. Same conditions as above, also check
   if a report is safe if one and only number is dropped
 - Do this by heuristically checking every subreport

## Answers

<details>
 <summary>Spoilers</summary>
 Part 1: 282

 Part 2: 349
</details>

# Advent of Code: Day 3

Input for this program is `3.txt`

## Part 1

1. Parse the input into a sequence of valid tokens `mul(XX,YY)` where XX and
   YY are an n-lenth sequence of numeric characters
2. Take the sum of all the multiplications

Observations:
- There are size lines of text in the program data file, but newlines will be
  treated like any other character

## Part 2

1. Same as above, handling extra tokens `do()` and `don't` which enable and
   disable multiplication respectively

## Answers

<details>
 <summary>Spoilers</summary>
 Part 1: 174103751

 Part 2: 100411201
</details>

# Advent of Code: Day 4

Input for this program is `4.txt`

## Part 1

1. Read the input into a grid of characters
2. For every instance of `X` in the grid, search in all 8 directions for `M`
3. If `M` is found, continue in that direction and check for `A` and `S`
4. Count the number of successful searches

Observations:
- The input grid is a square

## Part 2

1. Read the input into a grid of characters
2. For every instance of `A` in the grid, set a diagonal count of 0
3. For each diagonal, check for `M`
4. If `M` is found, check for `S` in the opposite direction
5. If `S` is found, increment the diagonal count for the associated `A`
6. Count the number of `A` with a diagonal count of 2

Observations:
- The orientation of the X is unimportant, it just needs to read `MAS` on both
  diagonals, forwards or backwards

## Answer

<details>
 <summary>Spoilers</summary>
 Part 1: 2575

 Part 2: 2041
</details>

# Advent of Code: Day 5

Input for this program is `5.txt`

## Part 1

1. Parse pages order rules and update lists
2. Parse order rules into a lookup map with ascending and descending order
   rules
3. For each update list, find the pairs of every page with every following page
4. Lookup these pairs in the lookup map, if any yield a descending order, then
   the associated update is invalid
5. Take the sum of the middle value of all valid updates

Observations:
- Page updates seem to always contain an odd amount of pages

## Part 2

1. Parse page order rules and updates lists as in part 1
2. For each update list, search for a pair of any two pages in order that
   yields a descending order in the lookup map
3. Swap the values in the update list
4. Reattempt a search for a desending pair of pages
5. Take the middle value of all modified update lists

## Answer

<details>
 <summary>Spoilers</summary>
 Part 1: 6505

 Part 2: 6897
</details>

# Advent of Code: Day 6 (Incomplete)

Input for this program is `6.txt`

## Part 1

1. Parse input into a grid of tiles, which are either ground or obstruction,
   recording also the starting position and orientation of the guard
2. While the guard is still within the grid and has not return to their
   starting position and orientation:
   - If the guard is facing an obstruct, turn right 90 degrees
   - Otherwise the guard moves in the direction they are facing
3. Find the number of positions within the grid in the guard's path

## Part 2

1. Parse input into a grid of tiles and starting position and orientation of a
   guard, as above
2. Travel around the grid using the same rules as above, but for each tile that
   the guard travels, check if a theoretical guard, started at the guard's
   position, but turned right by 90 degrees would loop
3. Count the number of positions at which the guard could theoretically loop
   were an obstale placed in front of them

## Answer

<details>
 <summary>Spoilers</summary>
 Part 1: 4433

 Part 2:
</details>

# Advent of Code: Day 7

Input for this program is `7.txt`

## Part 1

1. Parse equations
2. Create the equations results by incrementally performing adding and
   multiplication on a branching set of numbers for each operand
3. Search through the final equation results for the expected result
4. If the expected result is found, this equation is valid
5. Take the sum of all valid equations

## Part 2

1. Parse equations
2. Create the equations results as above, only with another concatenation
   operator (explained below)
3. Search for expected result; if found, equation is valid
4. Take the sum of all valid equations

Concatenation:
concat(10, 345) -> 10345

I have implemented concatenation by iteratively dividing a temporary copy of
the second operand by 10 and multiplying the first operand by 10; then adding
the first and second operands. So above, 345 can be divided by 10 three times,
so the first operand is multiplied by 10 three times to become 10000, then
added to 345, yielding 10345.

## Answers

<details>
 <summary>Spoilers</summary>
 Part 1: 1620690235709

 Part 2: 145397611075341
</details>

# Advent of Code: Day 8

Input for this program is `8.txt`

## Part 1

1. Parse grid of grid spaces with antennas
2. For each pair of spaces:
   - Check that the spaces are not the same space
   - Check that the second space has an antenna
   - Calculate the far space, which is twice the distance from the first
     space to the second space
   - If the far space has the same antenna frequency as the second space,
     mark the first space as having an antinode
3. Take the number of spaces having antinodes as the asnwer

## Part 2

1. Parse grid of grid spaces with antenna
2. For each pair of spaces:
   - Check that the spaces are not the same space
   - Check that the first space has an antenna
   - Check that the first and second space both have the same antenna
3. For each successful pair, find the distance between them
4. Simplify this distance to small integer step, similar to simplifying a
   fraction
5. Mark every multiple of this step distance from the first space as having
   an antinode (this will mark the second space; since every pair is visited
   twice, the inverse will happen when the second space and first space are
   swapped)
6. Take the number of spaces having antinodes as the answer

## Answers

<details>
 <summary>Spoilers</summary>
 Part 1: 409

 Part 2: 1308
</details>

# Advent of Code: Day 9

Input for this program is `9.txt`

The algorithms below may no longer reflect the code implementation

## Part 1

1. Parse the input character stream into a contiguous memory
2. Take a pointer to the first free space in the memory and another to the last
   non-free space in memory
3. Swap them
4. Increment first pointer to next free space and decrement second to the next
   non-free space
5. Repeat 3-4 until the position of the first pointer in the memory is after after
   the second
6. Take the sum of the products of every non free space value in the memory and
   its index in the memory

## Part 2

1. Parse the input disk into contiguous memory as above, also storing the
   distance to the next block
2. For each non-free space block, starting from the end of memory, take its
   block size
3. Find a pointer to a block of free space from the beginning of memory that
   has a block size greater than or equal to the block size from 2
4. If no block is found, continue to next non-free block from the end at 2
5. Swap the blocks
6. Repeat 2-5 for all non-free space blocks
7. Calculate the checksum as above

## Answers

<details>
 <summary>Spoilers</summary>
 Part 1: 6421128769094

 Part 2: 6448168620520
</details>

