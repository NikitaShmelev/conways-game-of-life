### MS: I asked ChatGPT to hint me project structure and delepment path for it

> It doesn't mean we should follow it. That just a start point.
> I asked ChatGPT to NOT give me some coding solution


# **Conway's Game of Life - C++ Implementation**

## **Project Overview**
Conway's Game of Life is a cellular automaton where cells on a grid evolve over discrete time steps based on simple rules. This project provides a structured and efficient implementation using C++ best practices.

---

## **Project Structure**
A clean and modular structure improves maintainability and scalability.

```
GameOfLife/
│── src/
│   │── main.cpp            # Entry point of the application
│   │── GameOfLife.cpp      # Core implementation of the game
│   │── Grid.cpp            # Manages the grid (cells and states)
│   │── Renderer.cpp        # Handles rendering (console/GUI)
│── include/
│   │── GameOfLife.h        # Header for game logic
│   │── Grid.h              # Header for grid operations
│   │── Renderer.h          # Header for rendering
│── tests/
│   │── test_GameOfLife.cpp # Unit tests for game logic
│   │── test_Grid.cpp       # Unit tests for grid operations
│── assets/                 # (Optional) Store assets if using graphical representation
│── CMakeLists.txt          # CMake build script
│── README.md               # Documentation
│── .gitignore              # Ignore unnecessary files
```

---

## **Development Roadmap**
### **1. Define Core Concepts**
- The game consists of a **grid of cells**, each cell having two states: **alive or dead**.
- A **tick-based simulation** determines the next state based on rules:
  1. Any live cell with fewer than two live neighbors **dies** (underpopulation).
  2. Any live cell with two or three live neighbors **survives**.
  3. Any live cell with more than three live neighbors **dies** (overpopulation).
  4. Any dead cell with exactly three live neighbors **becomes alive** (reproduction).

---

### **2. Implement the Grid Class (`Grid.h`, `Grid.cpp`)**
- **Store cell states** using a 2D vector (`std::vector<std::vector<bool>>`).
- **Provide access methods**:
  - Get the current state of a cell.
  - Count live neighbors.
  - Toggle a cell state.
  - Initialize the grid with a pattern.

```cpp
class Grid {
public:
    Grid(int width, int height);
    void update();  // Compute next state
    void toggleCell(int x, int y);
    void display() const;  // For console output
private:
    int width, height;
    std::vector<std::vector<bool>> cells;
    int countLiveNeighbors(int x, int y) const;
};
```

---

### **3. Implement the Game Logic (`GameOfLife.h`, `GameOfLife.cpp`)**
- Manages **grid updates** and game loop.
- Handles **initialization and user input**.

```cpp
class GameOfLife {
public:
    GameOfLife(int width, int height);
    void run();
private:
    Grid grid;
    void processInput();
};
```

---

### **4. Implement Rendering (`Renderer.h`, `Renderer.cpp`)**
- **Basic Version**: Use terminal output (`std::cout`).
- **Advanced Version**: Use **SFML** or **SDL2** for graphical rendering.

```cpp
class Renderer {
public:
    void render(const Grid& grid);
};
```

---

### **5. Build the `main.cpp`**
- Initialize the game.
- Run the game loop.

```cpp
int main() {
    GameOfLife game(20, 20);
    game.run();
    return 0;
}
```

---

### **6. Implement Testing (`tests/`)**
- Use **Google Test** or a simple `assert()`-based testing framework.
- Test **neighbor counting, grid evolution, and boundary cases**.

---

### **7. Optimize**
- Consider **multithreading** for performance (`std::thread`).
- Use **bit manipulation** for space efficiency.
- Implement **wrapping edges** (toroidal grid).

---

## **Bonus Enhancements**
- **GUI with SFML/SDL2** (instead of console).
- **Save/load grid state** (serialization).
- **Custom rules** (adjustable survival/reproduction conditions).
- **Pattern library** (Glider, Pulsar, etc.).

---

## **How to Compile and Run**
### **Using CMake**
1. **Clone the Repository**
   ```sh
   git clone https://github.com/yourusername/GameOfLife.git
   cd GameOfLife
   ```
2. **Create a Build Directory**
   ```sh
   mkdir build && cd build
   ```
3. **Run CMake**
   ```sh
   cmake ..
   make
   ```
4. **Run the Executable**
   ```sh
   ./GameOfLife
   ```

---

## **License**
This project is open-source and available under the MIT License.

