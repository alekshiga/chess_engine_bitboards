#include "Bitboards.h"
#include "ZobristHash.h"
#include "RepetitionHistory.h"
#include "Move.h"

#pragma once

class Position {
private:
    Chessboard pieces;
    uint8_t enPassant;

    bool whiteShortCastling;
    bool whiteLongCastling;
    bool blackShortCastling;
    bool blackLongCastling;

    float moveCounter;
    ZobristHash hash;
    float fiftyMovesCounter;
    RepetitionHistory repetitionHistory;

public:
    Position() = default;

    Position(const std::string fenNotation,
        uint8_t enPassantMove,
        bool whiteShortCastling,
        bool whiteLongCastling,
        bool blackShortCastling,
        bool blackLongCastling,
        float moveCounter)
    {
        this->pieces = { fenNotation };
        this->enPassant = enPassant;
        this->whiteShortCastling = whiteShortCastling;
        this->whiteLongCastling = whiteLongCastling;
        this->blackShortCastling = blackShortCastling;
        this->blackLongCastling = blackLongCastling;

        this->moveCounter = moveCounter;
        this->hash = { this->pieces, this->blackToMove(), this->whiteShortCastling, this->whiteLongCastling, this->blackShortCastling, this->moveCounter};
        this->repetitionHistory.addPosition(this->hash);;
        this->fiftyMovesCounter = 0;
    }

    static constexpr uint8_t NONE = 255;

    void move(Move move) {
        this->removePiece(move.startPosition, move.attackerPieceType, move.attackerPieceAlliance);
        this->addPiece(move.destinationCoordinate, move.attackerPieceType, move.attackerPieceAlliance);
        if (move.attackedPieceType != Move::NONE) {
            this->removePiece(move.destinationCoordinate, move.attackedPieceType, move.attackedPieceAlliance);
        }

        switch (move.flag) {
        case Move::Flag::regular:
            break;

        case Move::Flag::pawnJump:
            this->enPassant = (move.startPosition + move.destinationCoordinate) / 2;
            break;

        case Move::Flag::enPassantMove:
            if (move.attackerPieceAlliance == SIDE::WHITE) {
                this->removePiece(move.destinationCoordinate - 8, PIECE::PAWN, SIDE::BLACK);
            }
            else {
                this->removePiece(move.destinationCoordinate + 8, PIECE::PAWN, SIDE::WHITE);
            }
            break;

        case Move::Flag::whiteShortCastling:
            this->removePiece(7, PIECE::ROOK, SIDE::WHITE);
            this->addPiece(5, PIECE::ROOK, SIDE::WHITE);
            break;

        case Move::Flag::whiteLongCastling:
            this->removePiece(0, PIECE::ROOK, SIDE::WHITE);
            this->addPiece(3, PIECE::ROOK, SIDE::WHITE);
            break;

        case Move::Flag::blackShortCastling:
            this->removePiece(63, PIECE::ROOK, SIDE::BLACK);
            this->addPiece(61, PIECE::ROOK, SIDE::BLACK);
            break;

        case Move::Flag::blackLongCastling:
            this->removePiece(56, PIECE::ROOK, SIDE::BLACK);
            this->addPiece(59, PIECE::ROOK, SIDE::BLACK);
            break;

        case Move::Flag::promotionToKnight:
            this->removePiece(move.destinationCoordinate, PIECE::PAWN, move.attackerPieceAlliance);
            this->addPiece(move.destinationCoordinate, PIECE::KNIGHT, move.attackerPieceAlliance);
            break;
        case Move::Flag::promotionToBishop:
            this->removePiece(move.destinationCoordinate, PIECE::PAWN, move.attackerPieceAlliance);
            this->addPiece(move.destinationCoordinate, PIECE::BISHOP, move.attackerPieceAlliance);
            break;
        case Move::Flag::promotionToRook:
            this->removePiece(move.destinationCoordinate, PIECE::PAWN, move.attackerPieceAlliance);
            this->addPiece(move.destinationCoordinate, PIECE::ROOK, move.attackerPieceAlliance);
            break;
        case Move::Flag::promotionToQueen:
            this->removePiece(move.destinationCoordinate, PIECE::PAWN, move.attackerPieceAlliance);
            this->addPiece(move.destinationCoordinate, PIECE::QUEEN, move.attackerPieceAlliance);
            break;
        }
        this->pieces.updateBitboards();
        
        if (move.flag != Move::Flag::pawnJump) {
            this->enPassant = Position::NONE;
        }

        switch (move.startPosition) {
        case 4:
            this->removeWhiteShortCastling();
            this->removeWhiteLongCastling();
            break;
        case 0:
            this->removeWhiteLongCastling();
            break;
        case 7:
            this->removeWhiteShortCastling();
            break;
        case 56:
            this->removeBlackLongCastling();
            break;
        case 60:
            this->removeBlackLongCastling();
            this->removeBlackShortCastling();
            break;
        case 63:
            this->removeBlackShortCastling();
            break;
        }
        this->updateMoveCounter();
        this->updateFiftyMovesCounter(move.attackerPieceType == PIECE::PAWN or move.attackedPieceType != 255);
        if (move.attackerPieceType == PIECE::PAWN or move.attackedPieceType != Move::NONE) {
            this->repetitionHistory.clear();
        }
        this->repetitionHistory.addPosition(this->hash);
    }

