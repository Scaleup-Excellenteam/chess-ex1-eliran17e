README - Chess Engine Project
=============================

Overview
--------
This project is a console-based chess engine written in C++.
The engine supports manual play, play vs computer (AI), and autoplay (AI vs AI).
The AI suggests top moves based on board evaluation,
and can use multiple threads to speed up its calculations.

Features & Rules Implemented
----------------------------
- Full chess move legality enforcement, including pawn promotion and castling.
- Check, checkmate, stalemate, and draw by insufficient material are all supported.
- Threefold repetition rule is implemented: The engine tracks board positions and declares a draw if the same position occurs three times.
- Castling rules are enforced, including checks for whether the king/rook have moved and whether the path is blocked (only works for manual player not AI).
- Manual, autoplay, and play vs AI modes available.
- Multithreading support for faster AI analysis (user chooses thread count).
- Error handling with informative messages for invalid moves.

Extra Features (Not Implemented)
-------------------------------
- No graphical user interface (console only).
- No support for en passant .

How the Computer Player Chooses Moves
-------------------------------------
- The AI uses a minimax-based evaluation to score moves:
    - Considers material gain (piece captures).
    - Rewards good positioning (center control, king safety, protected pieces).
    - Evaluates riskiness of captures (safe/favorable trades).
    - Simulates the opponent’s best replies up to a user-defined depth (1-6).
- Uses a priority queue to rank moves and can pick randomly from the top N for more human-like play.
- In autoplay, the AI selects randomly from the top 5 moves.
- Supports both single-threaded and multithreaded evaluation for faster calculations.

Testing and Debugging
---------------------
- The engine was tested via:
    - Manual games in all three modes.
    - Edge cases (castling, threefold repetition, stalemate, and checkmate).
    - Print/debug statements in the code.

Challenges and What Took Most Time
----------------------------------
- Implementing efficient and bug-free castling logic and move legality checks.
- Debugging threefold repetition detection and ensuring correct draw conditions.
- Optimizing the AI for better performance with multithreading and deeper search depths.

Improvements with More Time
---------------------------
- Add undo/redo, save/load, and move timers.
- Add en passant support .
- Improve AI evaluation heuristics (for stronger play).
- Add a graphical or web-based interface.


Usage Instructions
------------------
1. Build the project using your favorite C++ compiler.
2. Run the executable. You will be prompted to choose:
    - AI search depth (1-6; higher is stronger but slower)
    - Number of threads (0 for single-threaded, up to 8 for multithreading)
    - Mode:
        - 0 = Manual (you play both sides, with AI suggestions)
        - 1 = Autoplay (AI vs AI for a specified number of moves)
        - 2 = Play vs Computer (choose your color)
    - For autoplay mode, enter number of moves to play (1-1000)
3. Enter your moves in algebraic notation (e.g., `b4d4`).
4. Type "exit" to quit the game.