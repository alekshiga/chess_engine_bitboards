#include <bit>
#include "Bitboards.h"
#include "KnightMask.h"
#include "KingMask.h"
#include "SlidersMask.h"
#include "BitboardOperations.h"

#pragma once

class PseudoLegalMove {
public:
    PseudoLegalMove() = default;

    static Bitboard calculateKnightMask(Chessboard chessboard, uint8_t position, uint8_t alliance, bool onlyCaptures) {
        if (onlyCaptures) {
            uint8_t oppositeAlliance = chessboard.invert(alliance);
            return KnightMasks::masks[position] & chessboard.getSideBitboard(oppositeAlliance);
        }
        return KnightMasks::masks[position] & chessboard.getInvertedSideBitboard(alliance);
    }
    static Bitboard calculateBishopMask(Chessboard chessboard, uint8_t position, uint8_t alliance, bool onlyCaptures) {
        Bitboard northWest = calculateAttackBeam(chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::NORTH_WEST, false);
        Bitboard northEast = calculateAttackBeam(chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::NORTH_EAST, false);
        Bitboard southWast = calculateAttackBeam(chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::SOUTH_WEST, true);
        Bitboard southEast = calculateAttackBeam(chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::SOUTH_EAST, true);
        return northWest | northEast | southWast | southEast;
    }

    static Bitboard calculateRookMask(Chessboard Chessboard, uint8_t position, uint8_t alliance, bool onlyCaptures) {
        Bitboard north = calculateAttackBeam(Chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::NORTH, false);
        Bitboard south = calculateAttackBeam(Chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::SOUTH, true);
        Bitboard west = calculateAttackBeam(Chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::WEST, true);
        Bitboard east = calculateAttackBeam(Chessboard, position, alliance, onlyCaptures, SlidersMasks::DIRECTION::EAST, false);
        return north | south | west | east;
    }

    static Bitboard calculateQueenMask(Chessboard chessboard, uint8_t position, uint8_t alliance, bool onlyCaptures) {
        Bitboard bishopMask = calculateBishopMask(chessboard, position, alliance, onlyCaptures);
        Bitboard rookMask = calculateRookMask(chessboard, position, alliance, onlyCaptures);
        return bishopMask | rookMask;
    }

    static Bitboard calculateKingMask(Chessboard chessboard, uint8_t position, uint8_t alliance, bool onlyCaptures) {
        if (onlyCaptures) {
            uint8_t oppositeAlliance = chessboard.invert(alliance);
            return KingMasks::masks[position] & chessboard.getSideBitboard(oppositeAlliance);
        }
        return KingMasks::masks[position] & chessboard.getInvertedSideBitboard(alliance);
    }

    static Bitboard calculatePawnMoveMask(Chessboard chessboard, uint8_t alliance) {
        if (alliance == SIDE::WHITE) {
            return (chessboard.pieceBitboards[SIDE::WHITE][PIECE::PAWN] << 8) & chessboard.emptyBoard;
        }
        else return (chessboard.pieceBitboards[SIDE::BLACK][PIECE::PAWN] >> 8) & chessboard.emptyBoard;
    }

    static Bitboard calculatePawnJumpMask(Chessboard chessboard, uint8_t alliance) {
        Bitboard pawnMove = calculatePawnMoveMask(chessboard, alliance);
        if (alliance == SIDE::WHITE) {
            return (pawnMove & BitboardRows::rows[2] << 8) & chessboard.emptyBoard;
        }
        else return (pawnMove & BitboardRows::rows[5] >> 8) & chessboard.emptyBoard;
    }

