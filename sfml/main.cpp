#include <fstream>
#include <thread>

#include <SFML/Graphics.hpp>
#include <life/state.h>
#include <life/transform.h>

#include <iostream>


void drawGrid(const life::Grid& grid, sf::Image& img) {
    auto sz = img.getSize();
    double aspectRatio = std::max(grid.rows() / double(sz.y), grid.columns() / double(sz.x));

    for (int i = 0; i < sz.y; ++i) {
        auto row = int(i * aspectRatio);
        for (int j = 0; j < sz.x; ++j) {
            auto col = int(j * aspectRatio);
            if (row >= grid.rows() || col >= grid.columns()) {
                img.setPixel(j, i, sf::Color::Black);
            } else {
                img.setPixel(j, i, grid[row][col] ? sf::Color::White : sf::Color::Black);
            }
        }
    }
}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");


    // Load a sprite to display
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    auto windowSize = window.getSize();
    image.create(windowSize.x, windowSize.y);

    life::Grid grid{512, 512};
    life::Grid swap{512, 512};
    {
        std::ifstream inFile{"../../example/pulsar_big.txt"};
        if (!inFile) return EXIT_FAILURE;
        inFile >> grid;
    }

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized) {
                image.create(event.size.width, event.size.height);
            }
        }
        // Clear screen
        window.clear();

        drawGrid(grid, image);
        texture.loadFromImage(image);
        sprite.setTexture(texture);

        window.draw(sprite);

        // Update the window
        window.display();

        life::Transform(grid, swap);
        std::swap(grid, swap);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return EXIT_SUCCESS;
}