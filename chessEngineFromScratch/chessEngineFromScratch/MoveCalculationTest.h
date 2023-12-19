#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>
#include "Position.h"
#include "LegalMoveGen.h"


#pragma once

#define nsecs std::chrono::high_resolution_clock::now().time_since_epoch().count()


class LegalMoveGenTester {
private:
    struct Test {
        std::string shortFen;
        uint8_t enPassant;
        bool whiteLongCastling;
        bool whiteShortCastling;
        bool blackLongCastling;
        bool blackShortCastling;
        uint8_t alliance;
        std::array<uint64_t, 6> nodes;
    };

public:

    static void test() {

        std::array<Test, 2> tests{};

        tests[0] = Test(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
            Position::NONE,
            true,
            true,
            true,
            true,
            SIDE::WHITE,
            { 1, 20, 400, 8902, 197281, 4865609 } );

        tests[1] = Test(
            "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R",
            Position::NONE,
            true,
            true,
            false,
            false,
            SIDE::WHITE,
            { 1, 44, 1486, 62379, 2103487, 89941194 } );

        for (const auto& test : tests) {
            runTest(test);
        }
    }

    static void runTest(const Test& test) {
        auto position = Position(test.shortFen, test.enPassant, test.whiteLongCastling, test.whiteShortCastling, test.blackLongCastling, test.blackShortCastling, test.alliance);

        for (uint32_t i = 0; i < test.nodes.size(); i = i + 1) {
            uint64_t start = nsecs;

            uint64_t correct = test.nodes[i];
            uint64_t got = getNodesNumber(position, test.alliance, i);

            float speed = (float)got / ((float)(nsecs - start) / (float)1e+9) / (float)1e+6;

            if (correct == got) {
                std::cout << "Depth " << std::setw(4) << i << ". Correct: " << std::setw(18) << correct << ". Got: " << std::setw(18) << got << ". Speed: " << std::setw(10) << speed << " MNPS. OK." << std::endl;
            }
            else {
                std::cout << "Depth " << std::setw(4) << i << ". Correct: " << std::setw(18) << correct << ". Got: " << std::setw(18) << got << ". Speed: " << std::setw(10) << speed << " MNPS. Error." << std::endl;
                std::terminate();
            }
        }
        std::cout << std::endl;
    }

    static uint64_t getNodesNumber(const Position& position, uint8_t alliance, uint32_t depth) {
        if (depth == 0) {
            return 1;
        }

        MoveList moves = LegalMoveGen::calculateLegalMoves(position, alliance, false);
        uint64_t counter = 0;

        for (uint8_t i = 0; i < moves.getSize(); i = i + 1) {
            Move move = moves[i];

            Position copy = position;
            copy.move(move);
            counter = counter + getNodesNumber(copy, Chessboard::invert(alliance), depth - 1);
        }
        return counter;
    }

};