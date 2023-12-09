#include <iostream>
#include "Bitboards.h"
#include "BitboardOperations.h"

int main()
{
    Chessboard chessboard = Chessboard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    std::cout << toString(chessboard);
}
