import matplotlib.pyplot as plt
import numpy as np

# Funkcja do wczytania danych dla układanki 3x3
def load_data_3x3(filename, heuristic):
    data = {'visited': [], 'steps': []}
    with open(filename, 'r') as file:
        lines = file.readlines()
        start_reading = False
        for line in lines:
            line = line.strip()
            if line.startswith(heuristic):  # Znajdź sekcję dla danej heurystyki
                start_reading = True
                continue
            if start_reading:
                if line == "":  # Koniec sekcji
                    break
                parts = line.split()
                if len(parts) == 2:  # Liczba stanów i liczba kroków
                    visited, steps = map(int, parts)
                    data['visited'].append(visited)
                    data['steps'].append(steps)
    return data

# Funkcja do generowania wykresu
def plot_data_3x3(data, title, filename):
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
    plt.xlabel('Przypadek')
    plt.ylabel('Wartość')
    plt.title(title)
    plt.legend()
    plt.grid(alpha=0.5)
    plt.tight_layout()

    # Zapis wykresu do pliku
    plt.savefig(filename)
    plt.close()

# Wczytanie danych z pliku
filename = 'output6.txt'
manhattan_data = load_data_3x3(filename, 'Euclidean')
# misplaced_data = load_data_3x3(filename, 'Misplaced')

# Generowanie wykresów
plot_data_3x3(manhattan_data, 'Układanka 4x4 - Euclidean', 'euclidean_4x4.png')
# plot_data_3x3(misplaced_data, 'Układanka 3x3 - Misplaced', 'misplaced_3x3.png')