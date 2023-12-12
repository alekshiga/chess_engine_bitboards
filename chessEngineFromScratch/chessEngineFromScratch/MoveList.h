#include <bit>
#include "Move.h"
#include "Bitboards.h"

#pragma once

class MoveList {

private:
	std::array<Move, 218> moves{};
	uint8_t size;

public:
	MoveList() {
		this->size = 0;
	}
	
	Move &operator[](uint8_t index) {
		return this->moves[index];
	}
	
	Move operator[](uint8_t index) const {
		return this->moves[index];
	}

	void push(Move move) {
		this->moves[this->size] = move;
		this->size++;
	}

	uint8_t getSize() const {
		return this->size;
	}
};