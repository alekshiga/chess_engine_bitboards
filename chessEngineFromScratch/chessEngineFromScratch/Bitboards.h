#include <bit>
#include <string>
#include "BitboardOperations.h"

constexpr auto zero = 0x0;

#pragma once

typedef uint64_t Bitboard;

enum PIECE {
	PAWN = 0,
	KNIGHT = 1,
	BISHOP = 2,
	ROOK = 3,
	QUEEN = 4,
	KING = 5
};


enum SIDE {
	WHITE = 0,
	BLACK = 1
};


class Chessboard {
public:

	std::array<std::array<Bitboard, 6>, 2> pieceBitboards{};
	std::array<Bitboard, 2> sideBitboards{};
	std::array<Bitboard, 2> invertedSideBitboards{};
	Bitboard chessBoard;
	Bitboard emptyBoard;

	Chessboard() = default;

	Chessboard(const std::string& fenNotation) {
		uint8_t x = 0;
		uint8_t y = 7;

		uint8_t side;

		for (auto buff : fenNotation) {
			if (buff == '/') {
				x = 0;
				y = y - 1;
			}
			else if (std::isdigit(buff)) {
				x = x + buff - '0';
			}
			else {
				if (std::isupper(buff)) {
					buff = (char)std::tolower(buff);
					side = SIDE::WHITE;
				}
				else {
					side = SIDE::BLACK;
				}

				switch (buff) {
				case 'p':
					this->pieceBitboards[side][PIECE::PAWN] = BitboardOperations::set_1(this->pieceBitboards[side][PIECE::PAWN], y * 8 + x);
					break;
				case 'n':
					this->pieceBitboards[side][PIECE::KNIGHT] = BitboardOperations::set_1(this->pieceBitboards[side][PIECE::KNIGHT], y * 8 + x);
					break;
				case 'b':
					this->pieceBitboards[side][PIECE::BISHOP] = BitboardOperations::set_1(this->pieceBitboards[side][PIECE::BISHOP], y * 8 + x);
					break;
				case 'r':
					this->pieceBitboards[side][PIECE::ROOK] = BitboardOperations::set_1(this->pieceBitboards[side][PIECE::ROOK], y * 8 + x);
					break;
				case 'q':
					this->pieceBitboards[side][PIECE::QUEEN] = BitboardOperations::set_1(this->pieceBitboards[side][PIECE::QUEEN], y * 8 + x);
					break;
				case 'k':
					this->pieceBitboards[side][PIECE::KING] = BitboardOperations::set_1(this->pieceBitboards[side][PIECE::KING], y * 8 + x);
					break;
				}
				x++;
			}
		}
		this->updateBitboards();
	}

	friend std::string toString(Chessboard pieces) {
		std::string result;
		for (int8_t y = 7; y >= 0; y = y - 1) {
			for (uint8_t x = 0; x < 8; x = x + 1) {
				uint8_t index = y * 8 + x;
				if (BitboardOperations::get_bit(pieces.pieceBitboards[SIDE::WHITE][PIECE::PAWN], index)) {
					result += "P ";
				}
				else if (BitboardOperations::get_bit(pieces.pieceBitboards[SIDE::WHITE][PIECE::KNIGHT], index)) {
					result += "K ";
				}
				else if (BitboardOperations::get_bit(pieces.pieceBitboards[SIDE::WHITE][PIECE::BISHOP], index)) {
					result += "B ";
				}
				else if (BitboardOperations::get_bit(pieces.pieceBitboards[SIDE::WHITE][PIECE::ROOK], index)) {
					result += "R ";
				}
				else if (BitboardOperations::get_bit(pieces.pieceBitboards[SIDE::WHITE][PIECE::QUEEN], index)) {
					result += "Q ";
				}
				else if (BitboardOperations::get_bit(pieces.pieceBitboards[SIDE::WHITE][PIECE::KING], index)) {
					result += "K ";
				}
				else if (BitboardOperations::get_bit(pieces.pieceBitboards[SIDE::BLACK][PIECE::PAWN], index)) {
					result += "p ";
				}
				else if (BitboardOperations::get_bit(pieces.pieceBitboards[SIDE::BLACK][PIECE::KNIGHT], index)) {
					result += "n ";
				}
				else if (BitboardOperations::get_bit(pieces.pieceBitboards[SIDE::BLACK][PIECE::BISHOP], index)) {
					result += "b ";
				}
				else if (BitboardOperations::get_bit(pieces.pieceBitboards[SIDE::BLACK][PIECE::ROOK], index)) {
					result += "r ";
				}
				else if (BitboardOperations::get_bit(pieces.pieceBitboards[SIDE::BLACK][PIECE::QUEEN], index)) {
					result += "q ";
				}
				else if (BitboardOperations::get_bit(pieces.pieceBitboards[SIDE::BLACK][PIECE::KING], index)) {
					result += "k ";
				}
			}
			if (y != 0) {
				result += "\n";
			}
		}
		return result;
	}

	void updateBitboards() {
		this->sideBitboards[SIDE::WHITE] = this->pieceBitboards[SIDE::WHITE][PIECE::PAWN] |
			this->pieceBitboards[SIDE::WHITE][PIECE::KNIGHT] |
			this->pieceBitboards[SIDE::WHITE][PIECE::BISHOP] |
			this->pieceBitboards[SIDE::WHITE][PIECE::ROOK] |
			this->pieceBitboards[SIDE::WHITE][PIECE::QUEEN] |
			this->pieceBitboards[SIDE::WHITE][PIECE::KING];

		this->sideBitboards[SIDE::BLACK] = this->pieceBitboards[SIDE::BLACK][PIECE::PAWN] |
			this->pieceBitboards[SIDE::BLACK][PIECE::KNIGHT] |
			this->pieceBitboards[SIDE::BLACK][PIECE::BISHOP] |
			this->pieceBitboards[SIDE::BLACK][PIECE::ROOK] |
			this->pieceBitboards[SIDE::BLACK][PIECE::QUEEN] |
			this->pieceBitboards[SIDE::BLACK][PIECE::KING];

		this->invertedSideBitboards[SIDE::WHITE] = ~this->sideBitboards[SIDE::WHITE];
		this->invertedSideBitboards[SIDE::BLACK] = ~this->sideBitboards[SIDE::BLACK];

		this->chessBoard = this->sideBitboards[SIDE::WHITE] | this->sideBitboards[SIDE::BLACK];
		this->emptyBoard = ~this->chessBoard;
	}

	void setPieceBitboard(uint8_t side, uint8_t piece, Bitboard pieceBitboard) {
		this->pieceBitboards[side][piece] = pieceBitboard;
	}

	std::array<std::array<Bitboard, 6>, 2> getAllPieceBitboards() const {
		return this->pieceBitboards;
	}

	Bitboard getPieceBitboard(uint8_t side, uint8_t piece) const {
		return this->pieceBitboards[side][piece];
	}

	Bitboard getSideBitboard(uint8_t side) const {
		return this->sideBitboards[side];
	}

	Bitboard getInvertedSideBitboard(uint8_t side) const {
		return this->invertedSideBitboards[side];
	}

	Bitboard getAllBitboard() const {
		return this->chessBoard;
	}

	Bitboard getEmptyBitboard() const {
		return this->emptyBoard;
	}

	static uint8_t invert(uint8_t alliance) {
		return !alliance;
	}
};