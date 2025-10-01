import matplotlib.pyplot as plt
import pandas as pd

# Wczytaj dane z pliku
data = pd.read_csv("results_groups.txt", header=None, names=["n", "k", "groupSize", "comparisonCount", "swapCount", "elapsedTime"])

# Grupowanie danych po n i groupSize
grouped_data = data.groupby(["n", "groupSize"]).mean().reset_index()

# Tworzenie układu 1x3 dla wykresów
fig, axes = plt.subplots(1, 3, figsize=(18, 6), sharex=True)

# Wykres liczby porównań w zależności od n dla różnych groupSize
for group_size in grouped_data["groupSize"].unique():
    subset = grouped_data[grouped_data["groupSize"] == group_size]
    axes[0].plot(subset["n"], subset["comparisonCount"], label=f"groupSize = {group_size}", marker="o", linestyle="None")
axes[0].set_xlabel("Rozmiar tablicy (n)")
axes[0].set_ylabel("Liczba porównań")
axes[0].set_title("Liczba porównań")
axes[0].legend()
axes[0].grid()

# Wykres liczby zamian w zależności od n dla różnych groupSize
for group_size in grouped_data["groupSize"].unique():
    subset = grouped_data[grouped_data["groupSize"] == group_size]
    axes[1].plot(subset["n"], subset["swapCount"], label=f"groupSize = {group_size}", marker="o", linestyle="None")
axes[1].set_xlabel("Rozmiar tablicy (n)")
axes[1].set_ylabel("Liczba zamian")
axes[1].set_title("Liczba zamian")
axes[1].legend()
axes[1].grid()

# Wykres czasu wykonania w zależności od n dla różnych groupSize
for group_size in grouped_data["groupSize"].unique():
    subset = grouped_data[grouped_data["groupSize"] == group_size]
    axes[2].plot(subset["n"], subset["elapsedTime"], label=f"groupSize = {group_size}", marker="o", linestyle="None")
axes[2].set_xlabel("Rozmiar tablicy (n)")
axes[2].set_ylabel("Czas wykonania (s)")
axes[2].set_title("Czas wykonania")
axes[2].legend()
axes[2].grid()

# Dopasowanie układu i zapis wykresu
plt.tight_layout()
plt.savefig("Groups.png")  # Zapisz wszystkie wykresy do jednego pliku
plt.show()