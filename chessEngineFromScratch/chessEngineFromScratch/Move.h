#pragma once

class Move {
    
    int startPosition;
    int destinationCoordinate;

    int attackingPieceType;
    int attackingPieceAlliance;

    int attackedPieceType;
    int attackedPieceAlliance;

    int flag;
    
    Move() = default;
    
    Move(int startPosition,
        int destinationCoordinate,
        int attackingPieceType,
        int attackingPieceAlliance,
        int attackedPieceType,
        int attackedPieceAlliance,
        int flag) 
    {
        this->startPosition = startPosition;
        this->destinationCoordinate = destinationCoordinate;
        this->attackingPieceType = attackingPieceType;
        this->attackingPieceAlliance = attackingPieceAlliance;
        this->attackedPieceType = attackedPieceType;
        this->attackedPieceAlliance = attackedPieceAlliance;
        this->flag = flag;
    }

    friend bool operator==(Move left, Move right) {

    }

    int getStartPosition() const {
        return this->startPosition;
    }

    int getDestinationCoordinate() const {
        return this->destinationCoordinate;
    }

    int getAttackingPieceType() const {
        return this->attackingPieceType;
    }

    int getAttackingPieceAlliance() const {
        return this->attackingPieceAlliance;
    }

    int getAttackedPieceType() const {
        return this->attackedPieceType;
    }

    int getAttackedPieceAlliance() const {
        return this->attackedPieceAlliance;
    }

    void setStartPosition(int startPosition) {
        this->startPosition = startPosition;
    }

    void setDestinationCoordinate(int destinationCoordinate) {
        this->destinationCoordinate = destinationCoordinate;
    }

    void setAttackingPieceType(int attackingPieceType) {
        this->attackingPieceType = attackingPieceType;
    }

    void setAttackingPieceAlliance(int attackingPieceAlliance) {
        this->attackingPieceAlliance = attackingPieceAlliance;
    }

    void setAttackedPieceType(int attackedPieceType) {
        this->attackedPieceType = attackedPieceType;
    }

    void setAttackedPieceAlliance(int attackedPieceAlliance) {
        this->attackedPieceAlliance = attackedPieceAlliance;
    }

    void setFlag(int flag) {
        this->flag = flag;
    }

    enum Flag {
        regular,

        pawnJump,
        enPassantMove,
        whiteShortCastling,
        whiteLongSideCastle,

        blackShortSideCastle,
        blackLongSideCastle,

        promotionToKnight,
        promotionToBishop,
        promotionToRook,
        promotionToQueen
    };
};
