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

def calculate_transformed_values(n_values, avg_values, transform):
    transformed_values = [avg / transform(n) for n, avg in zip(n_values, avg_values)]
    return transformed_values

# Wczytanie danych dla p = 0.1
data_01 = read_data('wyniki.txt')
n_values_01, avg_values_01 = calculate_averages(data_01)

# Wczytanie danych dla p = 0.5
data_05 = read_data('wyniki2.txt')
n_values_05, avg_values_05 = calculate_averages(data_05)

# Tworzenie wykresów dla p = 0.1
plt.figure(figsize=(10, 6))
transformed_values_01_log_p = calculate_transformed_values(n_values_01, avg_values_01, lambda n: np.log(n) / 0.1)
plt.plot(n_values_01, transformed_values_01_log_p, 'o-', color='red', markersize=6, label='Średnia p = 0.1 / (log(n) / p)')
plt.xlabel('n')
plt.ylabel('Średnia minimalna liczba rund (Tn) / (log(n) / p)')
plt.title('Średnia minimalna liczba rund potrzebna do rozesłania informacji (p = 0.1)')
plt.legend()
plt.grid(True)
plt.show()

# Tworzenie wykresów dla p = 0.5
plt.figure(figsize=(10, 6))
transformed_values_05_log_p = calculate_transformed_values(n_values_05, avg_values_05, lambda n: np.log(n) / 0.5)
plt.plot(n_values_05, transformed_values_05_log_p, 'o-', color='red', markersize=6, label='Średnia p = 0.5 / (log(n) / p)')
plt.xlabel('n')
plt.ylabel('Średnia minimalna liczba rund (Tn) / (log(n) / p)')
plt.title('Średnia minimalna liczba rund potrzebna do rozesłania informacji (p = 0.5)')
plt.legend()
plt.grid(True)
plt.show()