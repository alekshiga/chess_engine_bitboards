#include <cstdint>
#include <array>
#include "Bitboards.h"

#pragma once

namespace ZobristHashConstants {
    namespace PRNG {
        static constexpr Bitboard SEED = 0x98f107;
        static constexpr Bitboard MULTIPLIER = 0x71abc9;
        static constexpr Bitboard SUMMAND = 0xff1b3f;
        static consteval Bitboard nextRandomNumber(Bitboard previous) {
            return MULTIPLIER * previous + SUMMAND;
        }
    }
    static consteval std::array<std::array<std::array<Bitboard, 6>, 2>, 64> calculateConstants() {
        std::array<std::array<std::array<Bitboard, 6>, 2>, 64> constants{};

        uint64_t previous = PRNG::SEED;

        for (uint8_t square = 0; square < 64; square = square + 1) {
            for (uint8_t side = 0; side < 2; side = side + 1) {
                for (uint8_t type = 0; type < 6; type = type + 1) {
                    previous = PRNG::nextRandomNumber(previous);
                    constants[square][side][type] = previous;
                }
            }
        }
        return constants;
    }
    static constexpr std::array<std::array<std::array<Bitboard, 6>, 2>, 64> CONSTANTS = calculateConstants();
    static constexpr Bitboard BLACK_MOVE = PRNG::nextRandomNumber(CONSTANTS[63][1][5]);
    static constexpr Bitboard WL_CASTLING = PRNG::nextRandomNumber(BLACK_MOVE);
    static constexpr Bitboard WS_CASTLING = PRNG::nextRandomNumber(WL_CASTLING);
    static constexpr Bitboard BL_CASTLING = PRNG::nextRandomNumber(WS_CASTLING);
    static constexpr Bitboard BS_CASTLING = PRNG::nextRandomNumber(BL_CASTLING);
}