#pragma once

class Move {
public:
    uint8_t startPosition;
    uint8_t destinationCoordinate;

    uint8_t attackerPieceType;
    uint8_t attackerPieceAlliance;

    uint8_t attackedPieceType;
    uint8_t attackedPieceAlliance;

    uint8_t flag;
    
    Move() = default;
    
    Move(uint8_t startPosition,
        uint8_t destinationCoordinate,
        uint8_t attackingPieceType,
        uint8_t attackingPieceAlliance,
        uint8_t attackedPieceType,
        uint8_t attackedPieceAlliance,
        uint8_t flag) 
    {
        this->startPosition = startPosition;
        this->destinationCoordinate = destinationCoordinate;
        this->attackerPieceType = attackingPieceType;
        this->attackerPieceAlliance = attackingPieceAlliance;
        this->attackedPieceType = attackedPieceType;
        this->attackedPieceAlliance = attackedPieceAlliance;
        this->flag = flag;
    }

    void setStartPosition(uint8_t startPosition) {
        this->startPosition = startPosition;
    }

    void setDestinationCoordinate(uint8_t destinationCoordinate) {
        this->destinationCoordinate = destinationCoordinate;
    }

    void setAttackingPieceType(uint8_t attackingPieceType) {
        this->attackerPieceType = attackingPieceType;
    }

    void setAttackingPieceAlliance(uint8_t attackingPieceAlliance) {
        this->attackerPieceAlliance = attackingPieceAlliance;
    }

    void setAttackedPieceType(uint8_t attackedPieceType) {
        this->attackedPieceType = attackedPieceType;
    }

    void setAttackedPieceAlliance(uint8_t attackedPieceAlliance) {
        this->attackedPieceAlliance = attackedPieceAlliance;
    }

    void setFlag(uint8_t flag) {
        this->flag = flag;
    }

    enum Flag {
        regular,

        pawnJump,
        enPassantMove,

        whiteShortCastling,
        whiteLongCastling,

        blackShortCastling,
        blackLongCastling,

        promotionToKnight,
        promotionToBishop,
        promotionToRook,
        promotionToQueen
    };

    static constexpr uint8_t NONE = 255;
};
