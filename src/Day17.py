from copy import deepcopy

class Grid3D:
    def __init__(self, start_width, cycles):
        self.grid = []
        self.total_width = start_width + 2 * cycles
        self.total_depth = 1 + 2 * cycles
        self.load_start = self.total_width // 2 - start_width // 2
        self.cycles = cycles
        for z in range(self.total_depth):
            self.grid.append([])
            for y in range(self.total_width):
                self.grid[-1].append([])
                for x in range(self.total_width):
                    self.grid[-1][-1].append(0)

    def load(self, plane):
        for y, row in enumerate(plane, start=self.load_start):
            for x, value in enumerate(row, start=self.load_start):
                if value == "#":
                    self.grid[self.load_start][y][x] = 1

    def tick(self):
        next_grid = deepcopy(self.grid)

        for z, plane in enumerate(self.grid):
            for y, row in enumerate(plane):
                for x, value in enumerate(row):
                    count = self.count_neighbors(x, y, z)
                    if value == 1 and count not in [2, 3]:
                        next_grid[z][y][x] = 0
                    elif value == 0 and count == 3:
                        next_grid[z][y][x] = 1
        
        self.grid = next_grid

    def tick_all(self):
        for t in range(self.cycles):
            self.tick()

    def count_neighbors(self, x, y, z):
        total = 0
        for zi in range(max(z - 1, 0), min(z + 2, self.total_depth)):
            for yi in range(max(y - 1, 0), min(y + 2, self.total_width)):
                for xi in range(max(x - 1, 0), min(x + 2, self.total_width)):
                    if x == xi and y == yi and z == zi:
                        continue
                    
                    total += self.grid[zi][yi][xi]
        return total

    def count_alive(self):
        return sum(cell for plane in self.grid
                        for row in plane
                        for cell in row)


def part1(plane):
    grid = Grid3D(len(plane), 6)
    grid.load(plane)
    grid.tick_all()
    print(grid.count_alive())


class Grid4D:
    def __init__(self, start_width, cycles):
        self.grid = []
        self.total_width = start_width + 2 * cycles
        self.total_depth = 1 + 2 * cycles
        self.load_start = self.total_width // 2 - start_width // 2
        self.cycles = cycles
        for w in range(self.total_depth):
            self.grid.append([])
            for z in range(self.total_depth):
                self.grid[-1].append([])
                for y in range(self.total_width):
                    self.grid[-1][-1].append([])
                    for x in range(self.total_width):
                        self.grid[-1][-1][-1].append(0)

    def load(self, plane):
        for y, row in enumerate(plane, start=self.load_start):
            for x, value in enumerate(row, start=self.load_start):
                if value == "#":
                    self.grid[self.load_start][self.load_start][y][x] = 1

    def tick(self):
        next_grid = deepcopy(self.grid)

        for w, cube in enumerate(self.grid):
            for z, plane in enumerate(cube):
                for y, row in enumerate(plane):
                    for x, value in enumerate(row):
                        count = self.count_neighbors(x, y, z, w)
                        if value == 1 and count not in [2, 3]:
                            next_grid[w][z][y][x] = 0
                        elif value == 0 and count == 3:
                            next_grid[w][z][y][x] = 1
            
        self.grid = next_grid

    def tick_all(self):
        for t in range(self.cycles):
            self.tick()

    def count_neighbors(self, x, y, z, w):
        total = 0
        for wi in range(max(w - 1, 0), min(w + 2, self.total_depth)):
            for zi in range(max(z - 1, 0), min(z + 2, self.total_depth)):
                for yi in range(max(y - 1, 0), min(y + 2, self.total_width)):
                    for xi in range(max(x - 1, 0), min(x + 2, self.total_width)):
                        if x == xi and y == yi and z == zi and wi == w:
                            continue
                        
                        total += self.grid[wi][zi][yi][xi]
        return total

    def count_alive(self):
        return sum(cell for cube in self.grid
                        for plane in cube
                        for row in plane
                        for cell in row)


def part2(plane):
    grid = Grid4D(len(plane), 6)
    grid.load(plane)
    grid.tick_all()
    print(grid.count_alive())

if __name__ == "__main__":
    with open("data/day17.txt") as f:
        plane = f.read().splitlines()
    part1(plane)
    part2(plane)
    