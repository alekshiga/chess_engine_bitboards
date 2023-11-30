#include "Bitboards.h"
#include "ZobristHash.h"
#include "RepetitionHistory.h"

#pragma once

class Position {
private:

    bool whiteShortCastling;
    bool whiteLongCastling;
    bool blackShortCastling;
    bool blackLongCastling;

    float moveCounter;
    ZobristHash hash;
    float fiftyMovesCtr;
    RepetitionHistory repetitionHistory;

    void addPiece(int square, int type, int side) {

    }
    void removePiece(int square, int type, int side) {

    }
    void changeEnPassant(int en_passant) {

    }

    void removeWhiteShortCastling() {

    }
    void removeWhiteLongCastling() {

    }
    void removeBlackShortCastling() {

    }
    void removeBlackLongCastling() {

    }

    void updateMoveCounter() {

    }
    void updateFiftyMovesCounter(bool breakEvent) {

    }

    Chessboard pieces;
    uint8_t enPassant;

public:
    Position() = default;

    Position(const std::string fenNotation,
        int enPassantMove,
        bool whiteShortSideCastle,
        bool whiteLongSideCastle,
        bool blackShortSideCastle,
        bool blackLongSideCastle,
        float moveCounter)
    {
        this->
    }

    friend std::ostream& operator <<(std::ostream& ostream, const Position& position);

    void move(Move move);
};