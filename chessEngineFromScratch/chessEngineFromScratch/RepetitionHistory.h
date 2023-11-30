#include <vector>
#include "ZobristHash.h"

#pragma once

class RepetitionHistory {
private:
    std::vector<ZobristHash> hashes;

public:
    RepetitionHistory(ZobristHash hash) {
        this->hashes.push_back(hash);
    }

    void addPosition() {
        this->hashes.clear();
    }

    void clear() {
        this->hashes.clear();
    }

    int getRepetitionNumber(ZobristHash hash) {
        int counter = 0;

        for (auto hash1 : this->hashes) {
            if (hash == hash1) counter++;
        }
        return counter;
    }
};