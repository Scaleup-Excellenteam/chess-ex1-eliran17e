#include "../include/Board.h"
#include "../include/Pawn.h"
#include "../include/Knight.h"
#include "../include/Bishop.h"
#include "../include/Rook.h"
#include "../include/Queen.h"
#include "../include/King.h"
#include "../include/PriorityQueue.h"
#include <stdexcept>
#include <cmath>
#include <memory>
using namespace std;

Board::Board(const string &board) {
    if (board.size() != 64) {
        throw MyExceptions("Board size must be 64");
    }
    whiteTurn = true;
    int col = 0;
    int row = 0;
    chessBoard.resize(8, vector<Piece*>(8, nullptr));
    for(char c : board) {
        switch (c) {
            case 'P': chessBoard[row][col] = new Pawn(true, 'P', col, row); break;
            case 'p': chessBoard[row][col] = new Pawn(false, 'p', col, row); break;
            case 'N': chessBoard[row][col] = new Knight(true, 'N', col, row); break;
            case 'n': chessBoard[row][col] = new Knight(false, 'n', col, row); break;
            case 'B': chessBoard[row][col] = new Bishop(true, 'B', col, row); break;
            case 'b': chessBoard[row][col] = new Bishop(false, 'b', col, row); break;
            case 'R': chessBoard[row][col] = new Rook(true, 'R', col, row); break;
            case 'r': chessBoard[row][col] = new Rook(false, 'r', col, row); break;
            case 'Q': chessBoard[row][col] = new Queen(true, 'Q', col, row); break;
            case 'q': chessBoard[row][col] = new Queen(false, 'q', col, row); break;
            case 'K': chessBoard[row][col] = new King(true, 'K', col, row); break;
            case 'k': chessBoard[row][col] = new King(false, 'k', col, row); break;
            case '#': chessBoard[row][col] = nullptr; break;
            default: break;
        }
        col++;
        if (col == 8) {
            col = 0;
            row++;
        }
    }
}

Board::~Board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            delete chessBoard[i][j];
        }
    }
}

int Board::checkMove(const std::string &command) {
    int fromRow = command[0] - 'a';
    int fromCol = command[1] - '1';
    int toRow = command[2] - 'a';
    int toCol = command[3] - '1';
    int result = isLegalMove(fromRow, fromCol, toRow, toCol);
    if (result == 42 || result == 41) {
        makeMove(fromRow, fromCol, toRow, toCol);
    }
    return result;
}

int Board::isLegalMove(int fromRow, int fromCol, int toRow, int toCol) {
    // Check if there's a piece at the source position
    if (chessBoard[fromRow][fromCol] == nullptr) {
        return 11; // No piece at source
    }

    // Check if the piece belongs to the current player
    if (whiteTurn != chessBoard[fromRow][fromCol]->getIsWhite()) {
        return 12; // Piece belongs to opponent
    }

    // Check if destination has a piece of the same color
    if (chessBoard[toRow][toCol] != nullptr &&
        chessBoard[toRow][toCol]->getIsWhite() == chessBoard[fromRow][fromCol]->getIsWhite()) {
        return 13; // Destination occupied by own piece
    }

    // Check if the move pattern is valid for the specific piece
    if (!chessBoard[fromRow][fromCol]->isValidMove(toRow, toCol)) {
        return 21; // Invalid move pattern for this piece
    }
    // Special handling for pawn diagonal capture
    if (auto* pawn = dynamic_cast<Pawn*>(chessBoard[fromRow][fromCol])) {
        int direction = pawn->getIsWhite() ? 1 : -1; // white moves down (row - 1), black moves up (row + 1)
        int rowDiff = toRow - fromRow;
        int colDiff = toCol - fromCol;
        Piece* target = chessBoard[toRow][toCol];

        // Diagonal capture
        if (abs(colDiff) == 1 && rowDiff == direction) {
            if (target != nullptr && target->getIsWhite() != pawn->getIsWhite()) {
                if (wouldBeInCheck(fromRow, fromCol, toRow, toCol)) return 31;
                return 42;
            }
            return 21; // diagonal but no enemy piece
        }

        // Forward move
        if (colDiff == 0) {
            if (target != nullptr) return 21; // can't move forward into a piece

            if (rowDiff == direction) {
                // single step
                if (wouldBeInCheck(fromRow, fromCol, toRow, toCol)) return 31;
                return 42;
            }

            // double step from starting row
            if (rowDiff == 2 * direction &&
                ((pawn->getIsWhite() && fromRow == 1) || (!pawn->getIsWhite() && fromRow == 6))) {
                if (chessBoard[fromRow + direction][fromCol] != nullptr) return 21;
                if (wouldBeInCheck(fromRow, fromCol, toRow, toCol)) return 31;
                return 42;
            }
        }

        return 21; // any other move is illegal
    }
    // For pieces other than Knight, check if the path is clear
    if (dynamic_cast<Knight*>(chessBoard[fromRow][fromCol]) == nullptr) {
        if (!isPathClear(fromRow, fromCol, toRow, toCol)) {
            return 21; // Path is blocked
        }
    }

    // Check if the move would put the king in check
    if (wouldBeInCheck(fromRow, fromCol, toRow, toCol)) {
        return 31; // Move would put king in check
    }

    // Check if the move creates a check on opponent's king
    if (wouldCauseCheck(fromRow, fromCol, toRow, toCol)) {
        return 41; // Legal move that causes check
    }

    // Legal move
    return 42;
}

