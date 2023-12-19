#include <bit>
#include <array>
#include "Bitboards.h"

#pragma once

namespace KnightMasks {

    static consteval uint8_t absoluteSubstract(uint8_t left, uint8_t right) {
        if (left >= right) {
            return left - right;
        }
        else {
            return right - left;
        }
    }
    static consteval std::array<Bitboard, 64> calculateKnightMasks() {
        std::array<Bitboard, 64> masks{};
        uint8_t x;
        uint8_t y;
        for (uint8_t x0 = 0; x0 < 8; ++x0) {
            for (uint8_t y0 = 0; y0 < 8; ++y0) {
                for (uint8_t x1 = 0; x1 < 8; ++x1) {
                    for (uint8_t y1 = 0; y1 < 8; ++y1) {
                        x = absoluteSubstract(x0, x1);
                        y = absoluteSubstract(y0, y1);
                        if ((x == 2 and y == 1) or (x == 1 and y == 2)) {
                            masks[y0 * 8 + x0] = BitboardOperations::set_1(masks[y0 * 8 + x0], y1 * 8 + x1);
                        }
                    }
                }
            }
        }
        return masks;
    }

    static constexpr std::array<Bitboard, 64> masks = calculateKnightMasks();
}