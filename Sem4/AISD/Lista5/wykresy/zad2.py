import pandas as pd
import matplotlib.pyplot as plt

# Wczytaj oba pliki (zakładam, że mają taki sam format: n rundy root)
plik1 = pd.read_csv('wyniki.csv', sep=' ', names=['n', 'rundy', 'root'], skiprows=1)
plik2 = pd.read_csv('wyniki2.csv', sep=' ', names=['n', 'rundy', 'root'], skiprows=1)

# Połącz dane
dane = pd.concat([plik1, plik2], ignore_index=True)

# Grupuj po n i licz min, max, avg
statystyki = dane.groupby('n')['rundy'].agg(['min', 'max', 'mean']).reset_index()

# Wykres
plt.figure(figsize=(10,6))
plt.plot(statystyki['n'], statystyki['min'], label='min')
plt.plot(statystyki['n'], statystyki['mean'], label='avg')
plt.plot(statystyki['n'], statystyki['max'], label='max')
plt.xlabel('Liczba wierzchołków (n)')
plt.ylabel('Liczba rund')
plt.title('Min, avg, max liczby rund rozgłaszania w drzewie MST')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('wykres_rundy.png')
plt.show()