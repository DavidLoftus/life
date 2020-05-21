
#include "catch.hpp"

#include <nibbles.h>

TEST_CASE("Accessors and mutators work", "[Nibbles]") {
    life::Nibbles nibbles{1024};

    REQUIRE(nibbles.nibbles_size() == 1024);

    SECTION("initial value") {
        for (int i = 0; i < 1024; i++) {
            REQUIRE(nibbles.Get(i) == 0);
        }
    }

    SECTION("setting once and retrieving value") {
        for (int i = 0; i < 1024; ++i) {
            nibbles.Set(i, 7);
            REQUIRE(nibbles.Get(i) == 7);
        }
    }

    SECTION("setting, resetting and then retrieving value") {
        nibbles.Set(0, 7);
        nibbles.Set(0, 2);
        REQUIRE(nibbles.Get(0) == 2);
    }

    SECTION("setting at two locations and retrieving both") {
        nibbles.Set(0, 7);
        nibbles.Set(1023, 9);
        REQUIRE(nibbles.Get(0) == 7);
        REQUIRE(nibbles.Get(1023) == 9);
    }
}

TEST_CASE("Accessors and mutators work using operator[]", "[Nibbles]") {
    life::Nibbles nibbles{1024};
    const auto& constNibbles = nibbles;

    REQUIRE(nibbles.nibbles_size() == 1024);

    SECTION("initial value") {
        for (int i = 0; i < 1024; i++) {
            REQUIRE(nibbles[i] == 0);
            REQUIRE(constNibbles[i] == 0);
        }
    }

    SECTION("setting once and retrieving value") {
        for (int i = 0; i < 1024; ++i) {
            nibbles[i] = 9;
            REQUIRE(nibbles[i] == 9);
            REQUIRE(constNibbles[i] == 9);
        }
    }

    SECTION("setting, resetting and then retrieving value") {
        nibbles[0] = 7;
        nibbles[0] = 2;
        REQUIRE(nibbles[0] == 2);
        REQUIRE(constNibbles[0] == 2);
    }

    SECTION("setting at two locations and retrieving both") {
        nibbles[0] = 7;
        nibbles[1023] = 9;

        REQUIRE(nibbles[0] == 7);
        REQUIRE(constNibbles[0] == 7);

        REQUIRE(nibbles[1023] == 9);
        REQUIRE(constNibbles[1023] == 9);
    }
}

TEST_CASE("Word granular mutators/accessors", "[Nibbles]") {
    life::Nibbles nibbles{32};
    const auto& constNibbles = nibbles;

    REQUIRE(nibbles.nibbles_size() == 32);

    SECTION("setting word and retrieving") {
        nibbles.SetWord(0, 0xDEADBEEFCAFEBABE);
        REQUIRE(nibbles.GetWord(0) == 0xDEADBEEFCAFEBABE);
    }

    SECTION("setting word twice and retrieving") {
        nibbles.SetWord(0, 0xC0FFEEB4C0C0A);
        nibbles.SetWord(0, 0xDEADBEEFCAFEBABE);
        REQUIRE(nibbles.GetWord(0) == 0xDEADBEEFCAFEBABE);
    }

    SECTION("setting two locations") {
        nibbles.SetWord(0, 0xC0FFEEB4C0C0A);
        nibbles.SetWord(1, 0xDEADBEEFCAFEBABE);
        REQUIRE(nibbles.GetWord(0) == 0xC0FFEEB4C0C0A);
        REQUIRE(nibbles.GetWord(1) == 0xDEADBEEFCAFEBABE);
    }

    SECTION("setting words then accessing nibbles") {
        nibbles.SetWord(0, 0xDEADBEEFCAFEBABE);

        REQUIRE(nibbles.Get(15) == 0xD);
        REQUIRE(nibbles.Get(14) == 0xE);
        REQUIRE(nibbles.Get(13) == 0xA);
        REQUIRE(nibbles.Get(12) == 0xD);

        REQUIRE(nibbles.Get(3) == 0xB);
        REQUIRE(nibbles.Get(2) == 0xA);
        REQUIRE(nibbles.Get(1) == 0xB);
        REQUIRE(nibbles.Get(0) == 0xE);
    }

    SECTION("setting nibbles then accessing word") {
        nibbles[15] = 0xD;
        nibbles[14] = 0xE;
        nibbles[13] = 0xA;
        nibbles[12] = 0xD;

        nibbles[3] = 0xB;
        nibbles[2] = 0xA;
        nibbles[1] = 0xB;
        nibbles[0] = 0xE;

        REQUIRE(nibbles.GetWord(0) == 0xDEAD00000000BABE);
    }
}

TEST_CASE("Iterators should work", "[Nibbles]") {
    life::Nibbles nibbles{64};

    SECTION("default values") {
        std::vector<uint64_t> words{ nibbles.begin(), nibbles.end() };

        REQUIRE(words.size() == 4);
        REQUIRE(words[0] == 0);
        REQUIRE(words[1] == 0);
        REQUIRE(words[2] == 0);
        REQUIRE(words[3] == 0);
    }

    SECTION("arbitrary values") {
        nibbles.SetWord(0, 0xDEADBEEFCAFEBABE);
        nibbles.SetWord(1, 0xCAFEBABEDEADBEEF);
        nibbles.SetWord(2, 0xC0FFEEB4C0C0A);
        nibbles.SetWord(3, 0xC0C0AB4C0FFEE);

        std::vector<uint64_t> words{ nibbles.begin(), nibbles.end() };

        REQUIRE(words.size() == 4);
        REQUIRE(words[0] == 0xDEADBEEFCAFEBABE);
        REQUIRE(words[1] == 0xCAFEBABEDEADBEEF);
        REQUIRE(words[2] == 0xC0FFEEB4C0C0A);
        REQUIRE(words[3] == 0xC0C0AB4C0FFEE);
    }
}
