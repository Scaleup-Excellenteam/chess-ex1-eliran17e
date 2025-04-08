//
// Created by User on 05/04/2025.
//

#include "../include/Queen.h"
Queen::Queen(bool isWhite, char symbol, int col, int row) : Piece(isWhite, symbol, col, row) {}

bool Queen::isValidMove( int toRow, int toCol) {
    if (abs(getRow() - toRow) == abs(getCol() - toCol)) {
        return true;}
    if ((getCol() == toCol || getRow() == toRow)){
        return true;}
    return false;
    }