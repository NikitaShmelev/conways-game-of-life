#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
namespace fs = filesystem;

const int WIDTH = 40;
const int HEIGHT = 20;
const char LIVE = '#';
const char DEAD = ' ';

using Grid = vector<vector<bool>>;

struct GameStats {
    int generation = 0;
    int liveCells = 0;
};

// Clear screen with terminal command
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Check is input, part of conio.h, not available on unix systems
bool kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

void printGrid(const Grid& grid, const GameStats& stats) {
    clearScreen();
    
    // Print stats
    cout << "Generation: " << stats.generation 
         << " | Live cells: " << stats.liveCells 
         << " | Controls: (R)andom (C)lear (L)oad (S)ave (Q)uit\n";
    
    // Print grid
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x)
            cout << (grid[y][x] ? LIVE : DEAD);
        cout << '\n';
    }
}

int countNeighbors(const Grid& grid, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (!(dx == 0 && dy == 0)) {
                int nx = (x + dx + WIDTH) % WIDTH;  // Wrap around
                int ny = (y + dy + HEIGHT) % HEIGHT;
                count += grid[ny][nx];
            }
        }
    }
    return count;
}

Grid nextGeneration(const Grid& grid) {
    Grid newGrid(HEIGHT, vector<bool>(WIDTH, false));
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int neighbors = countNeighbors(grid, x, y);
            if (grid[y][x]) {
                newGrid[y][x] = (neighbors == 2 || neighbors == 3);
            } else {
                newGrid[y][x] = (neighbors == 3);
            }
        }
    }
    return newGrid;
}

void randomSeed(Grid& grid, int density = 25) {
    for (auto& row : grid) {
        for (auto cell : row) {
            cell = (rand() % 100) < density;
        }
    }
}

void clearGrid(Grid& grid) {
    for (auto& row : grid) {
        fill(row.begin(), row.end(), false);
    }
}

bool loadPattern(Grid& grid, const string& filename) {
    if (!fs::exists(filename)) {
        cerr << "Error: File not found - " << filename << endl;
        this_thread::sleep_for(chrono::seconds(1));
        return false;
    }

    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file - " << filename << endl;
        this_thread::sleep_for(chrono::seconds(1));
        return false;
    }

    clearGrid(grid);
    
    string line;
    int startY = HEIGHT / 2;
    while (getline(file, line) && startY < HEIGHT) {
        int startX = (WIDTH - line.size()) / 2;
        for (int x = 0; x < line.size() && startX + x < WIDTH; ++x) {
            if (line[x] == LIVE) {
                grid[startY][startX + x] = true;
            }
        }
        startY++;
    }
    return true;
}

bool savePattern(const Grid& grid, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error: Cannot create file - " << filename << endl;
        this_thread::sleep_for(chrono::seconds(1));
        return false;
    }

    for (const auto& row : grid) {
        for (bool cell : row) {
            file << (cell ? LIVE : DEAD);
        }
        file << '\n';
    }
    return true;
}

void updateStats(GameStats& stats, const Grid& grid) {
    stats.generation++;
    stats.liveCells = 0;
    for (const auto& row : grid) {
        stats.liveCells += count(row.begin(), row.end(), true);
    }
}

void showMenu() {
    clearScreen();
    cout << "CONWAY'S GAME OF LIFE\n"
         << "1. Start with random seed\n"
         << "2. Load pattern from file\n"
         << "3. Exit\n"
         << "Choice: ";
}

void handleInput(Grid& grid, bool& running) {
    if (kbhit()) {
        char c = getchar();
        switch (tolower(c)) {
            case 'r':
                randomSeed(grid);
                break;
            case 'c':
                clearGrid(grid);
                break;
            case 'l': {
                string filename;
                cout << "Enter filename: ";
                cin >> filename;
                loadPattern(grid, filename);
                break;
            }
            case 's': {
                string filename;
                cout << "Enter filename to save: ";
                cin >> filename;
                savePattern(grid, filename);
                break;
            }
            case 'q':
                running = false;
                break;
        }
    }
}

int main() {
    srand(time(nullptr));
    Grid grid(HEIGHT, vector<bool>(WIDTH, false));
    GameStats stats;
    bool running = true;

    // Show menu
    showMenu();
    char choice;
    cin >> choice;

    switch (choice) {
        case '1':
            randomSeed(grid);
            break;
        case '2': {
            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            if (!loadPattern(grid, filename)) {
                return 1;
            }
            break;
        }
        case '3':
            return 0;
        default:
            cout << "Invalid choice!" << endl;
            return 1;
    }

    // Main game loop
    while (running) {
        updateStats(stats, grid);
        printGrid(grid, stats);
        grid = nextGeneration(grid);
        handleInput(grid, running);
        this_thread::sleep_for(chrono::milliseconds(200));
    }

    return 0;
}