//
// Created by User on 05/04/2025.
//

#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H
#include "Piece.h"

class Bishop: public Piece {

public:
    Bishop(bool isWhite, char symbol, int col, int row);
/**
 *  this function is just a simple function to check diagonal movement
 * @param toRow
 * @param toCol
 * @return if the move was valid
 */
    bool isValidMove( int toRow, int toCol) override;

};


#endif //CHESS_BISHOP_H
