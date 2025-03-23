import matplotlib.pyplot as plt
import numpy as np

def read_data(filename):
    data = {}
    with open(filename, 'r') as file:
        next(file)  # Pomijamy nagłówek
        for line in file:
            n, min_val, p = line.strip().split(';')
            n = int(n)
            min_val = int(min_val)
            if n not in data:
                data[n] = []
            data[n].append(min_val)
    return data

def calculate_averages(data):
    n_values = sorted(data.keys())
    avg_values = [np.mean(data[n]) for n in n_values]
    return n_values, avg_values

# Wczytanie danych dla p = 0.1
data_01 = read_data('wyniki.txt')
n_values_01, avg_values_01 = calculate_averages(data_01)

# Wczytanie danych dla p = 0.5
data_05 = read_data('wyniki2.txt')
n_values_05, avg_values_05 = calculate_averages(data_05)

# Tworzenie wykresu dla p = 0.1
plt.figure(figsize=(10, 6))
for n in n_values_01:
    plt.plot([n] * len(data_01[n]), data_01[n], 'o',markersize=4, color='blue', label='p = 0.1' if n == n_values_01[0] else "")
plt.plot(n_values_01, avg_values_01, 'o', color='red',markersize=6, label='Średnia p = 0.1')
plt.xlabel('n')
plt.ylabel('Minimalna liczba rund (Tn)')
plt.title('Minimalna liczba rund potrzebna do rozesłania informacji (p = 0.1)')
plt.legend()
plt.grid(True)
plt.show()

# Tworzenie wykresu dla p = 0.5
plt.figure(figsize=(10, 6))
for n in n_values_05:
    plt.plot([n] * len(data_05[n]), data_05[n], 'o',markersize=4, color='blue', label='p = 0.5' if n == n_values_05[0] else "")
plt.plot(n_values_05, avg_values_05, 'o', color='red',markersize=6, label='Średnia p = 0.5')
plt.xlabel('n')
plt.ylabel('Minimalna liczba rund (Tn)')
plt.title('Minimalna liczba rund potrzebna do rozesłania informacji (p = 0.5)')
plt.legend()
plt.grid(True)
plt.show()