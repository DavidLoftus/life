#include "life/nibbles.h"

#include <algorithm>

using namespace life;

void writeNibbleWord(std::ostream& os, Nibbles::NibbleWord word) {
    for (int i = 0; i < Nibbles::NIBBLES_PER_WORD; ++i) {
        unsigned nibble = word & Nibbles::NIBBLE_MASK;
        switch (nibble) {
            case 0:
                os << ' ';
                break;
            case 1:
                os << '#';
                break;
            default:
                os << std::hex << nibble;
        }
        word >>= Nibbles::NIBBLE_BITS;
    }
}


std::ostream& life::operator<<(std::ostream& os, const Nibbles& nibbles) {
    auto start = nibbles.begin();
    // Only print up to last non-zero word
    auto end = std::find_if(
            nibbles.rbegin(),
            nibbles.rend(),
            [](auto word){return word != 0;}).base();
    std::for_each(start, end, [&](auto word) {
        writeNibbleWord(os, word);
    });
    return os;
}
