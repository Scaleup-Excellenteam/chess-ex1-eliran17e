//
// Created by User on 05/04/2025.
//

#include "../include/King.h"
King::King(bool isWhite, char symbol, int col, int row) : Piece(isWhite, symbol, col, row) {
    value = 1000;
}

bool King::isValidMove( int toRow, int toCol) const  {
    if (abs(getRow() - toRow) <=1 && abs(getCol() - toCol) <=1) {
        return true;
    }

    return false;
}