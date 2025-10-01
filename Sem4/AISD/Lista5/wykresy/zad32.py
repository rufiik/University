import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('total_comparisons_vs_n.csv')
df['total_ops'] = 2*df['n'] + 1 + 2*df['n']
df['avg_comparisons'] = df['total_comparisons'] / df['total_ops']

plt.figure(figsize=(10,6))
plt.scatter(df['n'], df['avg_comparisons'], marker='o', label='Średnia liczba porównań')
plt.plot(df['n'], np.log2(df['n']), color='red', linestyle='--', label='log₂(n)')

plt.xlabel('n')
plt.ylabel('Średnia liczba porównań na operację')
plt.title('Średni koszt operacji w kopcu dwumianowym w zależności od n')
plt.legend()
plt.grid(True)
plt.savefig('zad3b.png')
plt.show()