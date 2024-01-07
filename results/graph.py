import sys
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
from matplotlib.collections import LineCollection
from matplotlib.colors import Normalize
from pathlib import Path


PI = np.pi
RAD = PI/180.0
DEG = 180.0/PI


def getColor(col):
	if (col == 0):
		return "orange"
	elif (col == 1):
		return "blue"
	elif (col == 2):
		return "red"
	elif (col == 3):
		return "green"

def drawColoredP(ax, P: list[tuple[float,float,int]]):
    xy = {}

    for p in P:
        a = p[0]
        b = p[1]
        color = getColor(p[2])

        l = xy.get(color)
        if (l == None):
            l = xy[color] = ([0],[0])
        x,y = l		

        # From
        x.append(a)
        y.append(0)
        x.append(a)
        y.append(1)

        # To
        x.append(b)
        y.append(1)
        x.append(b)
        y.append(0)

    for color in xy:
        x,y = xy[color]
        x.append(np.pi*2)
        y.append(0)
        ax.set_xticks(
            [0, PI/2, PI, PI*3.0/2.0, 2*PI],
            ["0", r'$\frac{%s}{%s}$'%(r'\pi',2), r'$\pi$', r'$\frac{3%s}{%s}$'%(r'\pi',4), r'$2\pi$']
        )
        ax.fill(x, y, color=color, alpha=0.5)
        ax.plot(x, y, color=color)

[_, file] = sys.argv
path = Path(file)
p_path = Path(file + ".p")

# Parse input file
with open(path, 'r') as f:
    boids = list(map(int, f.readline().split(' ')))
    n     = sum(boids)
points = np.loadtxt(file, dtype=np.float32, skiprows=1, delimiter=' ').reshape((-1, n, 2))
steps = len(points)

# Parse projection field of a first boid
with open(p_path, 'r') as f:
    p = []
    for i in range(steps):
        step = []
        intervals = f.readline().split(' ')[0].split(',')
        for j in range(3, len(intervals), 3):
            step.append((float(intervals[j]), float(intervals[j+1]), int(intervals[j+2])))
        
        p.append(step)
P = p

acc = 0
for i in range(len(boids)):
    boids[i] += acc
    acc = boids[i]

# Draw last frame
fig, ax = plt.subplots(2, 1, height_ratios=(0.75, 0.25))
lines = []
tail = 20
t = np.linspace(0, 10, tail)
current_boid = 0

cmap_cycle = list(map(plt.get_cmap, ['Oranges', 'Blues']))
color_cycle = ['orange','blue']
cmap = cmap_cycle[0]

for i in range(n):
    if i == boids[current_boid]:
        current_boid += 1
        cmap = cmap_cycle[current_boid % len(cmap_cycle)]
    segments = np.array(list(zip(points[steps-tail+1:steps,i,:], points[steps-tail:steps-1,i,:])))
    lc = LineCollection(segments, cmap=cmap, norm=Normalize(0,10), zorder=0)
    lc.set_array(t)
    lines.append(lc)
    ax[0].add_collection(lc)

start = 0

lines.append(ax[0].scatter(points[steps-1, 0, 0], points[steps-1, 0, 1], color='orange', edgecolors='black', zorder=2))
for i in range(len(boids)):
    finish = boids[i]-1
    color = color_cycle[i % len(color_cycle)]
    lines.append(ax[0].scatter(points[steps-1, start:finish, 0], points[steps-1, start:finish, 1], color=color, zorder=1))
    start = finish + 1

drawColoredP(ax[1], P[steps-1])

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
    lines[n].set_offsets(points[frame,0])
    for i in range(len(boids)):
        finish = boids[i]
        lines[n+i + 1].set_offsets(points[frame, start:finish])
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

    ax[0].set_xlim(xlim)
    ax[0].set_ylim(ylim)
    ax[1].clear()
    drawColoredP(ax[1], P[frame])
    return lines


ani = animation.FuncAnimation(fig=fig, func=update, frames=steps, interval=30)
ani.save(filename=f"{path.parent}/{path.stem}.mp4", writer="ffmpeg")
fig.savefig(f"{path.parent}/{path.stem}.png")
# plt.show()