bool Board::isPathClear(int fromRow, int fromCol, int toRow, int toCol) {
    // Horizontal movement
    if (fromRow == toRow) {
        int step = (toCol > fromCol) ? 1 : -1;
        for (int col = fromCol + step; col != toCol; col += step) {
            if (chessBoard[fromRow][col] != nullptr) {
                return false; // Path is blocked
            }
        }
        return true;
    }

    // Vertical movement
    if (fromCol == toCol) {
        int step = (toRow > fromRow) ? 1 : -1;
        for (int row = fromRow + step; row != toRow; row += step) {
            if (chessBoard[row][fromCol] != nullptr) {
                return false; // Path is blocked
            }
        }
        return true;
    }

    // Diagonal movement
    if (abs(toRow - fromRow) == abs(toCol - fromCol)) {
        int rowStep = (toRow > fromRow) ? 1 : -1;
        int colStep = (toCol > fromCol) ? 1 : -1;

        int row = fromRow + rowStep;
        int col = fromCol + colStep;

        while (row != toRow && col != toCol) {
            if (chessBoard[row][col] != nullptr) {
                return false; // Path is blocked
            }
            row += rowStep;
            col += colStep;
        }
        return true;
    }
    // If we get here, the move is neither horizontal, vertical, nor diagonal
    return false;
}

bool Board::wouldBeInCheck(int fromRow, int fromCol, int toRow, int toCol) {
    // Save the current state
    Piece* movingPiece = chessBoard[fromRow][fromCol];
    Piece* capturedPiece = chessBoard[toRow][toCol];

    // Simulate the move
    chessBoard[fromRow][fromCol] = nullptr;
    chessBoard[toRow][toCol] = movingPiece;

    // Find the king's position
    int kingRow = -1, kingCol = -1;
    bool isWhiteKing = movingPiece->getIsWhite();

    // If the moving piece is the king, use the destination as king's position
    if (dynamic_cast<King*>(movingPiece) != nullptr) {
        kingRow = toRow;
        kingCol = toCol;
    } else {
        // Otherwise, find the king
        auto kingPos = findKing(isWhiteKing);
        kingRow = kingPos.first;
        kingCol = kingPos.second;
    }

    // Check if any opponent piece can attack the king
    bool isInCheck = isSquareUnderAttack(kingRow, kingCol, !isWhiteKing);

    // Restore the board state
    chessBoard[fromRow][fromCol] = movingPiece;
    chessBoard[toRow][toCol] = capturedPiece;

    return isInCheck;
}

bool Board::wouldCauseCheck(int fromRow, int fromCol, int toRow, int toCol) {
    Piece* movingPiece = chessBoard[fromRow][fromCol];
    Piece* capturedPiece = chessBoard[toRow][toCol];

    // Save original piece position
    int origRow = movingPiece->getRow();
    int origCol = movingPiece->getCol();

    // Simulate move
    chessBoard[fromRow][fromCol] = nullptr;
    chessBoard[toRow][toCol] = movingPiece;
    movingPiece->setRow(toRow);
    movingPiece->setCol(toCol);

    auto opponentKingPos = findKing(!movingPiece->getIsWhite());
    int kingRow = opponentKingPos.first;
    int kingCol = opponentKingPos.second;

    bool causesCheck = isSquareUnderAttack(kingRow, kingCol, movingPiece->getIsWhite());

    // Undo move
    chessBoard[fromRow][fromCol] = movingPiece;
    chessBoard[toRow][toCol] = capturedPiece;
    movingPiece->setRow(origRow);
    movingPiece->setCol(origCol);

    return causesCheck;
}

