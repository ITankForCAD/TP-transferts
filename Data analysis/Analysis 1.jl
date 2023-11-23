import CSV, DataFrames, Plots
using CSV, DataFrames, Plots


""" Temperature analysis of the data obtained from combustion chamber sensors """

df = DataFrame(CSV.File("Data analysis\\data.csv"))
TimeVector = range(start=0, stop=7200, length=721)
