import numpy as np
import matplotlib.pyplot as plt
import os

# Ścieżka do folderu z wynikami
results_dir = "./wyniki2/"
output_dir = "./wykresy3/"

# Tworzenie folderu na wykresy, jeśli nie istnieje
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# Funkcja do wczytywania danych z plików wynikowych
def load_results(file_path):
    data = np.loadtxt(file_path, delimiter=',', skiprows=0)
    return data[:, 0], data[:, 1], data[:, 3]  # n, comparisons, time

# Pliki wynikowe dla przypadków asc i desc
files_asc = [
    "results_quickoldAsc_k1.txt",
    "results_quickoldAsc_k10.txt",
    "results_quickoldAsc_k100.txt"
]

files_desc = [
    "results_quickoldDesc_k1.txt",
    "results_quickoldDesc_k10.txt",
    "results_quickoldDesc_k100.txt"
]

# Kolory i style dla różnych wartości k
colors = ["blue", "green", "red"]
labels = ["k=1", "k=10", "k=100"]

# Funkcja do generowania wykresów
def generate_plots(files, case_label):
    # Wykres liczby porównań w funkcji n^2
    fig, axes = plt.subplots(1, 3, figsize=(18, 6))  # 1 wiersz, 3 kolumny
    for ax, file, color, label in zip(axes, files, colors, labels):
        file_path = os.path.join(results_dir, file)
        if os.path.exists(file_path):
            n_values, comparisons, times = load_results(file_path)
            n_squared = n_values ** 2
            ax.plot(n_squared, comparisons, label=label, color=color, linestyle="none", marker="o")
            ax.set_xlabel("n^2")
            ax.set_ylabel("Liczba porównań")
            ax.set_title(f"Liczba porównań ({label})")
            ax.legend()
            ax.grid()

            # Obliczanie średniej stałej C
            constants = comparisons / n_squared
            avg_constant = np.mean(constants)

            # Wyświetlanie średniej stałej C na wykresie
            ax.text(0.05, 0.95, f"Średnia C: {avg_constant:.6f}",
                    transform=ax.transAxes, fontsize=10, verticalalignment='top')

    fig.suptitle(f"Liczba porównań w funkcji n^2 (QuickSort - {case_label})", fontsize=16)
    plt.tight_layout(rect=[0, 0, 1, 0.95])  # Dopasowanie układu
    plt.savefig(os.path.join(output_dir, f"comparisons_vs_n_squared_{case_label}.png"))
    plt.show()

    # Wykres czasu działania w funkcji n
    fig, axes = plt.subplots(1, 3, figsize=(18, 6))  # 1 wiersz, 3 kolumny
    for ax, file, color, label in zip(axes, files, colors, labels):
        file_path = os.path.join(results_dir, file)
        if os.path.exists(file_path):
            n_values, comparisons, times = load_results(file_path)
            ax.plot(n_values, times, label=label, color=color, linestyle="none", marker="o")
            ax.set_xlabel("n")
            ax.set_ylabel("Czas działania (s)")
            ax.set_title(f"Czas działania ({label})")
            ax.legend()
            ax.grid()

    fig.suptitle(f"Czas działania w funkcji n (QuickSort - {case_label})", fontsize=16)
    plt.tight_layout(rect=[0, 0, 1, 0.95])  # Dopasowanie układu
    plt.savefig(os.path.join(output_dir, f"time_vs_n_{case_label}.png"))
    plt.show()

# Generowanie wykresów dla przypadków asc i desc
generate_plots(files_asc, "asc")
generate_plots(files_desc, "desc")