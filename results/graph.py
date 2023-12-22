import sys
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
from itertools import cycle
from matplotlib.collections import LineCollection
from matplotlib.colors import Normalize
from pathlib import Path

[_, file] = sys.argv
path = Path(file)

# Parse input file
with open(path, 'r') as f:
    boids = list(map(int, f.readline().split(' ')))
    n     = sum(boids)
points = np.loadtxt(file, dtype=np.float32, skiprows=1, delimiter=' ').reshape((-1, n, 2))
steps = len(points)

acc = 0
for i in range(len(boids)):
    boids[i] += acc
    acc = boids[i]

# Draw last frame
fig, ax = plt.subplots()
lines = []
tail = 20
t = np.linspace(0, 10, tail)
current_boid = 0
cmap = plt.get_cmap('Oranges')

cmap_cycle = list(map(plt.get_cmap, ['Oranges', 'Blues']))
color_cycle = ['orange', 'blue']

for i in range(n):
    if i == boids[current_boid]:
        current_boid += 1
        cmap = cmap_cycle[current_boid % len(cmap_cycle)]

    segments = np.array(list(zip(points[steps-tail+1:steps,i,:], points[steps-tail:steps-1,i,:])))
    lc = LineCollection(segments, cmap=cmap, norm=Normalize(0,10), zorder=0)
    lc.set_array(t)
    lines.append(lc)
    plt.gca().add_collection(lc)

start = 0
for i in range(len(boids)):
    finish = boids[i]-1
    color = color_cycle[i % len(color_cycle)]
    lines.append(ax.scatter(points[steps-1, start:finish, 0], points[steps-1, start:finish, 1], color=color, zorder=1))
    start = finish + 1


# Camera follow logic
global boxSizes
global boxes
global index
boxes = 100
boxSizes = [0] * boxes
index = 0

def addToSizes(v):
    global boxSizes
    global index
    global boxes
    index = (index + 1) % boxes
    boxSizes[index] = v

def boxAverage():
    global boxSizes
    global boxes
    return sum(boxSizes) / boxes

def update(frame):
    t = np.linspace(0, 10, min(frame+1, tail))
    for i in range(n):
        lc = lines[i]
        segments = np.array(list(zip(points[max(0,frame-tail+2):frame+1,i,:], points[max(0,frame-tail+1):frame,i,:])))
        lc.set_segments(segments)
        lc.set_array(t)

    start = 0
    for i in range(len(boids)):
        finish = boids[i]
        lines[n+i].set_offsets(points[frame, start:finish])
        start = finish


    xs = points[frame,:,0]
    ys = points[frame,:,1]
    ylim = (np.min(ys), np.max(ys))
    xlim = (np.min(xs), np.max(xs))

    xdiff = xlim[1] - xlim[0] 
    ydiff = ylim[1] - ylim[0]
    y = np.average(ys)
    x = np.average(xs)

    if xdiff >= ydiff:
        _size = xdiff
    else:
        _size = ydiff

    addToSizes(_size)
    boxSize = boxAverage()

    xlim = (x - boxSize/2, x + boxSize/2)
    ylim = (y - boxSize/2, y + boxSize/2)

    p10 = (xlim[1] - xlim[0]) * 0.1
    xlim = (xlim[0] - p10, xlim[1] + p10)
    ylim = (ylim[0] - p10, ylim[1] + p10)

    ax.set_xlim(xlim)
    ax.set_ylim(ylim)
    return lines

ani = animation.FuncAnimation(fig=fig, func=update, frames=steps, interval=30)
ani.save(filename=f"{path.parent}/{path.stem}.mp4", writer="ffmpeg")
fig.savefig(f"{path.parent}/{path.stem}.png")
# plt.show()
