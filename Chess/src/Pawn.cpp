//
// Created by User on 05/04/2025.
//

#include "../include/Pawn.h"
Pawn::Pawn(bool isWhite, char symbol, int col, int row) : Piece(isWhite, symbol, col, row) {}

bool Pawn::isValidMove( int toRow, int toCol) {
    int direction = isWhite ? 1 : -1; // White pawns move down (-1), black pawns move up (+1)

    // Move forward one square
    if (getCol() == toCol && getRow() + direction == toRow) {
        return true;
    }

    // Move forward two squares from starting position
    if (getCol() == toCol && getRow() + 2 * direction == toRow && (getRow() == 6 || getRow() == 1)) {
        return true;
    }

    // Capture diagonally
    if (abs(getCol() - toCol) == 1 && getRow() + direction == toRow) {
        return true;
    }

    return false;
}