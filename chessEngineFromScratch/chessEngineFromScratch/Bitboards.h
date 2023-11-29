#include <bit>
#include <string>
#include "BitboardOperations.h"

constexpr auto zero = 0x0;;

#pragma once

typedef uint64_t Bitboard;

class Chessboard {
public:
	Bitboard chessboard = zero;
	Bitboard emptyBoard = zero;

	Bitboard whitePieces = zero;
	Bitboard blackPieces = zero;

	Bitboard whitePawns = zero;
	Bitboard whiteKnights = zero;
	Bitboard whiteBishops = zero;
	Bitboard whiteRooks = zero;
	Bitboard whiteQueen = zero;
	Bitboard whiteKing = zero;

	Bitboard blackPawns = zero;
	Bitboard blackKnights = zero;
	Bitboard blackBishops = zero;
	Bitboard blackRooks = zero;
	Bitboard blackQueen = zero;
	Bitboard blackKing = zero;

	Chessboard() = default;

	void updateBitboards() {
		this->whitePieces = 
			this->whitePawns |
			this->whiteKnights |
			this->whiteBishops |
			this->whiteRooks |
			this->whiteQueen |
			this->whiteKing;

		this->blackPieces = 
			this->blackPawns |
			this->blackKnights |
			this->blackBishops |
			this->blackRooks |
			this->blackQueen |
			this->blackKing;
		this->chessboard = this->blackPieces | this->whitePieces;
		this->emptyBoard = ~this->chessboard;
	}

	void setPiecesFEN(const std::string& fenNotation) {
		int x = 7;
		int y = 7;
		for (auto character : fenNotation) {
			if (character == '/') {
				x = 7;
				y = y - 1;
			}
			else if (std::isdigit(character)) {
				x = x + character - '0';
			}
			else {
				if (std::islower(character)) {
					switch (character) {
					case 'p': BitboardOperations::set_1(this->blackPawns, y * 8 + x); break;
					case 'n': BitboardOperations::set_1(this->blackKnights, y * 8 + x); break;
					case 'b': BitboardOperations::set_1(this->blackBishops, y * 8 + x); break;
					case 'r': BitboardOperations::set_1(this->blackRooks, y * 8 + x); break;
					case 'q': BitboardOperations::set_1(this->blackQueen, y * 8 + x); break;
					case 'k': BitboardOperations::set_1(this->blackKing, y * 8 + x); break;
					}
				}
				else {
					switch (character) {
					case 'P': BitboardOperations::set_1(this->whitePawns, y * 8 + x); break;
					case 'N': BitboardOperations::set_1(this->whiteKnights, y * 8 + x); break;
					case 'B': BitboardOperations::set_1(this->whiteBishops, y * 8 + x); break;
					case 'R': BitboardOperations::set_1(this->whiteRooks, y * 8 + x); break;
					case 'Q': BitboardOperations::set_1(this->whiteQueen, y * 8 + x); break;
					case 'K': BitboardOperations::set_1(this->whiteKing, y * 8 + x); break;
					}
				}
				x--;
			}
		}
		this->updateBitboards();
	}

	std::string toString() {
		std::string board;
		for (int i = 63; i >= 0; --i) {
			board += std::to_string(((this->chessboard >> i) & 1)) + " ";
			if (i % 8 == 0) {
				board += "\n";
			}
		}
		return board;
	}
};