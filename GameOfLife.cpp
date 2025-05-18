#include "GameOfLife.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
namespace fs = std::filesystem;

/* ---------- term:: implementation ---------- */
void term::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Very small POSIX-style replacement for <conio.h> kbhit()
bool term::kbhit() {
    termios oldt{}, newt{};
    int ch, oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;  newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) { ungetc(ch, stdin); return true; }
    return false;
}
/* ---------- end term:: ---------- */

void GameOfLife::randomSeed(int density) {
    for (auto& row : grid_) {
        for (auto&& cell : row) {
            cell = (rand() % 100) < density;
        }
    }
}

void GameOfLife::clear() {
    for (auto& row : grid_)
        std::fill(row.begin(), row.end(), false);
}

bool GameOfLife::loadPattern(const string& filename) {
    if (!fs::exists(filename)) { cerr << "File not found: " << filename << '\n'; return false; }
    ifstream file(filename);
    if (!file) { cerr << "Cannot open: " << filename << '\n'; return false; }

    clear();
    string line;
    int y = height_ / 2;
    while (getline(file, line) && y < height_) {
        int x0 = (width_ - static_cast<int>(line.size())) / 2;
        for (size_t dx = 0; dx < line.size() && x0 + static_cast<int>(dx) < width_; ++dx)
            grid_[y][x0 + dx] = (line[dx] == LIVE);
        ++y;
    }
    return true;
}

bool GameOfLife::savePattern(const string& filename) const {
    ofstream file(filename);
    if (!file) { cerr << "Cannot create: " << filename << '\n'; return false; }
    for (const auto& row : grid_) {
        for (bool cell : row) file << (cell ? LIVE : DEAD);
        file << '\n';
    }
    return true;
}

/* ---- Private helpers ---- */

int GameOfLife::countNeighbors(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy)
        for (int dx = -1; dx <= 1; ++dx)
            if (dx || dy)
                count += grid_[(y + dy + height_) % height_]
                              [(x + dx + width_)  % width_];
    return count;
}

void GameOfLife::nextGeneration() {
    Grid next = grid_;
    for (int y = 0; y < height_; ++y)
        for (int x = 0; x < width_; ++x) {
            int n = countNeighbors(x, y);
            next[y][x] = grid_[y][x] ? (n == 2 || n == 3) : (n == 3);
        }
    grid_.swap(next);
}

void GameOfLife::updateStats() {
    ++stats_.generation;
    stats_.liveCells = 0;
    for (const auto& row : grid_)
        stats_.liveCells += std::count(row.begin(), row.end(), true);
}

void GameOfLife::print() const {
    term::clearScreen();
    cout << "Gen: " << stats_.generation
         << " | Live: " << stats_.liveCells
         << " | (R)andom (C)lear (L)oad (S)ave (Q)uit\n";

    for (const auto& row : grid_) {
        for (bool cell : row) cout << (cell ? LIVE : DEAD);
        cout << '\n';
    }
}

void GameOfLife::handleInput(bool& running) {
    if (!term::kbhit()) return;
    char c = getchar();
    switch (tolower(c)) {
        case 'r': randomSeed(); break;
        case 'c': clear(); break;
        case 'l': {
            string f; cout << "File to load: "; cin >> f; loadPattern(f); break;
        }
        case 's': {
            string f; cout << "File to save: "; cin >> f; savePattern(f); break;
        }
        case 'q': running = false; break;
    }
}

/* ---- Public run() ---- */

void GameOfLife::run() {
    bool running = true;
    while (running) {
        updateStats();
        print();
        nextGeneration();
        handleInput(running);
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}
