import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib import cm
from matplotlib.ticker import LinearLocator
from mpl_toolkits.mplot3d import Axes3D
import os
import json

analyser_path = os.path.dirname(os.path.abspath(__file__))
data_path = os.path.join(os.path.split(analyser_path)[0],os.path.join("C++","data.txt"))
json_path = os.path.join(os.path.split(analyser_path)[0],os.path.join("C++","attribute.json"))
clean_data_path = os.path.join(analyser_path,"clean_data.csv")

data = np.loadtxt(data_path)
data_exp = pd.read_csv(clean_data_path)

with open(json_path) as f:
    att = json.load(f)

temps_de_phase = att["temps_de_phase"]

x = np.linspace(0,att["longueur"],num=data.shape[1])
t = np.linspace(0,att["temps_simulation"]/60,num=data.shape[0])
X,T = np.meshgrid(x,t)

temp_0 = data[:, att["Qn"]//6 + att["Qn"]//15]
temp_8 = data[:, att["Qn"]//2 + att["Qn"]//10 +1]
temp_16 = data[:,-att["Qn"]//6 ]

def plot3D():
    fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
    surface = ax.plot_surface(T, X, data, cmap=cm.coolwarm, linewidth=0.3, antialiased=True, alpha=0.5)

    ax.plot(t, temp_0, x[att["Qn"]//6 + att["Qn"]//15] ,zdir="y", color="k")
    ax.plot(t, temp_8, x[att["Qn"]//2 + att["Qn"]//10 +1] ,zdir="y", color="k")
    ax.plot(t, temp_16, x[-att["Qn"]//6] ,zdir="y", color="k")

    ax.set_zlim(np.min(data), np.max(data))
    ax.zaxis.set_major_locator(LinearLocator(10))
    ax.zaxis.set_major_formatter("{x:.1f}")
    ax.tick_params(axis='z', labelsize='smaller', pad=7.0)
    ax.tick_params(axis='x', labelsize='smaller')
    ax.tick_params(axis='y', labelsize='smaller')
    fig.colorbar(surface, shrink=0.5, aspect=5, location="bottom")
    ax.set_xlabel("Temps [min]")
    ax.set_ylabel("Distance [m]", labelpad=10)
    ax.set_zlabel("Température [$\degree K $]", labelpad=30)
    ax.zaxis.set_rotate_label(False)
    ax.yaxis.set_rotate_label(False)
    ax.view_init(elev=25, azim=-113, roll=0)
    plt.show()
    

print(data_exp["DATE"].shape[0], att["temps_simulation"])
time_exp = np.linspace(0, data_exp["DATE"].shape[0]*10/60, num=data_exp["DATE"].shape[0])
print(time_exp.shape)
T3 = np.array(data_exp["T3"]+273)
T4 = np.array(data_exp["T4"]+273)
T5 = np.array(data_exp["T5"]+273)

def profil():
    fig, axs = plt.subplots(ncols=1, figsize=(5,4))
    fig.suptitle("Profils de température")
    axs.set_ylim(np.min(data), np.max(data))
    axs.yaxis.set_major_locator(LinearLocator(10))
    axs.yaxis.set_major_formatter("{x:.1f}")
    axs.set_xlabel("Temps [min]")
    axs.set_ylabel("Température [$\degree K $]", labelpad=30)
    axs.plot(t, temp_0,linestyle="--" ,color="b", label="Température simulée à T3")
    axs.plot(t, temp_8,linestyle="--" ,color="orange", label="Température simulée à T4")
    axs.plot(t, temp_16,linestyle="--" ,color="g", label="Température simulée à T5")
    
    # axs[1].yaxis.set_major_locator(LinearLocator(10))
    # axs[1].yaxis.set_major_formatter("{x:.1f}")
    # axs[1].set_xlabel("Temps [min]")
    # axs[1].set_ylabel("Temp [$\degree K $]", labelpad=30, horizontalalignment="right")
    axs.plot(time_exp, T3,label="Température à T3")
    axs.plot(time_exp, T4,label="Température à T4")
    axs.plot(time_exp, T5,label="Température à T5")
    axs.legend()
    plt.show()
    
#profil()
plot3D()
