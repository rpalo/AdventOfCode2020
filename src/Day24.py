from collections import defaultdict
import sys


BLACK = True
WHITE = False

tiles = defaultdict(bool)
filename = sys.argv[1]
with open(filename, "r") as f:
    for line in f:
        x = 0
        y = 0
        line_iter = iter(line)
        for char in line_iter:
            if char == 'e':
                x += 1
            elif char == 'w':
                x -= 1
            elif char == 'n':
                y += 1
                if next(line_iter) == 'e':
                    x += 0.5
                else:
                    x -= 0.5
            elif char == 's':
                y -= 1
                if next(line_iter) == 'e':
                    x += 0.5
                else:
                    x -= 0.5
            
        tiles[(x, y)] = not tiles[(x, y)]

print("Part 1:", sum(tiles.values()))

for i in range(100):
    new_tiles = defaultdict(bool)

    # Ensure white tiles around black tiles in input set
    blacks = [coord for coord, color in tiles.items() if color]
    for x, y in blacks:
        tiles[(x + 1, y)]
        tiles[(x - 1, y)]
        tiles[(x + 0.5, y + 1)]
        tiles[(x - 0.5, y + 1)]
        tiles[(x + 0.5, y - 1)]
        tiles[(x - 0.5, y - 1)]
    
    # Evolve based on rules
    for (x, y), color in tiles.items():
        total = sum([
            (x + 1, y) in tiles and tiles[(x + 1, y)],
            (x - 1, y) in tiles and tiles[(x - 1, y)],
            (x + 0.5, y + 1) in tiles and tiles[(x + 0.5, y + 1)],
            (x - 0.5, y + 1) in tiles and tiles[(x - 0.5, y + 1)],
            (x + 0.5, y - 1) in tiles and tiles[(x + 0.5, y - 1)],
            (x - 0.5, y - 1) in tiles and tiles[(x - 0.5, y - 1)],
        ])
        if color == BLACK and (total == 0 or total > 2):
            new_tiles[(x, y)] = WHITE
        elif color == WHITE and total == 2:
            new_tiles[(x, y)] = BLACK
        else:
            new_tiles[(x, y)] = color

    tiles = new_tiles

print("Part 2:", sum(tiles.values()))