bool Board::isSquareUnderAttack(int row, int col, bool byWhite) {
    // Check if any piece of the specified color can attack this square
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = chessBoard[i][j];
            if (piece != nullptr && piece->getIsWhite() == byWhite) {
                // Check if the piece can move to the square
                if (auto* pawn = dynamic_cast<Pawn*>(piece)) {
                    if (pawn->canAttack(row, col)) {
                        return true;
                    }
                } else if (piece->isValidMove(row, col)) {
                    if (dynamic_cast<Knight*>(piece)) {
                        return true;
                    } else if (isPathClear(i, j, row, col)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

std::pair<int, int> Board::findKing(bool isWhite) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece* piece = chessBoard[row][col];
            if (piece != nullptr && piece->getIsWhite() == isWhite &&
                dynamic_cast<King*>(piece) != nullptr) {
                return {row, col};
            }
        }
    }
    throw MyExceptions("King not found for " + std::string(isWhite ? "White" : "Black"));

}

void Board::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    // Actually execute the move on the board
    Piece* piece = chessBoard[fromRow][fromCol];
    chessBoard[fromRow][fromCol] = nullptr;
    chessBoard[toRow][toCol] = piece;

    if (piece != nullptr) {
        piece->setRow(toRow);
        piece->setCol(toCol);
    }

    // Switch turns for future changes that might be needed
    whiteTurn = !whiteTurn;

    // Check for pawn promotion
    if (piece != nullptr && dynamic_cast<Pawn*>(piece)) {
        Pawn* pawn = dynamic_cast<Pawn*>(piece);
        if ((pawn->getIsWhite() && toRow == 7) || (!pawn->getIsWhite() && toRow == 0)) {
            handlePromotion(toRow, toCol, pawn->getIsWhite());
        }
    }
}
std::vector<Move> Board::getAllValidMoves(bool turn) {
    std::vector<Move> moves;
    for (int fromRow = 0; fromRow < 8; ++fromRow) {
        for (int fromCol = 0; fromCol < 8; ++fromCol) {
            Piece* piece = chessBoard[fromRow][fromCol];
            if (piece && piece->getIsWhite() == turn) {
                for (int toRow = 0; toRow < 8; ++toRow) {
                    for (int toCol = 0; toCol < 8; ++toCol) {
                        int res = isLegalMove(fromRow, fromCol, toRow, toCol);
                        if (res == 41 || res == 42) {
                            Move m;
                            m.fromRow = fromRow;
                            m.fromCol = fromCol;
                            m.toRow = toRow;
                            m.toCol = toCol;


                            m.notation = std::string(1, 'a' + fromRow) + std::to_string(fromCol + 1)
                                         + std::string(1, 'a' + toRow) + std::to_string(toCol + 1);
                            moves.push_back(m);
                        }
                    }
                }
            }
        }
    }
    return moves;
}

