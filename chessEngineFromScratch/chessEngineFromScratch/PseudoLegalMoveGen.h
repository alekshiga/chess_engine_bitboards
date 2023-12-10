#include <bit>
#include "Bitboards.h"
#include "KnightMask.h"
#include "KingMask.h"
#include "SlidersMask.h"

#pragma once

class PseudoLegalMove {

    Bitboard calculateKnightMask(Chessboard chessboard, uint8_t position, uint8_t alliance, bool onlyCaptures) {
        if (onlyCaptures) {
            uint8_t oppositeAlliance = chessboard.invert(alliance);
            return KnightMasks::masks[position] & chessboard.getSideBitboard(oppositeAlliance);
        }
        return KnightMasks::masks[position] & chessboard.getInvertedSideBitboard(alliance);
    }
    Bitboard calculateBishopMask(Chessboard chessboard, uint8_t position, uint8_t alliance, bool onlyCaptures) {
        Bitboard northWest = calculateAttackBeam(chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::NORTH_WEST, false);
        Bitboard northEast = calculateAttackBeam(chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::NORTH_EAST, false);
        Bitboard southWast = calculateAttackBeam(chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::SOUTH_WEST, true);
        Bitboard southEast = calculateAttackBeam(chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::SOUTH_EAST, true);
        return northWest | northEast | southWast | southEast;
    }

    Bitboard calculateRookMask(Chessboard Chessboard, uint8_t position, uint8_t alliance, bool onlyCaptures) {
        Bitboard north = calculateAttackBeam(Chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::NORTH, false);
        Bitboard south = calculateAttackBeam(Chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::SOUTH, true);
        Bitboard west = calculateAttackBeam(Chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::WEST, true);
        Bitboard east = calculateAttackBeam(Chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::EAST, false);
        return north | south | west | east;
    }

    Bitboard generateQueenMask(Chessboard chessboard, uint8_t position, uint8_t alliance, bool onlyCaptures) {
        Bitboard bishopMask = calculateBishopMask(chessboard, position, alliance, onlyCaptures);
        Bitboard rookMask = calculateRookMask(chessboard, position, alliance, onlyCaptures);
        return bishopMask | rookMask;
    }

    Bitboard generateKingMask(Chessboard chessboard, uint8_t position, uint8_t alliance, bool onlyCaptures) {
        if (onlyCaptures) {
            uint8_t oppositeAlliance = chessboard.invert(alliance);
            return KingMasks::masks[position] & chessboard.getSideBitboard(oppositeAlliance);
        }
        return KingMasks::masks[position] & chessboard.getInvertedSideBitboard(alliance);
    }

    Bitboard calculateAttackBeam(Chessboard Chessboard, uint8_t position, uint8_t alliance, bool onlyCaptures, uint8_t direction, bool bitScanReverse) {
        Bitboard blockers = SlidersMasks::masks[position][direction] & Chessboard.getAllBitboard();
        if (blockers == 0) {
            if (onlyCaptures) {
                return 0;
            }
            return SlidersMasks::masks[position][direction];
        }

        uint8_t blockingSquare;
        if (bitScanReverse) {
            blockingSquare = BitboardOperations::bitScanReverse(blockers);
        }
        else {
            blockingSquare = BitboardOperations::bitScanForward(blockers);
        }

        Bitboard moves;
        if (onlyCaptures) {
            moves = 0;
        }
        else {
            moves = SlidersMasks::masks[position][direction] ^ SlidersMasks::masks[blockingSquare][direction];
        }

        if (BitboardOperations::get_bit(Chessboard.getSideBitboard(alliance), blockingSquare)) {
            moves = BitboardOperations::set_0(moves, blockingSquare);
        }
        else {
            moves = BitboardOperations::set_1(moves, blockingSquare);
        }

        return moves;
    }
};
