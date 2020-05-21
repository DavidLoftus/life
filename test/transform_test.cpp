#include <state.h>
#include <transform.h>
#include "catch.hpp"

namespace Catch {
    template<>
    struct is_range<life::Grid> {
        static const bool value = false;
    };
}

TEST_CASE("CountNeighbours works properly with dims=8x64", "[CountNeighbours]") {

    life::Grid srcGrid{8, 64};
    life::Grid dstGrid = srcGrid;

    SECTION("empty grid should map to empty grid") {
        life::CountNeighbours(srcGrid, dstGrid);
        REQUIRE(srcGrid == dstGrid);
    }

    SECTION("grid with alive cell at (1,1)") {
        srcGrid.Set(1, 1, 1);
        life::CountNeighbours(srcGrid, dstGrid);

        life::Grid expected{8, 64};
        expected.Set(0, 0, 1);
        expected.Set(0, 1, 1);
        expected.Set(0, 2, 1);
        expected.Set(1, 0, 1);
        expected.Set(1, 1, 1);
        expected.Set(1, 2, 1);
        expected.Set(2, 0, 1);
        expected.Set(2, 1, 1);
        expected.Set(2, 2, 1);

        REQUIRE(dstGrid == expected);
    }

    SECTION("grid with all cells at (0..2, 0..2) alive") {
        srcGrid.Set(0, 0, 1);
        srcGrid.Set(0, 1, 1);
        srcGrid.Set(0, 2, 1);
        srcGrid.Set(1, 0, 1);
        srcGrid.Set(1, 1, 1);
        srcGrid.Set(1, 2, 1);
        srcGrid.Set(2, 0, 1);
        srcGrid.Set(2, 1, 1);
        srcGrid.Set(2, 2, 1);
        life::CountNeighbours(srcGrid, dstGrid);

        life::Grid expected{8, 64};
        expected.Set(0, 0, 4);
        expected.Set(0, 1, 6);
        expected.Set(0, 2, 4);
        expected.Set(0, 3, 2);

        expected.Set(1, 0, 6);
        expected.Set(1, 1, 9);
        expected.Set(1, 2, 6);
        expected.Set(1, 3, 3);

        expected.Set(2, 0, 4);
        expected.Set(2, 1, 6);
        expected.Set(2, 2, 4);
        expected.Set(2, 3, 2);

        expected.Set(3, 0, 2);
        expected.Set(3, 1, 3);
        expected.Set(3, 2, 2);
        expected.Set(3, 3, 1);

        REQUIRE(dstGrid == expected);
    }
}

