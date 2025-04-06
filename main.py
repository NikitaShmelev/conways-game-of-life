import pygame
import numpy as np

# configuration
CELL_SIZE = 10
GRID_WIDTH = 80
GRID_HEIGHT = 60
WIDTH = GRID_WIDTH * CELL_SIZE
HEIGHT = GRID_HEIGHT * CELL_SIZE

pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Conway's Game of Life")

clock = pygame.time.Clock()

grid = np.zeros((GRID_HEIGHT, GRID_WIDTH), dtype=bool)
running = False

def count_neighbors(grid, x, y):
    return np.sum(grid[max(0, y-1):y+2, max(0, x-1):x+2]) - grid[y, x]

def update(grid):
    new_grid = np.copy(grid)
    for y in range(GRID_HEIGHT):
        for x in range(GRID_WIDTH):
            neighbors = count_neighbors(grid, x, y)
            if grid[y, x]:
                if neighbors < 2 or neighbors > 3:
                    new_grid[y, x] = False
            else:
                if neighbors == 3:
                    new_grid[y, x] = True
    return new_grid

def draw_grid():
    for y in range(GRID_HEIGHT):
        for x in range(GRID_WIDTH):
            rect = pygame.Rect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE)
            color = (255, 255, 255) if grid[y, x] else (30, 30, 30)
            pygame.draw.rect(screen, color, rect)
            pygame.draw.rect(screen, (50, 50, 50), rect, 1)

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            exit()

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                running = not running
            elif event.key == pygame.K_c:
                grid = np.zeros((GRID_HEIGHT, GRID_WIDTH), dtype=bool)

        if pygame.mouse.get_pressed()[0]:  # left btn
            x, y = pygame.mouse.get_pos()
            grid[y // CELL_SIZE, x // CELL_SIZE] = True
        elif pygame.mouse.get_pressed()[2]:  # right btn
            x, y = pygame.mouse.get_pos()
            grid[y // CELL_SIZE, x // CELL_SIZE] = False

    screen.fill((0, 0, 0))
    draw_grid()

    if running:
        grid = update(grid)

    pygame.display.flip()
    clock.tick(10)
