import matplotlib.pyplot as plt
import numpy as np


PI = np.pi
DEG_TO_RAD = PI/180.0


def drawP(P: list[tuple[float,float]]):
	x = [0]
	y = [0]
	
	for p in P:
		a = p[0]*DEG_TO_RAD
		b = p[1]*DEG_TO_RAD
		
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
	
	x.append(2*PI)
	y.append(0)
	
	plt.fill(x, y, color="blue", alpha=0.5, label=r'$P(\varphi)$')
	plt.plot(x, y, color="blue", label='_nolegend_')
	return x, y


def drawCos():
	x = np.arange(0, 2*PI, 0.01)
	y = np.sin(x)
	plt.fill(x, y, color="red", alpha=0.5, label=r'$cos(\varphi)$')
	plt.plot(x, y, color="red", label='_nolegend_')


def drawCosP(P: list[tuple[float,float]]):
	x = []
	y = []
	
	for p in np.arange(0, 2*PI, 0.01):
		if (len(P) <= 0):
			x.append(p)
			y.append(0)
			continue
			
		a = P[0][0]*DEG_TO_RAD
		b = P[0][1]*DEG_TO_RAD
		
		x.append(p)
		if (p < a):
			y.append(0)
		elif (a <= p and p <= b):
			y.append(np.sin(p))
		elif (p > b):
			y.append(0)
			del P[0]
			
	
	
	plt.fill(x, y, color="green", alpha=0.5, label=r'$cos(\varphi)P(\varphi)$')
	plt.plot(x, y, color="green", label='_nolegend_')
	
	
	


P = [
	(20,60),
	(100,220),
	(300,310)
]

_, ax = plt.subplots(2, 1, sharex=True)
ax[0].set_xticks(
	[0, PI/2, PI, PI*3.0/2.0, 2*PI],
	["0", r'$\frac{%s}{%s}$'%(r'\pi',2), r'$\pi$', r'$\frac{3%s}{%s}$'%(r'\pi',4), r'$2\pi$']
)

plt.subplot(2, 1, 1)
drawCos()
drawP(P)
plt.legend(loc="lower left")

plt.subplot(2, 1, 2)
drawCosP(P)
plt.legend(loc="lower left")


plt.tight_layout()
plt.savefig('bin/integral.png', dpi=300)
plt.show()