TEST_CASE("Transform works properly with dims=8x64", "[Transform]") {

    life::Grid srcGrid{8, 64};
    life::Grid dstGrid = srcGrid;

    SECTION("empty grid should map to empty grid") {
        life::Transform(srcGrid, dstGrid);
        REQUIRE(srcGrid == dstGrid);
    }

    SECTION("grid with alive cell at (1,1) should die") {
        srcGrid.Set(1, 1, 1);
        life::Transform(srcGrid, dstGrid);

        life::Grid empty{8, 64};

        REQUIRE(dstGrid == empty);
    }

    SECTION("grid with all cells at (0..2, 0..2) alive") {
        srcGrid.Set(0, 0, 1);
        srcGrid.Set(0, 1, 1);
        srcGrid.Set(0, 2, 1);
        srcGrid.Set(1, 0, 1);
        srcGrid.Set(1, 1, 1);
        srcGrid.Set(1, 2, 1);
        srcGrid.Set(2, 0, 1);
        srcGrid.Set(2, 1, 1);
        srcGrid.Set(2, 2, 1);
        life::Transform(srcGrid, dstGrid);

        life::Grid expected{8, 64};
        expected.Set(0, 0, 1);
        expected.Set(0, 2, 1);

        expected.Set(1, 3, 1);

        expected.Set(2, 0, 1);
        expected.Set(2, 2, 1);

        expected.Set(3, 1, 1);

        REQUIRE(dstGrid == expected);
    }

    SECTION("blinker oscillator") {
        //
        // ###
        //
        life::Grid frame0{8, 64};
        frame0.Set(1, 0, 1);
        frame0.Set(1, 1, 1);
        frame0.Set(1, 2, 1);

        //  #
        //  #
        //  #
        life::Grid frame1{8, 64};
        frame1.Set(0, 1, 1);
        frame1.Set(1, 1, 1);
        frame1.Set(2, 1, 1);

        srcGrid = frame0;
        life::Transform(srcGrid, dstGrid);
        REQUIRE(dstGrid == frame1);

        srcGrid = frame1;
        life::Transform(srcGrid, dstGrid);
        REQUIRE(dstGrid == frame0);
    }

    SECTION("multiple blinker oscillator") {
        //      #
        // ###  #
        //      #
        //
        //  #
        //  #  ###
        //  #
        life::Grid frame0{8, 64};
        frame0.Set(1, 0, 1);
        frame0.Set(1, 1, 1);
        frame0.Set(1, 2, 1);

        frame0.Set(0, 4 + 1, 1);
        frame0.Set(1, 4 + 1, 1);
        frame0.Set(2, 4 + 1, 1);

        frame0.Set(4 + 0, 1, 1);
        frame0.Set(4 + 1, 1, 1);
        frame0.Set(4 + 2, 1, 1);

        frame0.Set(4 + 1, 4 + 0, 1);
        frame0.Set(4 + 1, 4 + 1, 1);
        frame0.Set(4 + 1, 4 + 2, 1);

        //  #
        //  #  ###
        //  #
        //
        //      #
        // ###  #
        //      #

        life::Grid frame1{8, 64};
        frame1.Set(0, 1, 1);
        frame1.Set(1, 1, 1);
        frame1.Set(2, 1, 1);

        frame1.Set(1, 4 + 0, 1);
        frame1.Set(1, 4 + 1, 1);
        frame1.Set(1, 4 + 2, 1);

        frame1.Set(4 + 1, 0, 1);
        frame1.Set(4 + 1, 1, 1);
        frame1.Set(4 + 1, 2, 1);

        frame1.Set(4 + 0, 4 + 1, 1);
        frame1.Set(4 + 1, 4 + 1, 1);
        frame1.Set(4 + 2, 4 + 1, 1);

        srcGrid = frame0;
        life::Transform(srcGrid, dstGrid);
        REQUIRE(dstGrid == frame1);

        srcGrid = frame1;
        life::Transform(srcGrid, dstGrid);
        REQUIRE(dstGrid == frame0);
    }
}

void gridFromString(life::Grid& grid, const std::string& gridStr) {

}

TEST_CASE("Transform imported grids", "[NextState]") {
    life::Grid srcGrid{16, 16};
    life::Grid dstGrid{16, 16};

    SECTION("pulsar oscillator") {

        std::string frame0 =
                "               \n"
                "   ###   ###   \n"
                "               \n"
                " #    # #    # \n"
                " #    # #    # \n"
                " #    # #    # \n"
                "   ###   ###   \n"
                "               \n"
                "   ###   ###   \n"
                " #    # #    # \n"
                " #    # #    # \n"
                " #    # #    # \n"
                "               \n"
                "   ###   ###   \n"
                "               \n";
        std::string frame1 =
                "    #     #    \n"
                "    #     #    \n"
                "    ##   ##    \n"
                "               \n"
                "###  ## ##  ###\n"
                "  # # # # # #  \n"
                "    ##   ##    \n"
                "               \n"
                "    ##   ##    \n"
                "  # # # # # #  \n"
                "###  ## ##  ###\n"
                "               \n"
                "    ##   ##    \n"
                "    #     #    \n"
                "    #     #    \n";

        std::string frame2 =
                "               \n"
                "   ##     ##   \n"
                "    ##   ##    \n"
                " #  # # # #  # \n"
                " ### ## ## ### \n"
                "  # # # # # #  \n"
                "   ###   ###   \n"
                "               \n"
                "   ###   ###   \n"
                "  # # # # # #  \n"
                " ### ## ## ### \n"
                " #  # # # #  # \n"
                "    ##   ##    \n"
                "   ##     ##   \n"
                "               \n";
        
        life::Grid expected{16, 16};

        gridFromString(srcGrid, frame0);
        life::Transform(srcGrid, dstGrid);
        gridFromString(expected, frame1);
        REQUIRE(dstGrid == expected);


        gridFromString(srcGrid, frame1);
        life::Transform(srcGrid, dstGrid);
        gridFromString(expected, frame2);
        REQUIRE(dstGrid == expected);


        gridFromString(srcGrid, frame2);
        life::Transform(srcGrid, dstGrid);
        gridFromString(expected, frame0);
        REQUIRE(dstGrid == expected);
    }


}