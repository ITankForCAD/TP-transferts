import numpy as np
import matplotlib.pyplot as plt

from matplotlib import cm
from matplotlib.ticker import LinearLocator
from mpl_toolkits.mplot3d import Axes3D

# data = np.loadtxt("/Users/antoine/Desktop/GPH/A2023/TransfertTermique/TP/TP-transferts/C++/data.txt")
data = np.loadtxt("C:\\Users\\xayga\\Desktop\\Code\\TP-transferts\\data.txt")
x = np.linspace(0,1.6,num=data.shape[1])
t = np.linspace(0,data.shape[0],num=data.shape[0])
X,T = np.meshgrid(x,t)


fig, ax = plt.subplots(subplot_kw={"projection": "3d"})

surface = ax.plot_surface(T, X, data, cmap=cm.coolwarm, linewidth=0, antialiased=True)
ax.set_zlim(np.min(data), np.max(data))
ax.zaxis.set_major_locator(LinearLocator(10))
ax.zaxis.set_major_formatter("{x:.2f}")
ax.tick_params(axis='z', labelsize='smaller')
ax.tick_params(axis='x', labelsize='smaller')
ax.tick_params(axis='y', labelsize='smaller')
fig.colorbar(surface, shrink=0.5, aspect=5, location="bottom")
ax.set_xlabel("t [s]")
ax.set_ylabel("x [m]")
ax.set_zlabel("Temp [$\degree K $]", labelpad=20)
ax.zaxis.set_rotate_label(False)
plt.show()

