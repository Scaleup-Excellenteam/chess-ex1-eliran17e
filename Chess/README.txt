Overview:
This project is a basic chess engine that suggests the best moves using AI logic.
It supports both manual play and autoplay modes.
You can adjust the AI's depth and choose whether to use multithreading to improve performance.

Features:
Calculates and suggests the top moves based on:
Piece captures and material gain
Positional advantages (center control, protection, threats)
Safety and favorable trades
Opponent's possible replies (up to a given depth)
Supports autoplay mode, where the AI plays both sides by randomly choosing from the top 5 best moves.
Uses multithreading (optional) for faster move evaluations.

How It Works:
The user inputs:
AI depth (how many future moves to look ahead)
Number of threads (0 = single-threaded)
Mode: Manual play or Autoplay
In Manual mode, you play moves yourself and the AI gives suggestions.
In Autoplay, the AI plays 8 moves automatically, showing the board after each.

Error Handling:
Custom exceptions (MyExceptions) are used for:
Invalid board setups
Illegal moves
No moves available
Empty priority queues
Errors are shown without crashing the program.

Performance Notes:
The deeper the depth, the more accurate but slower the evaluation.
Runtime grows roughly like O(moves^depth).