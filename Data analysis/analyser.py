import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib import cm
from matplotlib.ticker import LinearLocator
from mpl_toolkits.mplot3d import Axes3D

# data = np.loadtxt("/Users/antoine/Desktop/GPH/A2023/TransfertTermique/TP/TP-transferts/C++/data.txt")
data = np.loadtxt("C:\\Users\\xayga\\Desktop\\Code\\TP-transferts\\data.txt")
data_exp = pd.read_csv("C:\\Users\\xayga\\Desktop\\Code\\TP-transferts\\clean_data.csv")


x = np.linspace(0,1.6,num=data.shape[1])
t = np.linspace(0,120,num=data.shape[0])
X,T = np.meshgrid(x,t)

def plot3D():
    fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
    surface = ax.plot_surface(T, X, data, cmap=cm.coolwarm, linewidth=0, antialiased=True, alpha=0.5)
    ax.set_zlim(np.min(data), np.max(data))
    ax.zaxis.set_major_locator(LinearLocator(10))
    ax.zaxis.set_major_formatter("{x:.1f}")
    ax.tick_params(axis='z', labelsize='smaller', pad=7.0)
    ax.tick_params(axis='x', labelsize='smaller')
    ax.tick_params(axis='y', labelsize='smaller')
    fig.colorbar(surface, shrink=0.5, aspect=5, location="bottom")
    ax.set_xlabel("t [min]")
    ax.set_ylabel("x [m]", labelpad=10)
    ax.set_zlabel("Temp [$\degree K $]", labelpad=30)
    ax.zaxis.set_rotate_label(False)
    ax.yaxis.set_rotate_label(False)
    ax.view_init(elev=25, azim=-113, roll=0)
    plt.show()
    
temp_0 = data[:, 2]
temp_8 = data[:, 20]
temp_16 = data[:, 41]

time_exp = np.linspace(0, 120, num=data_exp["DATE"].shape[0])
T3 = np.array(data_exp["T3"]+273)
T4 = np.array(data_exp["T4"]+273)
T5 = np.array(data_exp["T5"]+273)

def profil():
    fig, axs = plt.subplots(ncols=2, figsize=(5,4))
    fig.suptitle("Profils de température")
    axs[0].set_ylim(np.min(data), np.max(data))
    axs[0].yaxis.set_major_locator(LinearLocator(10))
    axs[0].yaxis.set_major_formatter("{x:.1f}")
    axs[0].set_xlabel("Temps [min]")
    axs[0].set_ylabel("Temp [$\degree K $]", labelpad=30)
    # axs[0].plot(t, temp_0, color="b")
    # axs[0].plot(t, temp_8, color="orange")
    axs[0].plot(t, temp_16, color="g")
    
    axs[1].yaxis.set_major_locator(LinearLocator(10))
    axs[1].yaxis.set_major_formatter("{x:.1f}")
    axs[1].set_xlabel("Temps [min]")
    axs[1].set_ylabel("Temp [$\degree K $]", labelpad=30, horizontalalignment="right")
    axs[1].plot(time_exp, T3)
    axs[1].plot(time_exp, T4)
    axs[1].plot(time_exp, T5)
    plt.show()
    
profil()
# plot3D()