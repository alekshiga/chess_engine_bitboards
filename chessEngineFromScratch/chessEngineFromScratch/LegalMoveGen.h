#include "Bitboards.h"
#include "BitboardOperations.h"
#include "MoveList.h"
#include "Move.h"
#include "PseudoLegalMoveGen.h"
#include "Position.h"

#pragma once

class LegalMoveGen {

private:

public:

	void pieceMasksToMoves(Chessboard chessboard, Bitboard mask, uint8_t attackerPiecePosition, uint8_t attackerPieceType, uint8_t attackerPieceAlliance, MoveList& moves) {

		while (mask) {
			uint8_t attackedPiecePosition = BitboardOperations::bitScanForward(mask);
			mask = BitboardOperations::set_0(mask, attackedPiecePosition);

			uint8_t attackedPieceType = Move::NONE;
			for (uint8_t i = 0; i < 6; i++) {
				if (BitboardOperations::get_bit(chessboard.getPieceBitboard(Chessboard::invert(attackerPieceAlliance), i), attackedPiecePosition)) {
					attackedPieceType = i;
					break;
				}
			}
			Move move = { attackerPiecePosition, attackedPiecePosition, attackerPieceType, attackerPieceAlliance, attackedPieceType, Chessboard::invert(attackerPieceAlliance) };
			if (isLegalMove(chessboard, move)) moves.push(move);
		}
	}

	void pawnMasksToMoves(Chessboard chessboard, Bitboard mask, uint8_t attackerPieceAlliance, uint8_t attackerCandidate, bool isAttacked, uint8_t Flag, MoveList& moves) {
		uint8_t attackedPieceType = Move::NONE;

		while (mask) {
			uint8_t attackedPiecePosition = BitboardOperations::bitScanForward(mask);
			mask = BitboardOperations::set_0(mask, attackedPiecePosition);

			if (isAttacked) {
				attackedPieceType = Move::NONE;
				for (uint8_t i = 0; i < 6; i = i + 1) {
					if (BitboardOperations::get_bit(chessboard.getPieceBitboard(Chessboard::invert(attackerPieceAlliance), i), attackedPiecePosition)) {
						attackedPieceType = i;
						break;
					}
				}
			}

			Move move = { (uint8_t)(attackedPiecePosition + attackerCandidate), attackedPiecePosition, PIECE::PAWN, attackerPieceAlliance, attackedPieceType, Chessboard::invert(attackerPieceAlliance), Flag };

			if (isLegalMove(chessboard, move)) {
				if (attackedPiecePosition < 8 or attackedPiecePosition > 55) {
					moves.push({ (uint8_t)(attackedPiecePosition + attackerCandidate), attackedPiecePosition, 0, attackerPieceAlliance, attackedPieceType, Chessboard::invert(attackerPieceAlliance), Move::Flag::promotionToKnight });
					moves.push({ (uint8_t)(attackedPiecePosition + attackerCandidate), attackedPiecePosition, 0, attackerPieceAlliance, attackedPieceType, Chessboard::invert(attackerPieceAlliance), Move::Flag::promotionToBishop });
					moves.push({ (uint8_t)(attackedPiecePosition + attackerCandidate), attackedPiecePosition, 0, attackerPieceAlliance, attackedPieceType, Chessboard::invert(attackerPieceAlliance), Move::Flag::promotionToRook });
					moves.push({ (uint8_t)(attackedPiecePosition + attackerCandidate), attackedPiecePosition, 0, attackerPieceAlliance, attackedPieceType, Chessboard::invert(attackerPieceAlliance), Move::Flag::promotionToQueen });
				}
				else {
					moves.push(move);
				}
			}
		}
	}

	bool isLegalMove(Chessboard chessboard, Move move) {
		BitboardOperations::set_0(chessboard.pieceBitboards[move.attackerPieceAlliance][move.attackerPieceType], move.startPosition);
		BitboardOperations::set_1(chessboard.pieceBitboards[move.attackerPieceAlliance][move.attackerPieceType], move.destinationCoordinate);
		if (move.attackedPieceType != Move::NONE) BitboardOperations::set_0(chessboard.pieceBitboards[move.attackedPieceAlliance][move.attackedPieceType], move.destinationCoordinate);
		if (move.attackerPieceAlliance == SIDE::WHITE) {
			chessboard.setPieceBitboard(SIDE::BLACK, PIECE::PAWN, BitboardOperations::set_0(chessboard.getPieceBitboard(SIDE::BLACK, PIECE::PAWN), move.destinationCoordinate - 8));
		}
		else {
			chessboard.setPieceBitboard(SIDE::WHITE, PIECE::PAWN, BitboardOperations::set_0(chessboard.getPieceBitboard(SIDE::WHITE, PIECE::PAWN), move.destinationCoordinate + 8));
		}
		chessboard.updateBitboards();;
		return (!PseudoLegalMove::leavesInCheck(chessboard, BitboardOperations::bitScanForward(chessboard.getPieceBitboard(move.attackerPieceAlliance, PIECE::KING)), move.attackerPieceAlliance));
	}

	MoveList calculateLegalMoves(const Position& position, uint8_t alliance, bool onlyAttackMoves) {

		MoveList legalMoves;
		Bitboard pawnLeftAttackMoves = PseudoLegalMove::calculatePawnLeftAttackMove(position.getPieces(), alliance, false);
		Bitboard pawnRightAttackMoves = PseudoLegalMove::calculatePawnRightAttackMove(position.getPieces(), alliance, false);

		int8_t pawnsLeftAttackCandidate;
		int8_t pawnsRightAttackCandidate;

		if (alliance == SIDE::WHITE) {
			pawnsLeftAttackCandidate = -7;
			pawnsRightAttackCandidate = -9;
		}
		else {
			pawnsLeftAttackCandidate = 9;
			pawnsRightAttackCandidate = 7;
		}


	}
};