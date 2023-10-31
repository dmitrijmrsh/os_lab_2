import matplotlib.pyplot as plt
import csv

X = []
Y = []

with open('Data.csv', 'r') as datafile:
    plotting = csv.reader(datafile, delimiter=';')

    for ROWS in plotting:
        X.append(float(ROWS[0]))
        Y.append(float(ROWS[1]))

plt.plot(X, Y)
plt.xlim([0, 15])
plt.ylim([0, 35000])
plt.ylabel(r'Время, мc', fontsize = 12)
plt.xlabel(r'Количество потоков', fontsize = 12)
plt.title(r'Изменение времени от числа потоков')
plt.grid(True)
plt.show()