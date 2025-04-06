#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>

const int WIDTH = 40;
const int HEIGHT = 20;
const char LIVE = '#';
const char DEAD = ' ';

using Grid = std::vector<std::vector<bool>>;

void printGrid(const Grid& grid) {
    system("clear");
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x)
            std::cout << (grid[y][x] ? LIVE : DEAD);
        std::cout << '\n';
    }
}

int countNeighbors(const Grid& grid, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy)
        for (int dx = -1; dx <= 1; ++dx)
            if (!(dx == 0 && dy == 0)) {
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT)
                    count += grid[ny][nx];
            }
    return count;
}

Grid nextGeneration(const Grid& grid) {
    Grid newGrid = grid;
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x) {
            int neighbors = countNeighbors(grid, x, y);
            if (grid[y][x])
                newGrid[y][x] = (neighbors == 2 || neighbors == 3);
            else
                newGrid[y][x] = (neighbors == 3);
        }
    return newGrid;
}

void seed(Grid& grid) {
    grid[1][2] = true;
    grid[2][3] = true;
    grid[3][1] = true;
    grid[3][2] = true;
    grid[3][3] = true;
}

int main() {
    Grid grid(HEIGHT, std::vector<bool>(WIDTH, false));
    seed(grid);

    while (true) {
        printGrid(grid);
        grid = nextGeneration(grid);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}
