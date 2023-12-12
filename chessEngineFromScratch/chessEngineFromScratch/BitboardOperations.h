#include <bit>
#include <bitset>
#include "string"
#include <array>

#pragma once

typedef uint64_t Bitboard;

namespace BitboardOperations {

	static constexpr Bitboard set_1(Bitboard bb, uint8_t square) {
		return bb | (1ULL << square);
	}
	static constexpr Bitboard set_0(Bitboard bb, uint8_t square) {
		return bb = bb & (~(1ULL << square));
	}

	static constexpr bool get_bit(Bitboard bb, uint8_t square) {
		return (bb & (1ULL << square));
	}

	static constexpr uint8_t count_1(Bitboard bb) {
		return std::popcount(bb);
	}

	void toBoard(Bitboard bitboard) {
		std::string board;
		for (int i = 63; i >= 0; --i) {
			board += ((bitboard >> i) & 1) + " ";
			if (i % 8 == 0) {
				board += "\n";
			}
		}
	}

	static constexpr std::array<uint64_t, 64> bitScanTable = {
			0, 47,  1, 56, 48, 27,  2, 60,
			57, 49, 41, 37, 28, 16,  3, 61,
			54, 58, 35, 52, 50, 42, 21, 44,
			38, 32, 29, 23, 17, 11,  4, 62,
			46, 55, 26, 59, 40, 36, 15, 53,
			34, 51, 20, 43, 31, 22, 10, 45,
			25, 39, 14, 33, 19, 30,  9, 24,
			13, 18,  8, 12,  7,  6,  5, 63
	};


	static constexpr Bitboard bitScanForward(Bitboard bb) {
		return BitboardOperations::bitScanTable[((bb ^ (bb - 1)) * 0x03f79d71b4cb0a89) >> 58];
	}
	static constexpr Bitboard bitScanReverse(Bitboard bb) {
		bb = bb | (bb >> 1);
		bb = bb | (bb >> 2);
		bb = bb | (bb >> 4);
		bb = bb | (bb >> 8);
		bb = bb | (bb >> 16);
		bb = bb | (bb >> 32);

		return BitboardOperations::bitScanTable[(bb * 0x03f79d71b4cb0a89) >> 58];
	}

}

namespace BitboardRows {

	static consteval std::array<Bitboard, 8> calculateRows() {
		std::array<Bitboard, 8> rows{};
		for (uint8_t y = 0; y < 8; y = y + 1) {
			for (uint8_t x = 0; x < 8; x = x + 1) BitboardOperations::set_1(rows[y], y * 8 + x);
		} return rows;
	}

	static constexpr std::array<Bitboard, 8> rows = calculateRows();

	static consteval std::array<Bitboard, 8> calculateInvertedRows() {
		std::array<Bitboard, 8> inversionRows{};
		for (uint8_t i = 0; i < 8; i = i + 1) inversionRows[i] = ~rows[i];
		return inversionRows;
	}

	static constexpr std::array<Bitboard, 8> inversionRows = calculateInvertedRows();
}


namespace BitboardColumns {

	static consteval std::array<Bitboard, 8> calculateColumns() {
		std::array<Bitboard, 8> columns{};
		for (uint8_t x = 0; x < 8; x = x + 1) {
			for (uint8_t y = 0; y < 8; y = y + 1) BitboardOperations::set_1(columns[x], y * 8 + x);
		} return columns;
	}

	static constexpr std::array<Bitboard, 8> columns = BitboardColumns::calculateColumns();

	static consteval std::array<Bitboard, 8> calculateInvertedColumns() {
		std::array<Bitboard, 8> invertedColumns{};
		for (uint8_t i = 0; i < 8; i = i + 1) invertedColumns[i] = ~columns[i];
		return invertedColumns;
	}
	static constexpr std::array<Bitboard, 8> invertedColumns = BitboardColumns::calculateInvertedColumns();
}