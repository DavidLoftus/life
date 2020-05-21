#include "transform.h"

#include <array>

using namespace life;

void partialSumEdgeRow(const Nibbles& middle, const Nibbles& other, Nibbles& dstRow) {
    auto middleIt = middle.begin();
    auto otherIt = other.begin();

    auto dstIt = dstRow.begin();
    while (dstIt != dstRow.end()) {
        *dstIt++ = *middleIt++ + *otherIt++;
    }
}

void partialSumMiddleRow(const Nibbles& top, const Nibbles& middle, const Nibbles& bottom, Nibbles& dstRow) {
    auto topIt = top.begin();
    auto middleIt = middle.begin();
    auto bottomIt = bottom.begin();

    auto dstIt = dstRow.begin();
    while (dstIt != dstRow.end()) {
        *dstIt++ = *topIt++ + *middleIt++ + *bottomIt++;
    }
}

void sumVerticalNeighbours(const Grid& src, Grid& dst) {
    assert(src.rows() == dst.rows());
    assert(src.columns() == dst.columns());

    if (src.rows() < 2) {
        return;
    }

    partialSumEdgeRow(
            src.GetRow(0),
            src.GetRow(1),
            dst.GetRow(0));

    // Perform partial sums on all rows
    for (int i = 1; i < src.rows() - 1; ++i) {
        partialSumMiddleRow(
                src.GetRow(i - 1),
                src.GetRow(i),
                src.GetRow(i + 1),
                dst.GetRow(i));
    }

    partialSumEdgeRow(
            src.GetRow(src.rows() - 1),
            src.GetRow(src.rows() - 2),
            dst.GetRow(src.rows() - 1));
}

void sumHorizontalNeighbours(Nibbles& dstRow) {
    assert(dstRow.word_size() > 0);

    unsigned lastNibble = 0;
    for (int i = 0; i < dstRow.word_size() - 1; ++i) {
        auto word = dstRow.GetWord(i);
        auto rightShift = (word >> Nibbles::NIBBLE_BITS) | lastNibble;
        auto leftShift = (word << Nibbles::NIBBLE_BITS) | (dstRow.GetWord(i + 1) & Nibbles::NIBBLE_MASK);

        dstRow.SetWord(i, word + rightShift + leftShift);

        lastNibble = word >> (Nibbles::NIBBLE_BITS * (Nibbles::NIBBLES_PER_WORD - 1));
    }

    auto word = dstRow.GetWord(dstRow.word_size() - 1);
    auto rightShift = (word >> Nibbles::NIBBLE_BITS) | lastNibble;
    auto leftShift = (word << Nibbles::NIBBLE_BITS);

    dstRow.SetWord(dstRow.word_size() - 1, word + rightShift + leftShift);
}

void sumHorizontalNeighbours(Grid& dst) {
    if (dst.rows() * dst.columns() == 0) {
        return;
    }
    for (int i = 0; i < dst.rows(); ++i) {
        sumHorizontalNeighbours(dst.GetRow(i));
    }
}

void life::CountNeighbours(const Grid& src, Grid& dst) {
    sumVerticalNeighbours(src, dst);
    sumHorizontalNeighbours(dst);
}

using NibbleWord = Nibbles::NibbleWord;

NibbleWord getKey(NibbleWord nibble, NibbleWord neighbours) {
    return neighbours | (nibble << 3u);
}

std::vector<uint16_t> expandLookupTable(std::array<uint16_t, 16> baseTable) noexcept {
    const size_t len = 1u << 16u;

    std::vector<uint16_t> table(len);

    for (unsigned i = 0; i < len; ++i) {
        table[i] = (NibbleWord(baseTable[(i >> 12u) & Nibbles::NIBBLE_MASK]) << 12u)
                   | (NibbleWord(baseTable[(i >> 8u) & Nibbles::NIBBLE_MASK]) << 8u)
                   | (NibbleWord(baseTable[(i >> 4u) & Nibbles::NIBBLE_MASK]) << 4u)
                   | (NibbleWord(baseTable[(i >> 0u) & Nibbles::NIBBLE_MASK]) << 0u);
    }

    return table;
}

const std::vector<uint16_t> stateTransitionTable = expandLookupTable(
        {
                0, 0, 0, 1, 0, 0, 0, 0, // dead cells
                0, 0, 0, 1, 1, 0, 0, 0, // live cells
        });

NibbleWord lookupKey(uint16_t key) {
    return stateTransitionTable[key];
}

NibbleWord lookupKeys(NibbleWord keys) {
    const NibbleWord keyMask = (1u << 16u) - 1u;
    return (lookupKey(keys >> 48u) << 48u)
           | (lookupKey((keys >> 32u) & keyMask) << 32u)
           | (lookupKey((keys >> 16u) & keyMask) << 16u)
           | lookupKey(keys & keyMask);
}

void nextState(const Nibbles& src, Nibbles& dst) {
    auto srcIt = src.begin();
    auto dstIt = dst.begin();
    for (; dstIt != dst.end(); srcIt++, dstIt++) {
        NibbleWord keys = getKey(*srcIt, *dstIt);
        *dstIt = lookupKeys(keys);
    }
}

void life::NextState(const Grid& src, Grid& dst) {
    auto srcRowIt = src.begin();
    auto dstRowIt = dst.begin();
    for (; dstRowIt != dst.end(); srcRowIt++, dstRowIt++) {
        nextState(*srcRowIt, *dstRowIt);
    }
}

void life::Transform(const Grid& src, Grid& dst) {
    CountNeighbours(src, dst);
    NextState(src, dst);
}
