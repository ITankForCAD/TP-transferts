import numpy as np
import matplotlib.pyplot as plt

from matplotlib import cm
from matplotlib.ticker import LinearLocator
from mpl_toolkits.mplot3d import Axes3D

# data = np.loadtxt("/Users/antoine/Desktop/GPH/A2023/TransfertTermique/TP/TP-transferts/C++/data.txt")
# data = np.loadtxt("C:\\Users\\xayga\\Desktop\\Code\\TP-transferts\\data.txt")
data = np.loadtxt("C++\data.txt")

x = np.linspace(0,1.6,num=data.shape[1])
t = np.linspace(0,120,num=data.shape[0])
X,T = np.meshgrid(x,t)
temp_0 = data[:, 2]
temp_8 = data[:, 20]
temp_16 = data[:, 41]

fig, ax = plt.subplots(subplot_kw={"projection": "3d"})

surface = ax.plot_surface(T, X, data, cmap=cm.coolwarm, linewidth=0, antialiased=True, alpha=0.5)
ax.plot(t, temp_0, 0.1 ,zdir="y", color="k")
ax.plot(t, temp_8, 0.8 ,zdir="y", color="k")
ax.plot(t, temp_16, 1.6 ,zdir="y", color="k")
ax.set_zlim(np.min(data), np.max(data))
ax.zaxis.set_major_locator(LinearLocator(10))
ax.zaxis.set_major_formatter("{x:.2f}")
ax.tick_params(axis='z', labelsize='smaller')
ax.tick_params(axis='x', labelsize='smaller')
ax.set_xticks([15, 30, 45, 60, 75, 90, 105, 120])
ax.tick_params(axis='y', labelsize='smaller')
fig.colorbar(surface, shrink=0.5, aspect=5, location="bottom")
ax.set_xlabel("t [min]")
ax.set_ylabel("x [m]")
ax.set_zlabel("Temp [$\degree K $]", labelpad=20)
ax.zaxis.set_rotate_label(False)
ax.view_init(elev=28, azim=-96, roll=0)
plt.show()

