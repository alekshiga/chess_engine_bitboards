#include <bit>
#include <string>

#pragma once

typedef uint64_t Bitboard;

class Chessboard {
public:
	Bitboard chessboard;
	Bitboard emptyBoard;

	Bitboard whitePieces;
	Bitboard blackPieces;

	Bitboard whitePawns;
	Bitboard whiteKnights;
	Bitboard whiteBishops;
	Bitboard whiteRooks;
	Bitboard whiteQueen;
	Bitboard whiteKing;

	Bitboard blackPawns;
	Bitboard blackKnights;
	Bitboard blackBishops;
	Bitboard blackRooks;
	Bitboard blackQueen;
	Bitboard blackKing;

	Chessboard() = default;

	void updateBitboards() {
		this->whitePieces = this->whitePawns |
			this->whiteKnights |
			this->whiteBishops |
			this->whiteRooks |
			this->whiteQueen |
			this->whiteKing;

		this->blackPieces = this->blackPawns |
			this->whiteKnights |
			this->whiteBishops |
			this->whiteRooks |
			this->whiteQueen |
			this->whiteKing;
		this->chessboard = this->blackPieces | this->whitePieces;
		this->emptyBoard = ~this->chessboard;
	}

	void setPiecesFEN(const std::string& fenNotation) {
		uint8_t x = 0;
		uint8_t y = 7;
		for (auto character : fenNotation) {
			if (character == '/') {
				x = 0;
				y = y - 1;
			}
			else if (std::isdigit(character)) {
				x = x + character - '0';
			}
			else {
				if (std::isupper(character)) {
					character = std::tolower(character);
					switch (character) {
					case 'p': BitboardOperations::set_1(this->whitePawns, y * 8 + x); break;
					case 'n': BitboardOperations::set_1(this->whiteKnights, y * 8 + x); break;
					case 'b': BitboardOperations::set_1(this->whiteBishops, y * 8 + x); break;
					case 'r': BitboardOperations::set_1(this->whiteRooks, y * 8 + x); break;
					case 'q': BitboardOperations::set_1(this->whiteQueen, y * 8 + x); break;
					case 'k': BitboardOperations::set_1(this->whiteKing, y * 8 + x); break;
					}
				}
				else {
					switch (character) {
					case 'p': BitboardOperations::set_1(this->blackPawns, y * 8 + x); break;
					case 'n': BitboardOperations::set_1(this->blackKnights, y * 8 + x); break;
					case 'b': BitboardOperations::set_1(this->blackBishops, y * 8 + x); break;
					case 'r': BitboardOperations::set_1(this->blackRooks, y * 8 + x); break;
					case 'q': BitboardOperations::set_1(this->blackQueen, y * 8 + x); break;
					case 'k': BitboardOperations::set_1(this->blackKing, y * 8 + x); break;
					}
				}
				x = x + 1;
			}
		}
		this->updateBitboards();
	}
};