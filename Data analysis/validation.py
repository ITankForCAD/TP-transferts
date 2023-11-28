import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys

from data_cleanup import Nettoyeur
filepath = sys.path[0] + "\data.csv"
# TODO fonctions pour comparer nos résultats avec les données expérimentales

data = Nettoyeur(filepath)
plt.plot(data["DATE"], data["T5"])
# plt.plot(data["DATE"], data["T4"])
# plt.plot(data["DATE"], data["T3"])
# plt.plot(data["DATE"], data["L3"])
plt.show()

