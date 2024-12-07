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

Answer: 2742123

## Part 2

1. Split input into two lists for numbers on the left and numbers of the right
2. Sort the right lists
3. Find the sum of each number in the left list multiplied by the number of
   times it appears in the right list

Answer: 21328497

