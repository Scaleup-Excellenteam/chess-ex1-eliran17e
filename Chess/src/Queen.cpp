//
// Created by User on 05/04/2025.
//

#include "../include/Queen.h"
Queen::Queen(bool isWhite, char symbol, int col, int row) : Piece(isWhite, symbol, col, row) {
    value = 9.0;
}

bool Queen::isValidMove( int toRow, int toCol) const {
    if (abs(getRow() - toRow) == abs(getCol() - toCol)) {
        return true;}
    if ((getCol() == toCol || getRow() == toRow)){
        return true;}
    return false;
    }

bool Queen::canAttack(int toRow, int toCol, const Board& board) const {
    // Queen combines Rook and Bishop movement logic
    if ((getRow() == toRow || getCol() == toCol) || // Straight move
        (abs(getRow() - toRow) == abs(getCol() - toCol))) { // Diagonal move
        return board.isPathClear(getRow(), getCol(), toRow, toCol); // Check path clearance
    }
    return false;
}