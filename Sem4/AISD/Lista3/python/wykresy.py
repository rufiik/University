import matplotlib.pyplot as plt
import pandas as pd

# Wczytaj dane z plików
select_data = pd.read_csv("results_selectRand.txt", header=None, names=["n", "k", "comparisonCount", "swapCount"])
random_select_data = pd.read_csv("results_randomSelectRand.txt", header=None, names=["n", "k", "comparisonCount", "swapCount"])

# Grupowanie danych po n i obliczanie średnich
select_grouped = select_data.groupby("n").mean()
random_select_grouped = random_select_data.groupby("n").mean()

# Tworzenie układu 1x2 dla wykresów
fig, axes = plt.subplots(1, 2, figsize=(16, 6), sharex=True)

# Wykres liczby porównań
axes[0].plot(select_grouped.index, select_grouped["comparisonCount"], label="Select - Liczba porównań", marker="o", linestyle="None")
axes[0].plot(random_select_grouped.index, random_select_grouped["comparisonCount"], label="RandomSelect - Liczba porównań", marker="o", linestyle="None")
axes[0].set_xlabel("Rozmiar tablicy (n)")
axes[0].set_ylabel("Średnia liczba porównań")
axes[0].set_title("Porównanie liczby porównań")
axes[0].legend()
axes[0].grid()

# Wykres liczby zamian
axes[1].plot(select_grouped.index, select_grouped["swapCount"], label="Select - Liczba zamian", marker="o", linestyle="None")
axes[1].plot(random_select_grouped.index, random_select_grouped["swapCount"], label="RandomSelect - Liczba zamian", marker="o", linestyle="None")
axes[1].set_xlabel("Rozmiar tablicy (n)")
axes[1].set_ylabel("Średnia liczba zamian")
axes[1].set_title("Porównanie liczby zamian")
axes[1].legend()
axes[1].grid()

# Dopasowanie układu i zapis wykresu
plt.tight_layout()
plt.savefig("comparison_and_swap_selects.png")  # Zapisz oba wykresy do jednego pliku
plt.show()