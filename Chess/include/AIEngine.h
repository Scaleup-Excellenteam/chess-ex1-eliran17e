//
// Created by User on 06/06/2025.
//

#ifndef CHESS_AIENGINE_H
#define CHESS_AIENGINE_H
#pragma once
#include "Board.h"


#pragma once

#include "Board.h"
#include <vector>
#include <memory>

    class AIEngine {
    public:
        static std::vector<Move> getTopMovesMultithreaded(Board& board, int count, int depth, int numThreads);

        static Move pickBestMove(const std::vector<Move>& moves);

        static Move pickRandomTopMove(const std::vector<Move>& moves, int topN);


};

#endif //CHESS_AIENGINE_H
