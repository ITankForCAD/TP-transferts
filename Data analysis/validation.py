import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys

from data_cleanup import Nettoyeur
filepath = sys.path[0] + "\data.csv"
# TODO fonctions pour comparer nos résultats avec les données expérimentales

data = Nettoyeur(filepath)

plt.plot(data["DATE"], data["V3 (in 3)"])
plt.plot(data["DATE"], data["V6 (out 3)"])
plt.show()