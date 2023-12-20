import sys
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

PI = np.pi
RAD = PI/180.0
DEG = 180.0/PI

[_, file, colored] = sys.argv
path = Path(file)
P = np.fromfile(file, dtype=np.float64)

if (colored != "-c"):
	colored = False
	P = P.reshape((-1, 2))
else:
	colored = True
	P = P.reshape((-1, 3))


def getColor(col):
	if (col == 0):
		return "blue"
	elif (col == 1):
		return "red"
	elif (col == 2):
		return "green"
	elif (col == 3):
		return "black"




def drawP(P: list[tuple[float,float]]):
	x = [0]
	y = [0]
	
	for p in P:
		a = p[0]*DEG
		b = p[1]*DEG
		
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
	
	x.append(360)
	y.append(0)
	
	plt.fill(x, y, color="blue", alpha=0.5)
	plt.plot(x, y, color="blue")
	return x, y


def drawColoredP(P: list[tuple[float,float,float]]):
	xy = {}
	
	for p in P:
		a = p[0]*DEG
		b = p[1]*DEG
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
		x.append(360)
		y.append(0)
		plt.fill(x, y, color=color, alpha=0.5)
		plt.plot(x, y, color=color)


print(P)
if (not colored):
	drawP(P)
else:
	drawColoredP(P)

plt.tight_layout()
plt.show()