    static Bitboard calculatePawnLeftAttackMove(Chessboard chessboard, uint8_t alliance, bool includeAllPossibleCaptures) {
        if (alliance == SIDE::WHITE) {
            Bitboard mask = (chessboard.pieceBitboards[SIDE::WHITE][PIECE::PAWN] << 7) & BitboardColumns::invertedColumns[7];
            if (!includeAllPossibleCaptures) mask = mask & chessboard.sideBitboards[SIDE::BLACK];
            return mask;
        }
        else {
            Bitboard mask = (chessboard.pieceBitboards[SIDE::BLACK][PIECE::PAWN] >> 9) & BitboardColumns::invertedColumns[7];
            if (!includeAllPossibleCaptures) mask = mask & chessboard.sideBitboards[SIDE::WHITE];
            return mask;
        }
    }

    static Bitboard calculatePawnRightAttackMove(Chessboard chessboard, uint8_t alliance, bool includeAllPossibleCaptures) {
        if (alliance == SIDE::WHITE) {
            Bitboard mask = (chessboard.pieceBitboards[SIDE::WHITE][PIECE::PAWN] << 9) & BitboardColumns::invertedColumns[0];
            if (!includeAllPossibleCaptures) mask = mask & chessboard.sideBitboards[SIDE::BLACK];
            return mask;
        }
        else {
            Bitboard mask = (chessboard.pieceBitboards[SIDE::BLACK][PIECE::PAWN] >> 7) & BitboardColumns::invertedColumns[0];
            if (!includeAllPossibleCaptures) mask = mask & chessboard.sideBitboards[SIDE::WHITE];
            return mask;
        }
    }

    static Bitboard calculateAttackBeam(Chessboard chessboard, uint8_t position, uint8_t alliance, bool onlyCaptures, uint8_t direction, bool bitScanReverse) {
        Bitboard blockingPieces = SlidersMasks::masks[position][direction] ^ chessboard.chessBoard;
        if (blockingPieces == 0) {
            if (onlyCaptures) {
                return 0;
            }
            return SlidersMasks::masks[position][direction];
        }

        uint8_t blockingSquare;
        if (bitScanReverse) {
            blockingSquare = BitboardOperations::bitScanReverse(blockingPieces);
        }
        else {
            blockingSquare = BitboardOperations::bitScanForward(blockingPieces);
        }

        Bitboard moves;
        if (onlyCaptures) {
            moves = 0;
        }
        else {
            moves = SlidersMasks::masks[position][direction] & SlidersMasks::masks[blockingSquare][direction];
        }

        if (BitboardOperations::get_bit(chessboard.getSideBitboard(alliance), blockingSquare)) {
            moves = BitboardOperations::set_0(moves, blockingSquare);
        }
        else {
            moves = BitboardOperations::set_1(moves, blockingSquare);
        }
        return moves;
    }

    static bool leavesInCheck(Chessboard chessboard, uint8_t position, uint8_t alliance) {

    uint8_t oppositeAlliance = chessboard.invert(alliance);

        Bitboard pawnsLeftAttacks = calculatePawnLeftAttackMove(chessboard, oppositeAlliance, true);
        Bitboard pawnRightAttacks = calculatePawnRightAttackMove(chessboard, oppositeAlliance, true);
        Bitboard pawnAttackMoves = pawnsLeftAttacks | pawnRightAttacks;

        if (BitboardOperations::get_bit(pawnAttackMoves, position)) return true;
        if (calculateBishopMask(chessboard, position, alliance, true) &
            chessboard.getPieceBitboard(oppositeAlliance, PIECE::BISHOP)) return true;
        if (calculateKnightMask(chessboard, position, alliance, true) &
            chessboard.getPieceBitboard(oppositeAlliance, PIECE::KNIGHT)) return true;
        if (calculateRookMask(chessboard, position, alliance, true) &
            chessboard.getPieceBitboard(oppositeAlliance, PIECE::ROOK)) return true;
        if (calculateQueenMask(chessboard, position, alliance, true) &
            chessboard.getPieceBitboard(oppositeAlliance, PIECE::QUEEN)) return true;
        if (calculateKingMask(chessboard, position, alliance, true) &
            chessboard.getPieceBitboard(oppositeAlliance, PIECE::KING)) return true;
        return false;
    }
};
