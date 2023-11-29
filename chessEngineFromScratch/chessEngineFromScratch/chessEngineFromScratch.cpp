#include <iostream>
#include "Bitboards.h"
#include "BitboardOperations.h"

int main()
{
    Chessboard chessboard;
    chessboard.setPiecesFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    std::cout << chessboard.toString();
}
