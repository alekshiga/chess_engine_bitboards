#pragma once


class Move {
    
    int startPosition;
    int destinationCoordinate;

    int attackingPieceType;
    int attackingPieceAlliance;

    int attackedPieceType;
    int attackedPieceAlliance;

    int flag;
    
    Move();
    
    Move(int startPosition,
        int destinationCoordinate,
        int attackingPieceType,
        int attackingPieceAlliance,
        int attackedPieceType,
        int attackedPieceAlliance,
        int flag = Flag::regular);

    friend bool operator==(Move left, Move right);
};

namespace Flag {
    static constexpr int regular = 0;

    static constexpr int pawnJump = 1;
    static constexpr int enPassantMove = 2;
    static constexpr int whiteShortCastling = 3;
    static constexpr int whiteLongSideCastle = 4;

    static constexpr int blackShortSideCastle = 5;
    static constexpr int blackLongSideCastle = 6;

    static constexpr int promotionToKnight = 7;
    static constexpr int promotionToBishop = 8;
    static constexpr int promotionToRook = 9;
    static constexpr int promotionToQueen = 10;
};