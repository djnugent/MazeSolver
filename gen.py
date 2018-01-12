import numpy as np
from numpy.random import random_integers as rand
import matplotlib.pyplot as pyplot
from PIL import Image


def maze(width=81, height=51, complexity=.75, density=.75):
    valid = False
    attempts = 0
    while not valid and attempts < 5:
        # Only odd shapes
        shape = ((height // 2) * 2 + 1, (width // 2) * 2 + 1)
        # Adjust complexity and density relative to maze size
        _complexity = int(complexity * (5 * (shape[0] + shape[1])))
        _density    = int(density * ((shape[0] // 2) * (shape[1] // 2)))
        # Build actual maze
        Z = np.zeros(shape, dtype=bool)
        # Fill borders
        Z[0, :] = Z[-1, :] = 1
        Z[:, 0] = Z[:, -1] = 1

        # Make aisles
        for i in range(_density):
            x, y = rand(0, shape[1] // 2) * 2, rand(0, shape[0] // 2) * 2
            Z[y, x] = 1
            for j in range(_complexity):
                neighbours = []
                if x > 1:             neighbours.append((y, x - 2))
                if x < shape[1] - 2:  neighbours.append((y, x + 2))
                if y > 1:             neighbours.append((y - 2, x))
                if y < shape[0] - 2:  neighbours.append((y + 2, x))
                if len(neighbours):
                    y_,x_ = neighbours[rand(0, len(neighbours) - 1)]
                    if Z[y_, x_] == 0:
                        Z[y_, x_] = 1
                        Z[y_ + (y - y_) // 2, x_ + (x - x_) // 2] = 1
                        x, y = x_, y_

        attempts += 1
        # check to make sure we can add an entry and exit
        if Z[1,1] == 0 and Z[height-1,width-1] == 0:
            valid = True
            Z[1,0] = 0
            Z[height-1,width] = 0

    if(attempts >= 5):
        raise RuntimeError("Unable to generate maze with 5 attempts. Please increase maze size")

    return Z


m = maze(150,150)
im = Image.fromarray(255-np.uint8(m)*255)
im.save("maze.png")

pyplot.figure(figsize=(10, 5))
pyplot.imshow(m, cmap=pyplot.cm.binary, interpolation='nearest')
pyplot.xticks([]), pyplot.yticks([])
pyplot.show()