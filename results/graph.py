import sys
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
from matplotlib.collections import LineCollection
from matplotlib.colors import Normalize
from pathlib import Path

[_, file, steps, n] = sys.argv
path = Path(file)
steps = int(steps)
n = int(n)
points = np.fromfile(file, dtype=np.float64).reshape((steps, n, 2))

fig, ax = plt.subplots()
lines = []
tail = 20
t = np.flip(np.linspace(0, 10, tail))
for i in range(n):
    segments = np.array(list(zip(points[steps-tail+1:steps,i,:], points[steps-tail:steps-1,i,:])))
    lc = LineCollection(segments, cmap=plt.get_cmap('copper'), norm=Normalize(0,10), zorder=0)
    lc.set_array(t)
    lines.append(lc)
    plt.gca().add_collection(lc)
lines.append(ax.scatter(points[steps -1, :, 0], points[steps -1, :, 1], color='orange', zorder=1))

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
    t = np.flip(np.linspace(0, 10, min(frame+1, tail)))
    for i in range(n):
        lc = lines[i]
        segments = np.array(list(zip(points[max(0,frame-tail+2):frame+1,i,:], points[max(0,frame-tail+1):frame,i,:])))
        lc.set_segments(segments)
        lc.set_array(t)
    lines[n].set_offsets(points[frame, :])

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
