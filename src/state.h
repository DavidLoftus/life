#ifndef LIFE_STATE_H
#define LIFE_STATE_H

#include "nibbles.h"

namespace life {

    class Grid {
    public:

        using NibbleWord = Nibbles::NibbleWord;

        Grid(size_t rows, size_t columns) : _rows(rows), _columns(columns), _nibble_rows(rows, Nibbles(columns)) {

        }

        unsigned Get(size_t i, size_t j) const {
            return GetRow(i).Get(j);
        }

        void Set(size_t i, size_t j, unsigned val) {
            GetRow(i).Set(j, val);
        }

        const Nibbles& GetRow(size_t i) const {
            return _nibble_rows[i];
        }

        Nibbles& GetRow(size_t i) {
            return _nibble_rows[i];
        }

        void Reset() {
            for (auto& row : _nibble_rows) {
                row.Reset();
            }
        }

        const Nibbles& operator[](size_t i) const {
            return _nibble_rows[i];
        }

        Nibbles& operator[](size_t i) {
            return _nibble_rows[i];
        }

        size_t columns() const {
            return _columns;
        }

        size_t rows() const {
            return _rows;
        }

        bool operator==(const Grid& other) const;

    private:
        using _vector_type = std::vector<Nibbles>;

    public:
        _vector_type::iterator begin() { return _nibble_rows.begin(); }
        _vector_type::iterator end() { return _nibble_rows.end(); }
        _vector_type::const_iterator begin() const { return _nibble_rows.begin(); }
        _vector_type::const_iterator end() const { return _nibble_rows.end(); }
        _vector_type::reverse_iterator rbegin() { return _nibble_rows.rbegin(); }
        _vector_type::reverse_iterator rend() { return _nibble_rows.rend(); }
        _vector_type::const_reverse_iterator rbegin() const { return _nibble_rows.rbegin(); }
        _vector_type::const_reverse_iterator rend() const { return _nibble_rows.rend(); }

    private:
        size_t _columns;
        size_t _rows;
        _vector_type _nibble_rows;
    };

    std::ostream& operator<<(std::ostream& os, const life::Grid& grid);
    std::istream& operator>>(std::istream& is, life::Grid& grid);

}


#endif //LIFE_STATE_H
