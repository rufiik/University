import pandas as pd
import matplotlib.pyplot as plt

# Wczytaj dane
dane = pd.read_csv('czasy2.csv', skiprows=1, names=['n', 'rep_id', 'kruskal_ms', 'prim_ms'])

# Grupuj po n i licz min, max, avg osobno dla Kruskal i Prim
stat = dane.groupby('n').agg({
    'kruskal_ms': ['min', 'max', 'mean', 'count'],
    'prim_ms': ['min', 'max', 'mean', 'count']
}).reset_index()

# Popraw nagłówki kolumn
stat.columns = ['n', 'kruskal_min', 'kruskal_max', 'kruskal_avg', 'kruskal_count',
                'prim_min', 'prim_max', 'prim_avg', 'prim_count']

# Wypisz ile powtórzeń jest dla każdego n (kontrola)
# print(stat[['n', 'kruskal_count', 'prim_count']])

# Wykresy
plt.figure(figsize=(12, 6))
plt.plot(stat['n'], stat['kruskal_min'], label='Kruskal min')
plt.plot(stat['n'], stat['kruskal_avg'], label='Kruskal avg')
plt.plot(stat['n'], stat['kruskal_max'], label='Kruskal max')
plt.plot(stat['n'], stat['prim_min'], label='Prim min')
plt.plot(stat['n'], stat['prim_avg'], label='Prim avg')
plt.plot(stat['n'], stat['prim_max'], label='Prim max')
plt.xlabel('Liczba wierzchołków (n)')
plt.ylabel('Czas [ms]')
plt.title('Min, avg, max czasów Kruskal i Prim vs liczba wierzchołków')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('czasy_wykres.png')
plt.show()