#ifndef LIFE_NIBBLES_H
#define LIFE_NIBBLES_H

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <vector>
#include <ostream>

namespace life {

    namespace detail {
        class NibbleRef;
    }

    class Nibbles {
    public:
        using NibbleWord = uint64_t;

        static constexpr size_t NIBBLE_BITS = 4;
        static constexpr NibbleWord NIBBLE_MASK = 15;
        static constexpr size_t NIBBLES_PER_WORD = sizeof(NibbleWord) * 8 / NIBBLE_BITS;

        explicit Nibbles(size_t sz) : _nibbles(sz / NIBBLES_PER_WORD) {
            assert(sz % NIBBLES_PER_WORD == 0);
        }

        size_t nibbles_size() const {
            return _nibbles.size() * NIBBLES_PER_WORD;
        }

        size_t word_size() const {
            return _nibbles.size();
        }

        unsigned operator[](size_t nibbleIndex) const {
            return Get(nibbleIndex);
        }

        detail::NibbleRef operator[](size_t nibbleIndex);

        unsigned Get(size_t nibbleIndex) const {
            assert(nibbleIndex < nibbles_size());

            size_t wordIndex = nibbleIndex / NIBBLES_PER_WORD, subWordIndex = nibbleIndex % NIBBLES_PER_WORD;
            NibbleWord word = _nibbles[wordIndex];

            return (word >> (NIBBLE_BITS * subWordIndex)) & NIBBLE_MASK;
        }

        void Set(size_t nibbleIndex, unsigned val) {
            assert(nibbleIndex < nibbles_size());
            assert(val == (val & NIBBLE_MASK));

            size_t wordIndex = nibbleIndex / NIBBLES_PER_WORD;
            size_t subWordIndex = nibbleIndex % NIBBLES_PER_WORD;

            uint64_t mask = NIBBLE_MASK << (NIBBLE_BITS * subWordIndex);

            SetWord(wordIndex, (GetWord(wordIndex) & ~mask) | (NibbleWord(val) << (NIBBLE_BITS * subWordIndex)));
        }

        void SetWord(size_t wordIndex, NibbleWord word) {
            assert(wordIndex < word_size());
            _nibbles[wordIndex] = word;
        }

        NibbleWord GetWord(size_t wordIndex) const {
            assert(wordIndex < word_size());
            return _nibbles[wordIndex];
        }

        void Reset() {
            std::fill(begin(), end(), 0);
        }

        bool operator==(const Nibbles& other) const {
            return nibbles_size() == other.nibbles_size() && _nibbles == other._nibbles;
        }

    private:
        using _vector_type = std::vector<NibbleWord>;

    public:
        _vector_type::iterator begin() { return _nibbles.begin(); }
        _vector_type::iterator end() { return _nibbles.end(); }
        _vector_type::const_iterator begin() const { return _nibbles.begin(); }
        _vector_type::const_iterator end() const { return _nibbles.end(); }
        _vector_type::reverse_iterator rbegin() { return _nibbles.rbegin(); }
        _vector_type::reverse_iterator rend() { return _nibbles.rend(); }
        _vector_type::const_reverse_iterator rbegin() const { return _nibbles.rbegin(); }
        _vector_type::const_reverse_iterator rend() const { return _nibbles.rend(); }

    private:
        _vector_type _nibbles;

    public:
    };

    std::ostream& operator<<(std::ostream& os, const Nibbles& nibbles);

    namespace detail {
        class NibbleRef {
        public:
            explicit NibbleRef(Nibbles& nibbles, size_t nibbleIndex) : _nibbles(nibbles), _nibbleIndex(nibbleIndex) {}

            NibbleRef& operator=(unsigned x) {
                _nibbles.Set(_nibbleIndex, x);
                return *this;
            }

            operator unsigned() const {
                return _nibbles.Get(_nibbleIndex);
            }

        private:
            Nibbles& _nibbles;
            size_t _nibbleIndex;
        };
    }

    inline detail::NibbleRef Nibbles::operator[](size_t nibbleIndex) {
        return detail::NibbleRef(*this, nibbleIndex);
    }

}

#endif //LIFE_NIBBLES_H
