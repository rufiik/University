import pandas as pd
import matplotlib.pyplot as plt

# Wczytaj dane z pliku wyniki.txt
data = pd.read_csv('wyniki.txt', delimiter=';')

# Oblicz średnią liczbę porównań i przestawień dla każdego n
average_comparisons = data.groupby('n')['comparisonCount'].mean()
average_swaps = data.groupby('n')['swapCount'].mean()
plt.figure(figsize=(10, 5))
plt.scatter(data['n'], data['comparisonCount'], label='Liczba porównań dla różnych n', color='blue', alpha=0.5 ,s=20)
plt.plot(average_comparisons.index, average_comparisons.values, label='Średnia liczba porównań', color='red')
plt.xlabel('n')
plt.ylabel('Liczba porównań', fontsize=14)
plt.title('Liczba porównań dla różnych n')
plt.legend()
plt.grid(True)
plt.show()

plt.figure(figsize=(10, 5))
plt.scatter(data['n'], data['swapCount'], label='Liczba przestawień dla różnych n', color='blue', alpha=0.5, s=20)
plt.plot(average_swaps.index, average_swaps.values, label='Średnia liczba przestawień', color='red')
plt.xlabel('n')
plt.ylabel('Liczba przestawień', fontsize=14)
plt.title('Liczba przestawień dla różnych n ')
plt.legend()
plt.grid(True)
plt.show()

# # Wykres liczby porównań jako funkcji n
# plt.figure(figsize=(10, 5))
# plt.plot(average_comparisons.index, average_comparisons.values, label='Średnia liczba porównań')
# plt.xlabel('n')
# plt.ylabel('Średnia liczba porównań', fontsize=14)
# plt.title('Średnia liczba porównań cmp(n)')
# plt.legend()
# plt.grid(True)
# plt.show()

# # Wykres liczby przestawień jako funkcji n
# plt.figure(figsize=(10, 5))
# plt.plot(average_swaps.index, average_swaps.values, label='Średnia liczba przestawień', color='orange')
# plt.xlabel('n')
# plt.ylabel('Średnia liczba przestawień', fontsize=14)
# plt.title('Średnia liczba przestawień s(n)')
# plt.legend()
# plt.grid(True)
# plt.show()

# Wykres cmp(n)/n
plt.figure(figsize=(10, 5))
plt.plot(average_comparisons.index, average_comparisons.values / average_comparisons.index, label='cmp(n)/n')
plt.xlabel('n')
plt.ylabel('cmp(n)/n', fontsize=14)
plt.title('cmp(n)/n')
plt.legend()
plt.grid(True)
plt.show()

# Wykres cmp(n)/n^2
plt.figure(figsize=(10, 5))
plt.plot(average_comparisons.index, average_comparisons.values / (average_comparisons.index ** 2), label='cmp(n)/n^2')
plt.xlabel('n')
plt.ylabel('cmp(n)/n^2', fontsize=14)
plt.title('cmp(n)/n^2')
plt.legend()
plt.grid(True)
plt.show()

# Wykres s(n)/n
plt.figure(figsize=(10, 5))
plt.plot(average_swaps.index, average_swaps.values / average_swaps.index, label='s(n)/n', color='green')
plt.xlabel('n')
plt.ylabel('s(n)/n', fontsize=14)
plt.title('s(n)/n')
plt.legend()
plt.grid(True)
plt.show()

# Wykres s(n)/n^2
plt.figure(figsize=(10, 5))
plt.plot(average_swaps.index, average_swaps.values / (average_swaps.index ** 2), label='s(n)/n^2', color='red')
plt.xlabel('n')
plt.ylabel('s(n)/n^2', fontsize=14)
plt.title('s(n)/n^2')
plt.legend()
plt.grid(True)
plt.show()