import numpy as np
import matplotlib.pyplot as plt
import os

# Funkcja do wczytywania danych z plików wynikowych
def load_results(file_path):
    data = np.loadtxt(file_path, delimiter=',', skiprows=0)
    return data[:, 0], data[:, 1], data[:, 2]  # n, comparisons (c), swaps (s)

# Funkcja do obliczania średnich wartości
def calculate_averages(n_values, comparisons, swaps):
    unique_n = np.unique(n_values)
    avg_comparisons = []
    avg_swaps = []
    for n in unique_n:
        mask = n_values == n
        avg_comparisons.append(np.mean(comparisons[mask]))
        avg_swaps.append(np.mean(swaps[mask]))
    return unique_n, np.array(avg_comparisons), np.array(avg_swaps)

# Ścieżki do plików wynikowych
algorithms = ["quick", "hybrid", "merge", "myMerge", "DualPivotQS"]
colors = ["blue", "green", "red", "orange", "purple"]
results_dir = "./wyniki/"
output_dir = "./wykresy/"  # Folder do zapisu wykresów
k_values = [1, 10, 100]

# Tworzenie folderu na wykresy, jeśli nie istnieje
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# Generowanie wykresów dla różnych kategorii danych
categories = ["c", "s", "c_n", "s_n"]
for category in categories:
    fig, axes = plt.subplots(1, 3, figsize=(18, 6))  # Siatka 1x3 (1 wiersz, 3 kolumny)

    for ax, (k, color) in zip(axes, zip(k_values, ["blue", "green", "red"])):  # Kolory dla różnych k
        for algo, algo_color in zip(algorithms, colors):
            file_path = os.path.join(results_dir, f"results_{algo.lower()}Random_k{k}.txt")
            if os.path.exists(file_path) and os.stat(file_path).st_size > 0:
                n_values, comparisons, swaps = load_results(file_path)
                n_unique, avg_comparisons, avg_swaps = calculate_averages(n_values, comparisons, swaps)

                # Wybór danych do rysowania
                if category == "c":
                    ax.plot(n_unique, avg_comparisons, label=f"{algo}", color=algo_color)
                elif category == "s":
                    ax.plot(n_unique, avg_swaps, label=f"{algo}", color=algo_color)
                elif category == "c_n":
                    ax.plot(n_unique, avg_comparisons / n_unique, label=f"{algo}", color=algo_color)
                elif category == "s_n":
                    ax.plot(n_unique, avg_swaps / n_unique, label=f"{algo}", color=algo_color)

        # Ustawienia dla każdego wykresu
        ax.set_title(f"{category} dla k = {k}")
        ax.set_xlabel("n (rozmiar tablicy)")
        ax.set_ylabel(f"{category}")
        ax.legend()
        ax.grid()

    # Układ i zapis wykresów
    fig.suptitle(f"Wykresy dla kategorii: {category} (Random)", fontsize=16)
    plt.tight_layout(rect=[0, 0, 1, 0.95])  # Dopasowanie układu
    plt.savefig(os.path.join(output_dir, f"Random_{category}.png"))
    #plt.show()


for category in categories:
    fig, axes = plt.subplots(1, 3, figsize=(18, 6))  # Siatka 1x3 (1 wiersz, 3 kolumny)

    for ax, (k, color) in zip(axes, zip(k_values, ["blue", "green", "red"])):  # Kolory dla różnych k
        for algo, algo_color in zip(algorithms, colors):
            file_path = os.path.join(results_dir, f"results_{algo.lower()}Asc_k{k}.txt")
            if os.path.exists(file_path) and os.stat(file_path).st_size > 0:
                n_values, comparisons, swaps = load_results(file_path)
                n_unique, avg_comparisons, avg_swaps = calculate_averages(n_values, comparisons, swaps)

                # Wybór danych do rysowania
                if category == "c":
                    ax.plot(n_unique, avg_comparisons, label=f"{algo}", color=algo_color)
                elif category == "s":
                    ax.plot(n_unique, avg_swaps, label=f"{algo}", color=algo_color)
                elif category == "c_n":
                    ax.plot(n_unique, avg_comparisons / n_unique, label=f"{algo}", color=algo_color)
                elif category == "s_n":
                    ax.plot(n_unique, avg_swaps / n_unique, label=f"{algo}", color=algo_color)

        # Ustawienia dla każdego wykresu
        ax.set_title(f"{category} dla k = {k}")
        ax.set_xlabel("n (rozmiar tablicy)")
        ax.set_ylabel(f"{category}")
        ax.legend()
        ax.grid()

    # Układ i zapis wykresów
    fig.suptitle(f"Wykresy dla kategorii: {category} (Asc)", fontsize=16)
    plt.tight_layout(rect=[0, 0, 1, 0.95])  # Dopasowanie układu
    plt.savefig(os.path.join(output_dir, f"Asc_{category}.png"))
    #plt.show()  
for category in categories:
    fig, axes = plt.subplots(1, 3, figsize=(18, 6))  # Siatka 1x3 (1 wiersz, 3 kolumny)

    for ax, (k, color) in zip(axes, zip(k_values, ["blue", "green", "red"])):  # Kolory dla różnych k
        for algo, algo_color in zip(algorithms, colors):
            file_path = os.path.join(results_dir, f"results_{algo.lower()}Desc_k{k}.txt")
            if os.path.exists(file_path) and os.stat(file_path).st_size > 0:
                n_values, comparisons, swaps = load_results(file_path)
                n_unique, avg_comparisons, avg_swaps = calculate_averages(n_values, comparisons, swaps)

                # Wybór danych do rysowania
                if category == "c":
                    ax.plot(n_unique, avg_comparisons, label=f"{algo}", color=algo_color)
                elif category == "s":
                    ax.plot(n_unique, avg_swaps, label=f"{algo}", color=algo_color)
                elif category == "c_n":
                    ax.plot(n_unique, avg_comparisons / n_unique, label=f"{algo}", color=algo_color)
                elif category == "s_n":
                    ax.plot(n_unique, avg_swaps / n_unique, label=f"{algo}", color=algo_color)

        # Ustawienia dla każdego wykresu
        ax.set_title(f"{category} dla k = {k}")
        ax.set_xlabel("n (rozmiar tablicy)")
        ax.set_ylabel(f"{category}")
        ax.legend()
        ax.grid()

    # Układ i zapis wykresów
    fig.suptitle(f"Wykresy dla kategorii: {category} (Desc)", fontsize=16)
    plt.tight_layout(rect=[0, 0, 1, 0.95])  # Dopasowanie układu
    plt.savefig(os.path.join(output_dir, f"Desc_{category}.png"))
    #plt.show()    
  

    