void Board::evaluateAllMoves(int depth) {
    currentMoves.clear(); // store them here if needed later
    currentMoves = getAllValidMoves(whiteTurn);
    for (Move& move : currentMoves) {
        move.score = evaluateMove(move, depth);
    }
}
double Board::evaluateMove(const Move& move, int depth) {
    double score = 0;
    Piece* attacker = chessBoard[move.fromRow][move.fromCol];
    Piece* target = chessBoard[move.toRow][move.toCol];

    if (!attacker) return -999; // Invalid move - no piece at source

    // Save original piece position
    int origRow = attacker->getRow();
    int origCol = attacker->getCol();

    // Simulate move
    chessBoard[move.fromRow][move.fromCol] = nullptr;
    chessBoard[move.toRow][move.toCol] = attacker;
    attacker->setRow(move.toRow);
    attacker->setCol(move.toCol);

    // Base score from board position
    score += evaluateBoardPosition(whiteTurn);



    // Center control bonus - this should now have a significant effect
    if ((move.toRow >= 2 && move.toRow <= 5) && (move.toCol >= 2 && move.toCol <= 5)) {
        score += 2.0;

    }
    if ((move.toRow == 3 || move.toRow == 4) && (move.toCol == 3 || move.toCol == 4)) {
        score += 4.5;

    }

    // Safety bonus
    if (!isSquareUnderAttack(move.toRow, move.toCol, !attacker->getIsWhite())) {
        score += 0.3;
    }

    // Capturing a piece
    if (target) {
        double captureValue = target->getValue();
        score += captureValue;


        bool isSafeCapture = !isSquareUnderAttack(move.toRow, move.toCol, !attacker->getIsWhite());
        bool attackerIsProtected = isSquareProtected(move.toRow, move.toCol, attacker->getIsWhite());

        // Safe capture = bonus
        if (isSafeCapture) {
            double safeBonus = captureValue * 2;
            score += safeBonus;

        }

        // Encourage equal or favorable trades
        if (attacker->getValue() <= target->getValue()) {
            score += 1.5;

        }

        // Penalty if the capture is dangerous
        if (!isSafeCapture && !attackerIsProtected) {
            double loss = attacker->getValue() - target->getValue();
            if (loss > 0) {
                double penalty = loss * 2;
                score -= penalty;

            }
        }
    }

    // Penalty for moves to unsafe squares
    if (isSquareUnderAttack(move.toRow, move.toCol, !attacker->getIsWhite()) &&
        !isSquareProtected(move.toRow, move.toCol, attacker->getIsWhite()))
    {
        double lossPenalty = attacker->getValue() * 1.2;
        score -= lossPenalty;

    }

    // Evaluate piece threats and protections
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (chessBoard[r][c] == nullptr) continue;
            Piece* piece = chessBoard[r][c];
            if (!piece || piece == attacker) continue;

            bool isEnemy = piece->getIsWhite() != attacker->getIsWhite();
            bool isAlly = !isEnemy;

            // Check if attacker can attack this piece after the move
            if (attacker->canAttack(r, c) &&
                (dynamic_cast<Knight*>(attacker) != nullptr ||
                 dynamic_cast<Pawn*>(attacker) != nullptr ||
                 isPathClear(move.toRow, move.toCol, r, c))) {

                // Threatening valuable enemy
                if (isEnemy) {
                    double diff = piece->getValue() - attacker->getValue();
                    if (diff >= -1.0) {
                        double threatBonus = std::max(0.5, diff) * 0.8;
                        score += threatBonus;


                        if (!isSquareProtected(r, c, piece->getIsWhite())) {
                            double unprotectedBonus = std::max(1.0, diff) * 1.2;
                            score += unprotectedBonus;

                        }
                    } else if (!isSquareProtected(r, c, piece->getIsWhite())) {
                        score += 1.2;

                    }
                }

                // Protecting threatened ally
                if (isAlly && isSquareUnderAttack(r, c, !attacker->getIsWhite())) {
                    bool protectorIsSafe = !isSquareUnderAttack(move.toRow, move.toCol, !attacker->getIsWhite());
                    if (protectorIsSafe) {
                        double protectionBonus = piece->getValue() * 0.2;
                        score += protectionBonus;

                    } else {
                        score -= 0.3;  // Small penalty for risky protection

                    }
                }
            }
        }
    }

    // Apply recursion for deeper evaluation (opponent's best reply)
    if (depth > 0) {
        whiteTurn = !whiteTurn;  // Switch turn temporarily
        std::vector<Move> replies = getAllValidMoves(whiteTurn);

        double bestReplyScore = -1000.0;  // Very low initial value
        for (const Move& reply : replies) {

            // (what's good for opponent is bad for us)
            double replyScore = -evaluateMove(reply, depth - 1);
            bestReplyScore = std::max(bestReplyScore, replyScore);
        }

        whiteTurn = !whiteTurn;  // Switch turn back


        double replyPenalty = bestReplyScore * 0.8;
        score -= replyPenalty;

    }

    // Undo move
    chessBoard[move.toRow][move.toCol] = target;
    chessBoard[move.fromRow][move.fromCol] = attacker;
    attacker->setRow(origRow);
    attacker->setCol(origCol);


    return score;
}

