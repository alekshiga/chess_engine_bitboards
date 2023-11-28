#pragma once
#include <bit>
#define ZERO 0b00000000;
typedef uint64_t Bitboard;


class Chessboard {
public:
	Bitboard chessboard = ZERO;

	Bitboard whitePieces = ZERO;
	Bitboard blackPieces = ZERO;

	Bitboard whitePawns = ZERO;
	Bitboard whiteKnights = ZERO;
	Bitboard whiteBishops = ZERO;
	Bitboard whiteRooks = ZERO;
	Bitboard whiteQueen = ZERO;
	Bitboard whiteKing = ZERO;

	Bitboard blackPawns = ZERO;
	Bitboard blackKnights = ZERO;
	Bitboard blackBishops = ZERO;
	Bitboard blackRooks = ZERO;
	Bitboard blackQueen = ZERO;
	Bitboard blackKing = ZERO;

	Chessboard() {
		for (int i = 0; i < 8; ++i) {
			whitePawns = whitePawns | (1ULL << 8 + i);
			blackPawns = blackPawns | (1ULL << 48 + i);
		}
		whiteKnights = whiteKnights | (1ULL << 1);
		whiteKnights = whiteKnights | (1ULL << 6);
		whiteBishops = whiteBishops | (1ULL << 2);
		whiteBishops = whiteBishops | (1ULL << 5);
		whiteRooks = whiteRooks | (1ULL << 0);
		whiteRooks = whiteRooks | (1ULL << 7);
		whiteQueen = whiteQueen | (1ULL << 4);
		whiteKing = whiteKing | (1ULL << 3);
		blackKnights = blackKnights | (1ULL << 62);
		blackKnights = blackKnights | (1ULL << 57);
		blackBishops = blackBishops | (1ULL << 58);
		blackBishops = blackBishops | (1ULL << 61);
		blackRooks = blackRooks | (1ULL << 56);
		blackRooks = blackRooks | (1ULL << 63);
		blackQueen = blackQueen | (1ULL << 60);
		blackKing = blackKing | (1ULL << 59);
		whitePieces = whitePawns | whiteKing | whiteQueen | whiteRooks | whiteBishops | whiteKnights;
		blackPieces = blackPawns | blackKing | blackQueen | blackRooks | blackBishops | blackKnights;
		chessboard = whitePieces | blackPieces;
	}

	static constexpr void set_1(Bitboard& bb, uint8_t square) {
		bb = bb | (1ULL << square);
	}
	static constexpr void set_0(Bitboard& bb, uint8_t square) {
		bb = bb & (~(1ULL << square));
	}

	static constexpr bool get_bit(Bitboard bb, uint8_t square) {
		return (bb & (1ULL << square));
	}

	static constexpr uint8_t count_1(Bitboard bb) {
		return std::popcount(bb);
	}

	void toBoard(Bitboard bitboard) {
		for (int i = 63; i >= 0; --i) {
			std::cout << ((bitboard >> i) & 1) << " ";
			if (i % 8 == 0) {
				std::cout << std::endl;
			}
		}
		std::bitset<64>(bitboard).to_string();
	}

};