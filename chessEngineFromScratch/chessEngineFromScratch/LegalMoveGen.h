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

	static void pieceMasksToMoves(Chessboard chessboard, Bitboard mask, uint8_t attackerPiecePosition, uint8_t attackerPieceType, uint8_t attackerPieceAlliance, MoveList& moves) {

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

	static void pawnMasksToMoves(Chessboard chessboard, Bitboard mask, uint8_t attackerPieceAlliance, uint8_t attackerCandidate, bool isAttacked, uint8_t Flag, MoveList& moves) {
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

	static bool isLegalMove(Chessboard chessboard, Move move) {
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

	static void addEnPassantMoves(Chessboard chessboard, uint8_t alliance, uint8_t enPassant, MoveList &moves) {
	
		if (enPassant == Position::NONE) {
			return;
		}

		if (alliance == SIDE::WHITE) {
			if (enPassant % 8 != 7 and BitboardOperations::get_bit(chessboard.pieceBitboards[SIDE::WHITE][PIECE::PAWN], enPassant - 7)) {
				auto move = Move((uint8_t)(enPassant - 7), enPassant, PIECE::PAWN, SIDE::WHITE, Move::NONE, Move::NONE, Move::Flag::enPassantMove);
				if (isLegalMove(chessboard, move)) {
					moves.push(move);
				}
			}
			if (enPassant % 8 != 0 and BitboardOperations::get_bit(chessboard.pieceBitboards[SIDE::WHITE][PIECE::PAWN], enPassant - 9)) {
				auto move = Move((uint8_t)(enPassant - 9), enPassant, PIECE::PAWN, SIDE::WHITE, Move::NONE, Move::NONE, Move::Flag::enPassantMove);
				if (isLegalMove(chessboard, move)) {
					moves.push(move);
				}
			}
		}
		else {
			if (enPassant % 8 != 0 and BitboardOperations::get_bit(chessboard.pieceBitboards[SIDE::BLACK][PIECE::PAWN], enPassant + 7)) {
				auto move = Move((uint8_t)(enPassant + 7), enPassant, PIECE::PAWN, SIDE::BLACK, Move::NONE, Move::NONE, Move::Flag::enPassantMove);
				if (isLegalMove(chessboard, move)) {
					moves.push(move);
				}
			}
			if (enPassant % 8 != 7 and BitboardOperations::get_bit(chessboard.pieceBitboards[SIDE::BLACK][PIECE::PAWN], enPassant + 9)) {
				auto move = Move((uint8_t)(enPassant + 9), enPassant, PIECE::PAWN, SIDE::BLACK, Move::NONE, Move::NONE, Move::Flag::enPassantMove);
				if (isLegalMove(chessboard, move)) {
					moves.push(move);
				}
			}
		}
	}

	static void addCastlingMoves(Chessboard chessboard, uint8_t alliance, bool longCastling, bool shortCastling, MoveList& moves) {
		uint8_t index;
		uint8_t longCastlingFlag;
		uint8_t shortCastlingFlag;
		if (alliance == SIDE::WHITE) {
			index = 0;
			longCastlingFlag = Move::Flag::whiteLongCastling;
			shortCastlingFlag = Move::Flag::whiteShortCastling;
		}
		else {
			index = 56;
			longCastlingFlag = Move::Flag::blackLongCastling;
			shortCastlingFlag = Move::Flag::blackShortCastling;
		}

		if (longCastling and
			BitboardOperations::get_bit(chessboard.pieceBitboards[alliance][PIECE::ROOK], 0 + index) and
			BitboardOperations::get_bit(chessboard.emptyBoard, 1 + index) and
			BitboardOperations::get_bit(chessboard.emptyBoard, 2 + index) and
			BitboardOperations::get_bit(chessboard.emptyBoard, 3 + index) and
			!PseudoLegalMove::leavesInCheck(chessboard, BitboardOperations::bitScanForward(chessboard.getPieceBitboard(alliance, PIECE::KING)), alliance) and
			!PseudoLegalMove::leavesInCheck(chessboard, 2 + index, alliance) and
			!PseudoLegalMove::leavesInCheck(chessboard, 3 + index, alliance)) {
			moves.push({ (uint8_t)(4 + index), (uint8_t)(2 + index), PIECE::KING, alliance, Move::NONE, Move::NONE, longCastlingFlag });
		}

		if (shortCastling and
			BitboardOperations::get_bit(chessboard.pieceBitboards[alliance][PIECE::ROOK], 7 + index) and
			BitboardOperations::get_bit(chessboard.emptyBoard, 5 + index) and
			BitboardOperations::get_bit(chessboard.emptyBoard, 6 + index) and
			!PseudoLegalMove::leavesInCheck(chessboard, BitboardOperations::bitScanForward(chessboard.getPieceBitboard(alliance, PIECE::KING)), alliance) and
			!PseudoLegalMove::leavesInCheck(chessboard, 5 + index, alliance) and
			!PseudoLegalMove::leavesInCheck(chessboard, 6 + index, alliance)) {
			moves.push({ (uint8_t)(4 + index), (uint8_t)(6 + index), PIECE::KING, alliance, Move::NONE, Move::NONE, shortCastlingFlag });
		}
	}

	static MoveList calculateLegalMoves(const Position& position, uint8_t alliance, bool onlyAttackMoves) {

		MoveList legalMoves;
		Bitboard pawnLeftAttackMoves = PseudoLegalMove::calculatePawnLeftAttackMove(position.getBoard(), alliance, false);
		Bitboard pawnRightAttackMoves = PseudoLegalMove::calculatePawnRightAttackMove(position.getBoard(), alliance, false);

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

		pawnMasksToMoves(position.getBoard(), pawnLeftAttackMoves, alliance, pawnsLeftAttackCandidate, true, Move::Flag::regular, legalMoves);
		pawnMasksToMoves(position.getBoard(), pawnRightAttackMoves, alliance, pawnsRightAttackCandidate, true, Move::Flag::regular, legalMoves);

		if (!onlyAttackMoves) {
			Bitboard pawnMoves = PseudoLegalMove::calculatePawnMoveMask(position.getBoard(), alliance);
			Bitboard pawnJumpMoves = PseudoLegalMove::calculatePawnJumpMask(position.getBoard(), alliance);
			int8_t pawnMoveCandidate;
			int8_t pawnJumpCandidate;
			
			if (alliance == SIDE::WHITE) {
				pawnMoveCandidate = -8;
				pawnJumpCandidate = -16;
			}
			else {
				pawnMoveCandidate = 8;
				pawnJumpCandidate = 16;
			}
			pawnMasksToMoves(position.getBoard(), pawnMoves, alliance, pawnMoveCandidate, false, Move::Flag::regular, legalMoves);
			pawnMasksToMoves(position.getBoard(), pawnJumpMoves, alliance, pawnJumpCandidate, false, Move::Flag::pawnJump, legalMoves);
		}

		Bitboard knightMoves = position.getBoard().pieceBitboards[alliance][PIECE::KNIGHT];
		Bitboard bishopMoves = position.getBoard().pieceBitboards[alliance][PIECE::BISHOP];
		Bitboard rookMoves = position.getBoard().pieceBitboards[alliance][PIECE::ROOK];
		Bitboard queenMoves = position.getBoard().pieceBitboards[alliance][PIECE::QUEEN];
		
		uint8_t attackerPosition;
		Bitboard mask;

		while (knightMoves) {
			attackerPosition = BitboardOperations::bitScanForward(knightMoves);
			knightMoves = BitboardOperations::set_0(knightMoves, attackerPosition);
			mask = PseudoLegalMove::calculateKnightMask(position.getBoard(), attackerPosition, alliance, onlyAttackMoves);
			pieceMasksToMoves(position.getBoard(), mask, attackerPosition, PIECE::KNIGHT, alliance, legalMoves);
		}

		while (bishopMoves) {
			attackerPosition = BitboardOperations::bitScanForward(bishopMoves);
			bishopMoves = BitboardOperations::set_0(bishopMoves, attackerPosition);
			mask = PseudoLegalMove::calculateBishopMask(position.getBoard(), attackerPosition, alliance, onlyAttackMoves);
			pieceMasksToMoves(position.getBoard(), mask, attackerPosition, PIECE::BISHOP, alliance, legalMoves);
		}
		while (rookMoves) {
			attackerPosition = BitboardOperations::bitScanForward(rookMoves);
			rookMoves = BitboardOperations::set_0(rookMoves, attackerPosition);
			mask = PseudoLegalMove::calculateRookMask(position.getBoard(), attackerPosition, alliance, onlyAttackMoves);
			pieceMasksToMoves(position.getBoard(), mask, attackerPosition, PIECE::ROOK, alliance, legalMoves);
		}
		while (queenMoves) {
			attackerPosition = BitboardOperations::bitScanForward(queenMoves);
			queenMoves = BitboardOperations::set_0(queenMoves, attackerPosition);
			mask = PseudoLegalMove::calculateQueenMask(position.getBoard(), attackerPosition, alliance, onlyAttackMoves);
			pieceMasksToMoves(position.getBoard(), mask, attackerPosition, PIECE::QUEEN, alliance, legalMoves);
		}
		attackerPosition = BitboardOperations::bitScanForward(position.getBoard().getPieceBitboard(alliance, PIECE::KING));
		mask = PseudoLegalMove::calculateKingMask(position.getBoard(), attackerPosition, alliance, onlyAttackMoves);
		pieceMasksToMoves(position.getBoard(), mask, attackerPosition, PIECE::KING, alliance, legalMoves);

		addEnPassantMoves(position.getBoard(), alliance, position.getEnPassant(), legalMoves);
		if (!onlyAttackMoves) {
			if (alliance == SIDE::WHITE) {
				addCastlingMoves(position.getBoard(), SIDE::WHITE, position.getWhiteLongCastling(), position.getWhiteShortCastling(), legalMoves);
			}
			else {
				addCastlingMoves(position.getBoard(), SIDE::BLACK, position.getBlackLongCastling(), position.getBlackShortCastling(), legalMoves);
			}
		}

		return legalMoves;
	}
};