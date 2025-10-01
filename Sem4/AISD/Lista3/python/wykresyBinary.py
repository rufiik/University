import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Wczytaj dane z plików
random_data = pd.read_csv("results_binary_random.txt", header=None, names=["n", "v", "comparisonCount", "elapsedTime", "found"], sep=" ")
specific_data = pd.read_csv("results_binary.txt", header=None, names=["n", "v", "comparisonCount", "elapsedTime", "found"], sep=" ")

# Grupowanie danych po n i obliczanie średnich
random_grouped = random_data.groupby("n").mean()
specific_grouped = specific_data.groupby("n").mean()

# Teoretyczne oszacowanie liczby porównań (log2(n))
random_grouped["log_n"] = np.log2(random_grouped.index)
specific_grouped["log_n"] = np.log2(specific_grouped.index)

# Tworzenie wykresu z dwiema osiami Y
fig, ax1 = plt.subplots(figsize=(12, 6))

# Oś Y dla liczby porównań
ax1.set_xlabel("Rozmiar tablicy (n)")
ax1.set_ylabel("Średnia liczba porównań", color="tab:blue")
ax1.plot(random_grouped.index, random_grouped["comparisonCount"], label="Losowe elementy - Liczba porównań", marker="o", linestyle="None", color="tab:blue")
ax1.plot(specific_grouped.index, specific_grouped["comparisonCount"], label="Specyficzne elementy - Liczba porównań", marker="o", linestyle="None", color="tab:cyan")
ax1.plot(random_grouped.index, random_grouped["log_n"], label="Teoretyczne O(log n)", linestyle="--", color="tab:purple")
ax1.tick_params(axis="y", labelcolor="tab:blue")
ax1.legend(loc="upper left")
ax1.grid()

# Oś Y dla czasu wykonania
ax2 = ax1.twinx()  # Tworzenie drugiej osi Y
ax2.set_ylabel("Średni czas wykonania (s)", color="tab:red")
ax2.plot(random_grouped.index, random_grouped["elapsedTime"], label="Losowe elementy - Czas wykonania", marker="o", linestyle="None", color="tab:red")
ax2.plot(specific_grouped.index, specific_grouped["elapsedTime"], label="Specyficzne elementy - Czas wykonania", marker="o", linestyle="None", color="tab:orange")
ax2.tick_params(axis="y", labelcolor="tab:red")
ax2.legend(loc="upper right")

# Tytuł i zapis wykresu
plt.title("Porównanie liczby porównań i czasu wykonania na jednym wykresie")
plt.tight_layout()
plt.savefig("combined_comparison_execution_time.png")
plt.show()