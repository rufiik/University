import matplotlib.pyplot as plt
import pandas as pd

# Wczytaj dane z plików
select_data = pd.read_csv("results_selectRand.txt", header=None, names=["n", "k", "comparisonCount", "swapCount"])
random_select_data = pd.read_csv("results_randomSelectRand.txt", header=None, names=["n", "k", "comparisonCount", "swapCount"])

# Grupowanie danych po n i obliczanie średnich
select_grouped = select_data.groupby("n").mean()
random_select_grouped = random_select_data.groupby("n").mean()

# Obliczanie ilorazów c/n i s/n
select_grouped["comparison_per_n"] = select_grouped["comparisonCount"] / select_grouped.index
select_grouped["swap_per_n"] = select_grouped["swapCount"] / select_grouped.index
random_select_grouped["comparison_per_n"] = random_select_grouped["comparisonCount"] / random_select_grouped.index
random_select_grouped["swap_per_n"] = random_select_grouped["swapCount"] / random_select_grouped.index

# Tworzenie układu 1x2 dla wykresów
fig, axes = plt.subplots(1, 2, figsize=(16, 6), sharex=True)

# Wykres ilorazu c/n
axes[0].plot(select_grouped.index, select_grouped["comparison_per_n"], label="Select - c/n", marker="o", linestyle="None")
axes[0].plot(random_select_grouped.index, random_select_grouped["comparison_per_n"], label="RandomSelect - c/n", marker="o", linestyle="None")
axes[0].set_xlabel("Rozmiar tablicy (n)")
axes[0].set_ylabel("Iloraz c/n")
axes[0].set_title("Iloraz liczby porównań do rozmiaru tablicy (c/n)")
axes[0].legend()
axes[0].grid()

# Wykres ilorazu s/n
axes[1].plot(select_grouped.index, select_grouped["swap_per_n"], label="Select - s/n", marker="o", linestyle="None")
axes[1].plot(random_select_grouped.index, random_select_grouped["swap_per_n"], label="RandomSelect - s/n", marker="o", linestyle="None")
axes[1].set_xlabel("Rozmiar tablicy (n)")
axes[1].set_ylabel("Iloraz s/n")
axes[1].set_title("Iloraz liczby zamian do rozmiaru tablicy (s/n)")
axes[1].legend()
axes[1].grid()

# Dopasowanie układu i zapis wykresu
plt.tight_layout()
plt.savefig("comparison_and_swap_per_n.png")  # Zapisz oba wykresy do jednego pliku
plt.show()