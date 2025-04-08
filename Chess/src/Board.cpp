#include "../include/Board.h"
#include "../include/Pawn.h"
#include "../include/Knight.h"
#include "../include/Bishop.h"
#include "../include/Rook.h"
#include "../include/Queen.h"
#include "../include/King.h"
#include <stdexcept>
#include <cmath>
using namespace std;

Board::Board(const string &board) {
    if (board.size() != 64) {
        throw invalid_argument("Board size must be 64");
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
    if (dynamic_cast<Pawn*>(chessBoard[fromRow][fromCol]) != nullptr) {
        int direction = chessBoard[fromRow][fromCol]->getIsWhite() ? -1 : 1;

        // Check for diagonal capture
        if (abs(fromCol - toCol) == 1 && fromRow + direction == toRow) {
            Piece* targetPiece = chessBoard[toRow][toCol];
            if (targetPiece != nullptr && dynamic_cast<King*>(targetPiece) == nullptr) {
                if (wouldBeInCheck(fromRow, fromCol, toRow, toCol)) {
                    return 31; // Move would put king in check
                }
                return 42; // Valid diagonal capture
            }
            return 21; // Invalid move (no piece to capture or target is a king)
        }
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
                if (piece->isValidMove(row, col)) {
                    // For all pieces except Knight, check if path is clear
                    if (dynamic_cast<Knight*>(piece) != nullptr) {
                        return true; // Knights can jump over pieces
                    } else if (isPathClear(i, j, row, col)) {
                        return true; // Path is clear, square is under attack
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
    cerr << "Warning: King not found on board for color: " << (isWhite ? "White" : "Black") << endl;
    return {-1, -1}; // added for the times the string not contain 2 kings
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
}