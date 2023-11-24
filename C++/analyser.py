import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
data = np.loadtxt("data.txt")
print(data.shape)
x = np.linspace(0,data.shape[1],num=data.shape[1])
t = np.linspace(0,data.shape[0],num=data.shape[0])
X,T = np.meshgrid(x,t)

fig = plt.figure()
ax = plt.axes(projection='3d')
ax.plot_wireframe(X, T, data, cmap='binary')
ax.set_xlabel('x')
ax.set_ylabel('t')
ax.set_zlabel('z')
plt.show()