std::vector<Move> Board::getTopMoves(int count, int depth) {
    evaluateAllMoves(depth); // calculate full scores for all currentMoves
    PriorityQueue<std::shared_ptr<Move>> pq;

    if (currentMoves.empty()) {
        throw MyExceptions("No legal moves available (checkmate or stalemate).");
    }

    for (Move& m : currentMoves) {
        auto ptr = std::make_shared<Move>(m);
        pq.push(ptr);
    }

    std::vector<Move> top;
    for (int i = 0; i < count && !pq.empty(); ++i) {
        top.push_back(*pq.poll());
    }


    for (const Move& m : top) {
        Piece* piece = chessBoard[m.fromRow][m.fromCol];

    }

    return top;
}
std::ostream& operator<<(std::ostream& os, const Move& m) {
    os << m.notation << " (score: " << m.score << ")";
    return os;
}
// Check if a square is protected by pieces of a certain color
bool Board::isSquareProtected(int row, int col, bool byWhite) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = chessBoard[r][c];
            if (piece && piece->getIsWhite() == byWhite) {
                if (piece->canAttack(row, col)) {
                    if (dynamic_cast<Knight*>(piece) || dynamic_cast<Pawn*>(piece)) return true;
                    if (isPathClear(r, c, row, col)) return true;
                }
            }
        }
    }
    return false;
}
double Board::evaluateBoardPosition(bool isWhite) {
    double score = 0.0;

    int homeRow = isWhite ? 0 : 7;
    int pawnStartRow = isWhite ? 1 : 6;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece *piece = chessBoard[row][col];
            if (!piece || piece->getIsWhite() != isWhite) continue;

            // Reward pawn development (moved from start row)
            if (auto *p = dynamic_cast<Pawn *>(piece)) {
                if (row != pawnStartRow) score += 0.5;
            }

            // Reward minor piece development
            if ((dynamic_cast<Knight *>(piece) || dynamic_cast<Bishop *>(piece)) && row != homeRow) {
                score += 0.5;
            }

        }}


    return score;
}
// Determine the game stage (0=opening, 1=middlegame, 2=endgame). will use it to improve the code later on.
int Board::getGameStage() {
    int pieceCount = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (chessBoard[r][c] != nullptr) pieceCount++;
        }
    }

    if (pieceCount >= 28) return 0; // Opening
    if (pieceCount > 16) return 1;  // Middlegame
    return 2;                       // Endgame
}
// Evaluate king safety. will use it to improve the code later on.
double Board::evaluateKingSafety(int row, int col, bool isWhite, int gameStage) {
    double score = 0;

    if (gameStage == 2) return 0; // King safety less important in endgame

    // Penalize king in center during opening or middlegame
    if ((row >= 2 && row <= 5) && (col >= 2 && col <= 5)) {
        score -= 5.0;
        if (gameStage == 0) score -= 3.0; // Extra penalty in opening
    }

    // Check pawn shield
    int pawnShield = 0;
    int frontRow = isWhite ? row - 1 : row + 1;
    if (frontRow >= 0 && frontRow < 8) {
        for (int c = col - 1; c <= col + 1; c++) {
            if (c >= 0 && c < 8) {
                Piece* piece = chessBoard[frontRow][c];
                if (piece && dynamic_cast<Pawn*>(piece) && piece->getIsWhite() == isWhite) {
                    pawnShield++;
                }
            }
        }
    }
    score += pawnShield * 1.5;

    return score;
}


void Board::handlePromotion(int row, int col, bool isWhite) {
    char choice;
    while (true) {
        std::cout << "Pawn promotion! Choose (q)ueen, (r)ook, (b)ishop, (n)ight: ";
        std::cin >> choice;
        choice = std::tolower(choice);

        Piece* promoted = nullptr;
        switch (choice) {
            case 'q':
                promoted = new Queen(isWhite, isWhite ? 'Q' : 'q', col, row);
                break;
            case 'r':
                promoted = new Rook(isWhite, isWhite ? 'R' : 'r', col, row);
                break;
            case 'b':
                promoted = new Bishop(isWhite, isWhite ? 'B' : 'b', col, row);
                break;
            case 'n':
                promoted = new Knight(isWhite, isWhite ? 'N' : 'n', col, row);
                break;
            default:
                std::cout << "Invalid input. Try again.\n";
                continue;
        }

        delete chessBoard[row][col]; // Delete the pawn
        chessBoard[row][col] = promoted;
        break;
    }
}
// Helper function that helped me debug the code keeping it for future code upgrades.
void Board::dumpBoard() {
    std::cout << "Current board state:\n";
    for (int row = 0; row < 8; ++row) {
        std::cout << (char)('A' + row) << " ";
        for (int col = 0; col < 8; ++col) {
            if (chessBoard[row][col]) {
                std::cout << chessBoard[row][col]->getSymbol() << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "  1 2 3 4 5 6 7 8\n";
}