#include <iostream>

#include <state.h>
#include <transform.h>

void printUsage(char* commandName, std::ostream& os = std::cout) {
    os << "Usage: " << commandName << " WIDTH HEIGHT\n"
       << "Grid should then be provided through stdin.\n";
}

int main(int argc, char** argv) {
    if (argc <= 2) {
        printUsage(argv[0], std::cerr);
        return 1;
    }

    size_t width = std::stoull(argv[1]);
    size_t height = std::stoull(argv[2]);

    life::Grid grid{height, width};

    std::cin >> grid;

    life::Grid nextState{height, width};
    life::Transform(grid, nextState);

    std::cout << nextState;
}
