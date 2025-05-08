//
// Created by User on 05/04/2025.
//

#include "../include/Piece.h"

Piece::Piece(bool isWhite, char symbol, int col, int row)
        : isWhite(isWhite), symbol(symbol), col(col), row(row) {}



Piece::Piece(const Piece& other)
        : isWhite(other.isWhite), symbol(other.symbol), col(other.col), row(other.row) {}

Piece& Piece::operator=(const Piece& other) {
    if (this == &other) {
        return *this; // Handle self-assignment
    }

    // Copy the data members
    this->symbol = other.symbol;
    this->isWhite = other.isWhite;
    this->col = other.col;
    this->row = other.row;

    return *this;
}
int Piece::getValue() const {
    return value;
}



char Piece::getSymbol() const {
    return symbol;
}

bool Piece::getIsWhite() const {
    return isWhite;}

int Piece::getCol() const {
    return col;
}
int Piece::getRow() const {
    return row;
}
void Piece::setCol(int x) {
    Piece::col = x;
}
void Piece::setRow(int x) {
    Piece::row = x;
}
