**README**

## Overview
This repository contains solutions for three programming assignments focused on formal languages and automata theory.

### Assignment 1 Task 1 - String Generator (Java)
A program that generates strings from three specific languages (L1, L2, L3) over a given alphabet, based on length and count constraints.

**Key Features:**
- Generates strings in breadth-first order
- L1: All character counts are even
- L2: Contains all alphabet symbols
- L3: Missing exactly one alphabet symbol
- Outputs strings sorted by length and lexicographically

### Assignment 1 Task 2 - DFA Simulator (C++)
A deterministic finite automaton (DFA) simulator that processes input strings and determines acceptance/rejection.

**Key Features:**
- Reads DFA definition (states, alphabet, transitions)
- Processes multiple input strings
- Outputs "A" for accepted strings, "R" for rejected ones
- Handles empty string transitions

### Assignment 2 Task A - FSA to Regular Expression Converter (C++)
Converts a finite state automaton (FSA) to an equivalent regular expression using the state elimination method (Floyd-Warshall variant).

**Key Features:**
- Comprehensive error handling (E1-E7):
  - E1: Malformed input file
  - E2: No initial state
  - E3: Empty accepting states
  - E4: Undefined state
  - E5: Undefined alphabet symbol
  - E6: Disjoint states
  - E7: Non-deterministic FSA
- Supports both deterministic and non-deterministic FSAs
- Outputs regular expression in standard notation

## Technical Notes
- Task 1 uses BFS for string generation
- Task 2 implements exact DFA simulation
- Task 2A implements the state elimination algorithm with tensor representation
- All solutions read from standard input unless specified otherwise
- Task 2A reads from "input.txt" file
