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

Method worked out on paper then implemented below.

I'd like to assume that each report could only have one issue, but I wrote a
guard for this just in case.

## Answers

<details>
 <summary>Spoilers</summary>
 Part 1: 282
 Part 2:
</details>

