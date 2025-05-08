README.txt - Chess Top Move Evaluation System
Project Summary:
This program implements a simplified chess AI that evaluates and ranks the top N legal moves
from a given board state. It uses a scoring algorithm to assess moves based on material gain,
positional advantages, safety, and threats, and returns the best options to the player.

Top Move Evaluation (getTopMoves):
The function `getTopMoves(int count, int depth)` returns the best 'count' moves using a recursive
evaluation function with the given 'depth' (like a mini minimax that we did in python).

Key components of the scoring:
- Material gain (capturing enemy pieces)
- Favorable trades (even small-value trades are rewarded if safe)
- Center control (bonuses for controlling central squares)
- Safety (penalties for unsafe or unprotected squares)
- Threat evaluation (bonus for threatening unprotected valuable enemy pieces)
- Protection bonus (for guarding allied pieces under attack)
- Depth-based opponent replies (simulated recursively with penalty for strong replies)

The system uses a custom `PriorityQueue` to track and return the best N moves sorted by score.
- push() works on O(n)
- poll() works on (1)

Runtime:
- Algorithem run time will be O(Moves^depth) since inside the evaluation moves
- I use for loops but I run on 8x8 (64)
- Depth 1 fast, shallow evaluation
- Depth 2 reasonable accuracy and still fast
- Each extra depth increases time exponentially (due to recursion and branching)

Exceptions:
A custom exception class `MyExceptions` is used throughout the program to handle logical errors such as:
- Invalid board setup (e.g., not 64 characters)
- Polling from an empty priority queue
- No legal moves available (checkmate/stalemate)
- Invalid piece movement or board conditions

All exceptions print helpful error messages and do not crash the program.

Algo