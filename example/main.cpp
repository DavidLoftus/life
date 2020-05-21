#include <iostream>
#include <chrono>

#include <state.h>
#include <transform.h>

void printUsage(char* commandName, std::ostream& os = std::cout) {
    os << "Usage: " << commandName << " WIDTH HEIGHT [N]\n"
       << "N is the number of iterations to perform, default is 1\n"
       << "Grid data should then be provided through stdin.\n";
}

life::Grid advance(life::Grid src, size_t n) {
    life::Grid dst{src.rows(), src.columns()};
    while (n >= 2) {
        life::Transform(src, dst);
        life::Transform(dst, src);
        n -= 2;
    }
    if (n == 1) {
        life::Transform(src, dst);
        return dst;
    } else {
        return src;
    }
}

life::Grid advance(const life::Grid& src) {
    life::Grid dst{src.rows(), src.columns()};
    life::Transform(src, dst);
    return dst;
}

const auto timeIt = [](auto func){
    auto start = std::chrono::high_resolution_clock::now();
    auto ret = func();
    auto finish = std::chrono::high_resolution_clock::now();

    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);

    return std::make_pair(std::move(ret), microseconds);
};

int main(int argc, char** argv) {
    if (argc <= 2) {
        printUsage(argv[0], std::cerr);
        return 1;
    }

    size_t width = std::stoull(argv[1]);
    size_t height = std::stoull(argv[2]);

    life::Grid grid{height, width};

    std::cin >> grid;

    if (argc > 3) {

        size_t N = std::stoull(argv[3]);
        auto [final, time] = timeIt([&]{return advance(grid, N);});
        std::cout << final;
        std::cerr << "Took " << time.count() << "µs\n";
    } else {
        std::cout << advance(grid);
    }
}
