import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


def Nettoyeur(file: str):
    clean_data = []
    try:
        if '.csv' in file:
            try:
                df = pd.read_csv(file, delimiter=';', decimal=',', parse_dates=["DATE"])
            except FileNotFoundError as error:
                print(error)
                print("Wrong filename or incomplete filepath")
            for headers in df.columns[:]:
                if headers != "DATE":
                    data = df[headers]
                    if headers != "Text":
                        array = []
                        for i in range(1, len(data)):
                            array.append(float(data[i].strip().replace(",", ".")))
                        clean_data.append(array)
                    if headers == "Text":
                        ar = []
                        for k in range(1, len(data)):
                            ar.append(data[k])
                        clean_data.insert(2, ar)
                if headers == "DATE":
                    time = df["DATE"]
                    arr = []
                    for j in range(1, len(time)):
                        arr.append(time[j])
                    clean_data.insert(0, arr)
            dic = {h:clean_data[a] for a, h in enumerate(df.columns)}
            new_dataframe = pd.DataFrame(data=dic)
            return new_dataframe
    except UnboundLocalError as error:
        print(error)