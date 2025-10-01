import matplotlib.pyplot as plt

# Wczytanie danych z pliku
thresholds = []
times = []

with open("results_hybrid.txt", "r") as file:
    for line in file:
        if line.startswith("Threshold:"):
            parts = line.split(",")
            threshold = int(parts[0].split(":")[1].strip())
            time = float(parts[1].split(":")[1].strip().replace("s", ""))
            thresholds.append(threshold)
            times.append(time)

# Obliczenie średnich czasów dla każdego progu
unique_thresholds = sorted(set(thresholds))
average_times = []

for t in unique_thresholds:
    t_times = [times[i] for i in range(len(thresholds)) if thresholds[i] == t]
    average_times.append(sum(t_times) / len(t_times))

# Znalezienie najkorzystniejszego progu
min_time = min(average_times)
optimal_threshold = unique_thresholds[average_times.index(min_time)]

# Wykres
plt.figure(figsize=(10, 6))
plt.plot(unique_thresholds, average_times, marker='o', label='Średni czas wykonania')
plt.axvline(optimal_threshold, color='r', linestyle='--', label=f'Optymalny próg: {optimal_threshold}')
plt.title('Czas wykonania HybridSort w zależności od progu przełączania')
plt.xlabel('Próg przełączania (threshold)')
plt.ylabel('Średni czas wykonania (s)')
plt.legend()
plt.grid(True)
plt.savefig("hybrid_sort_analysis.png")
plt.show()


# Wyświetlenie optymalnego progu
print(f"Najkorzystniejszy próg przełączania: {optimal_threshold}, średni czas: {min_time:.8f}s")