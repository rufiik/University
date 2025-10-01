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

# Podzielenie danych przez log2(n)
random_grouped["comparisonCount_div_log"] = random_grouped["comparisonCount"] / random_grouped["log_n"]
random_grouped["elapsedTime_div_log"] = random_grouped["elapsedTime"] / random_grouped["log_n"]

specific_grouped["comparisonCount_div_log"] = specific_grouped["comparisonCount"] / specific_grouped["log_n"]
specific_grouped["elapsedTime_div_log"] = specific_grouped["elapsedTime"] / specific_grouped["log_n"]

# Obliczenie średnich
random_comparison_mean = random_grouped["comparisonCount_div_log"].mean()
specific_comparison_mean = specific_grouped["comparisonCount_div_log"].mean()

random_time_mean = random_grouped["elapsedTime_div_log"].mean()
specific_time_mean = specific_grouped["elapsedTime_div_log"].mean()

# Tworzenie wykresów na jednym panelu
fig, axs = plt.subplots(1, 2, figsize=(16, 6), sharex=True)

# Wykres liczby porównań podzielonej przez log2(n)
axs[0].plot(random_grouped.index, random_grouped["comparisonCount_div_log"], label="Losowe elementy - Liczba porównań / log2(n)", marker="o", linestyle="None")
axs[0].plot(specific_grouped.index, specific_grouped["comparisonCount_div_log"], label="Specyficzne elementy - Liczba porównań / log2(n)", marker="o", linestyle="None")
axs[0].axhline(y=random_comparison_mean, color="blue", linestyle="--", label=f"Średnia losowe: {random_comparison_mean:.2f}")
axs[0].axhline(y=specific_comparison_mean, color="orange", linestyle="--", label=f"Średnia specyficzne: {specific_comparison_mean:.2f}")
axs[0].set_xlabel("Rozmiar tablicy (n)")
axs[0].set_ylabel("Liczba porównań / log2(n)")
axs[0].set_title("Liczba porównań podzielona przez log2(n) z zaznaczoną średnią")
axs[0].legend()
axs[0].grid()

# Wykres czasu wykonania podzielonego przez log2(n)
axs[1].plot(random_grouped.index, random_grouped["elapsedTime_div_log"], label="Losowe elementy - Czas wykonania / log2(n)", marker="o", linestyle="None")
axs[1].plot(specific_grouped.index, specific_grouped["elapsedTime_div_log"], label="Specyficzne elementy - Czas wykonania / log2(n)", marker="o", linestyle="None")
axs[1].axhline(y=random_time_mean, color="blue", linestyle="--", label=f"Średnia losowe: {random_time_mean:.2e}")
axs[1].axhline(y=specific_time_mean, color="orange", linestyle="--", label=f"Średnia specyficzne: {specific_time_mean:.2e}")
axs[1].set_xlabel("Rozmiar tablicy (n)")
axs[1].set_ylabel("Czas wykonania / log2(n)")
axs[1].set_title("Czas wykonania podzielony przez log2(n) z zaznaczoną średnią")
axs[1].legend()
axs[1].grid()

# Zapisanie i wyświetlenie wykresów
plt.tight_layout()
plt.savefig("combined_plots.png")
plt.show()