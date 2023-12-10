#include <bit>
#include <array>
#include "Bitboards.h"

#pragma once

static constexpr int NUM_TILES = 64;

namespace SlidersMasks {

    static constexpr std::array<std::array<Bitboard, 8>, 64> masks = calculateSliderMasks();

    enum DIRECTION {
        NORTH = 1,
        SOUTH = 2,
        WEST = 3,
        EAST = 4,
        NORTH_WEST = 5,
        NORTH_EAST = 6,
        SOUTH_WEST = 7,
        SOUTH_EAST = 8,
    };

    static consteval Bitboard calculateMask(uint8_t position, uint8_t direction) {
        Bitboard mask = 0;
        int8_t x = position % 8;
        int8_t y = position / 8;

        for (; ;) {
            switch (direction) {
            case SlidersMasks::DIRECTION::NORTH:
                y++;
                break;
            case SlidersMasks::DIRECTION::SOUTH:
                y--;
                break;
            case SlidersMasks::DIRECTION::WEST:
                x--;
                break;
            case SlidersMasks::DIRECTION::EAST:
                x++;
                break;
            case SlidersMasks::DIRECTION::NORTH_WEST:
                y++;
                x--;
                break;
            case SlidersMasks::DIRECTION::NORTH_EAST:
                y++;
                x++;
                break;
            case SlidersMasks::DIRECTION::SOUTH_WEST:
                y--;
                x--;
                break;
            case SlidersMasks::DIRECTION::SOUTH_EAST:
                y--;
                x++;
                break;
            }
            if (x > 7 or x < 0 or y > 7 or y < 0) break;
            BitboardOperations::set_1(mask, y * 8 + x);
        }
        return mask;
    }

    static consteval std::array<std::array<Bitboard, 8>, 64> calculateSliderMasks() {
        std::array<std::array<Bitboard, 8>, 64> masks;
        for (uint8_t i = 0; i < NUM_TILES; ++i) {
            for (uint8_t j = 0; j < 8; ++j) {
                masks[i][j] = SlidersMasks::calculateMask(i, j);
            }
        }
        return masks;
    }
}