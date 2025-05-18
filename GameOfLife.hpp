#pragma once
#include <vector>
#include <string>
using namespace std;

/// Simple terminal helpers kept in one translation unit for the sake of minimalism.
namespace term {
    void clearScreen();
    bool kbhit();
}

class GameOfLife {
public:
    using Grid = vector<vector<bool>>;

    struct Stats {
        int generation{0};
        int liveCells{0};
    };

    GameOfLife(int width = 40, int height = 20,
               char liveCh = '#', char deadCh = ' ')
        : width_(width), height_(height),
          LIVE(liveCh), DEAD(deadCh),
          grid_(height, vector<bool>(width, false)) {}

    // Public actions that can be triggered by the user
    void randomSeed(int density = 25);
    void clear();
    bool loadPattern(const string& file);
    bool savePattern(const string& file) const;

    // Starts the blocking game loop
    void run();

private:
    // Internal helpers
    int  countNeighbors(int x, int y) const;
    void nextGeneration();
    void updateStats();
    void print() const;
    void handleInput(bool& running);

    // Data members
    int width_, height_;
    const char LIVE, DEAD;
    Grid grid_;
    Stats stats_;
};
