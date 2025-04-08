//
// Created by User on 05/04/2025.
//

#include "../include/Bishop.h"

Bishop::Bishop(bool isWhite, char symbol, int col, int row) : Piece(isWhite, symbol, col, row) {}

bool Bishop::isValidMove( int toRow, int toCol) {
    if (abs(getRow() - toRow) == abs(getCol() - toCol)) {
        return true;
    } else {
        return false;
    }
}