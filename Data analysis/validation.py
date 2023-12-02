import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys

from data_cleanup import Nettoyeur
filepath = sys.path[0] + "\data.csv"
# TODO fonctions pour comparer nos résultats avec les données expérimentales

data = Nettoyeur(filepath)
data.to_csv("clean_data.csv", index=False)