    Chessboard getPieces() const {
        return this->pieces;
    }
    uint8_t getEnPassant() const {
        return this->enPassant;
    }
    
    bool getWhiteShortCastling() const {
        return this->whiteShortCastling;
    }

    bool getWhiteLongCastling() const {
        return this->whiteLongCastling;
    }
 
    bool getBlackShortCastling() const {
        return this->blackShortCastling;
    }

    bool getBlackLongCastling() const {
        return this->blackLongCastling;
    }
 
    bool whiteToMove() const {
        return !this->blackToMove();
    }

    bool blackToMove() const {
        return ((this->moveCounter - this->moveCounter) > 0.0001);
    }

    ZobristHash getHash() const {
        return this->hash;
    }

    bool fiftyMovesDraw() const {
        return (this->fiftyMovesCounter == 50);
    }

    bool threeRepetitionDraw() const {
        return (this->repetitionHistory.getRepetitionNumber(this->hash) == 3);
    }

    void addPiece(uint8_t square, uint8_t type, uint8_t side) {
        if (!BitboardOperations::get_bit(this->pieces.getPieceBitboard(side, type), square)) {
            this->pieces.setPieceBitboard(side, type, BitboardOperations::set_1(this->pieces.getPieceBitboard(side, type), square));
            this->hash.invertPiece(square, type, side);
        }
    }

    void removePiece(uint8_t square, uint8_t type, uint8_t side) {
        if (BitboardOperations::get_bit(this->pieces.getPieceBitboard(side, type), square)) {
            this->pieces.setPieceBitboard(side, type, BitboardOperations::set_0(this->pieces.getPieceBitboard(side, type), square));
            this->hash.invertPiece(square, type, side);
        }
    }

    void removeWhiteShortCastling() {
        if (this->whiteShortCastling) {
            this->whiteShortCastling = false;
            this->removeWhiteShortCastling();
        }
    }

    void removeWhiteLongCastling() {
        if (this->whiteLongCastling) {
            this->whiteLongCastling = false;
            this->removeWhiteLongCastling();
        }
    }

    void removeBlackShortCastling() {
        if (this->blackShortCastling) {
            this->blackShortCastling = false;
            this->removeBlackShortCastling();
        }
    }

    void removeBlackLongCastling() {
        if (this->blackLongCastling) {
            this->blackLongCastling = false;
            this->removeBlackLongCastling();
        }
    }

    void updateMoveCounter() {
        this->moveCounter += 0.5f;
        this->hash.invertMove();
    }

    void updateFiftyMovesCounter(bool breakEvent) {
        if (breakEvent) {
            this->fiftyMovesCounter = 0;
        }
        else {
            this->fiftyMovesCounter += 0.5f;
        }
    }
};