#include <vector>
#include "Bitboards.h"
#include "BitboardOperations.h"
#include "ZobristHashConstants.h"

#pragma once

class ZobristHash {
private:

    std::vector<ZobristHash> hashes;
    Bitboard value;

public:
    ZobristHash() = default;

    void addPosition(ZobristHash hash);
    
    void clear();

    uint8_t getRepetitionNumber(ZobristHash hash) const;

    ZobristHash() = default;

    ZobristHash(Chessboard chessboard, bool blackTurn, bool whiteShortSideCastle, bool whiteLongSideCastle, bool blackShortSideCastling, bool blackLongSideCastling) {
        this->value = 0;

        if (blackTurn) {
            this->invertMove();
        }
        if (whiteShortSideCastle) {
            this->invertWSCastling();
        }
        if (whiteLongSideCastle) {
            this->invertWLCastling();
        }
        if (blackShortSideCastling) {
            this->invertBSCastling();
        }
        if (blackLongSideCastling) {
            this->invertBLCastling();
        }

        uint8_t side;
        for (uint8_t tile = 0; tile < 64; tile = tile + 1) {
            if (BitboardOperations::get_bit(chessboard.getSideBitboard(SIDE::WHITE), tile)) {
                side = SIDE::WHITE;
            }
            else if (BitboardOperations::get_bit(chessboard.getSideBitboard(SIDE::BLACK), tile)) {
                side = SIDE::BLACK;
            }
            else {
                continue;
            }
            for (uint8_t type = 0; type < 6; type = type + 1) {
                if (BitboardOperations::get_bit(chessboard.getPieceBitboard(side, type), tile)) {
                    this->invertPiece(tile, type, side);
                    break;
                }
            }
        }
    }
    static bool operator ==(ZobristHash left, ZobristHash right) {
        return (left.value == right.value);
    }
    void invertPiece(uint8_t tile, uint8_t type, uint8_t side) {
        this->value = this->value ^ ZobristHashConstants::CONSTANTS[tile][side][type];
    }
    void invertMove() {
        this->value = this->value ^ ZobristHashConstants::BLACK_MOVE;
    }
    void invertWSCastling() {
        this->value = this->value ^ ZobristHashConstants::WS_CASTLING;
    }
    void invertWLCastling() {
        this->value = this->value ^ ZobristHashConstants::WL_CASTLING;
    }
    void invertBSCastling() {
        this->value = this->value ^ ZobristHashConstants::BS_CASTLING;
    }
    void invertBLCastling() {
        this->value = this->value ^ ZobristHashConstants::BL_CASTLING;
    }
    uint64_t getValue() const {
        return this->value;
    }
};

