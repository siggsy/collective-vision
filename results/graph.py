import sys
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation

[_, steps, n] = sys.argv
steps = int(steps)
n = int(n)
points = np.fromfile("out.bin", dtype=np.float32).reshape((steps, n, 2))

fig, ax = plt.subplots()
lines = []
for i in range(n):
    lines.append(ax.plot(points[0,i,0], points[0,i,1], )[0])
lines.append(ax.scatter(points[100, :, 0], points[100, :, 1]))

def update(frame):
    for i in range(n):
        line = lines[i];
        line.set_xdata(points[:frame, i, 0])
        line.set_ydata(points[:frame, i, 1])
    lines[n].set_offsets(points[frame, :])
    return lines

ani = animation.FuncAnimation(fig=fig, func=update, frames=steps, interval=30)
ani.save(filename="lmao.gif", writer="ffmpeg")
plt.show()
