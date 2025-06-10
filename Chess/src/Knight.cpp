//
// Created by User on 05/04/2025.
//

#include "../include/Knight.h"
Knight::Knight(bool isWhite, char symbol, int col, int row) : Piece(isWhite, symbol, col, row) {
    value = 3.0;
}

bool Knight::isValidMove(int toRow, int toCol) const{
    int rowDiff = abs(getRow() - toRow);
    int colDiff = abs(getCol() - toCol);

    if ((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)) {
        return true;
    }
    return false;
}
bool Knight::canAttack(int toRow, int toCol, const Board& board) const {
    // Knight's attack logic is the same as its movement logic,
    // as it jumps over pieces and doesn't need path clearance.
    return isValidMove(toRow, toCol); // Or just directly:
    // int rowDiff = abs(getRow() - toRow);
    // int colDiff = abs(getCol() - toCol);
    // return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}
