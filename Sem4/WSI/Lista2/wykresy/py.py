import matplotlib.pyplot as plt
import numpy as np

# Funkcja do wczytania danych z pliku
def load_data(filename):
    data = {'visited': [], 'steps': [], 'k': []}
    with open(filename, 'r') as file:
        lines = file.readlines()
        for line in lines:
            line = line.strip()
            if line and not line.endswith(":") and "Liczba stanów" not in line:
                parts = line.split()
                if len(parts) == 3:
                    visited, steps, k = map(int, parts)
                    data['visited'].append(visited)
                    data['steps'].append(steps)
                    data['k'].append(k)
    return data

# Funkcja do generowania wykresu
def plot_data(data, title, filename):
    visited = data['visited']
    steps = data['steps']

    # Obliczenie średnich
    avg_visited = np.mean(visited)
    avg_steps = np.mean(steps)

    # Wykres
    plt.figure(figsize=(12, 6))

    # Liczba odwiedzonych stanów
    plt.plot(range(len(visited)), visited, label='Liczba odwiedzonych stanów', color='blue', alpha=0.7)
    plt.axhline(avg_visited, color='blue', linestyle='--', label=f'Średnia odwiedzonych: {avg_visited:.2f}')

    # Liczba kroków
    plt.plot(range(len(steps)), steps, label='Liczba kroków', color='orange', alpha=0.7)
    plt.axhline(avg_steps, color='orange', linestyle='--', label=f'Średnia kroków: {avg_steps:.2f}')

    # Ustawienia wykresu
    plt.xlabel('Dla k=21 do k=75, 5000 iteracji')
    plt.ylabel('Wartość')
    plt.title(title)
    plt.legend()
    plt.grid(alpha=0.5)
    plt.tight_layout()

    # Zapis wykresu do pliku
    plt.savefig(filename)
    plt.close()  # Zamknij wykres, aby zwolnić pamięć

# Wczytanie danych z plików
# data1 = load_data('output4.txt')
data2 = load_data('output10.txt')

# Generowanie wykresów i zapis do plików
# plot_data(data1, 'Manhattan', 'manhattan1_plot.png')
plot_data(data2, 'Misplaced', 'misplaced_plot.png')