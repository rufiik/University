import numpy as np
import matplotlib.pyplot as plt
import os

# Ścieżka do katalogu z wynikami
results_dir = "./wyniki2/"

# Funkcja do wczytywania danych z plików wynikowych
def load_results(file_pattern):
    data = []
    for file_name in os.listdir(results_dir):
        if file_pattern in file_name:  # Dopasowanie wzorca w nazwie pliku
            file_path = os.path.join(results_dir, file_name)
            if os.path.isfile(file_path) and os.stat(file_path).st_size > 0:
                # Wczytaj dane z pliku
                file_data = np.loadtxt(file_path, delimiter=",")
                data.append(file_data)
    if len(data) > 0:
        return np.vstack(data)  # Połącz dane z wielu plików
    else:
        raise FileNotFoundError(f"Nie znaleziono plików pasujących do wzorca: {file_pattern}")

# Wczytaj dane dla DualPivotQS
try:
    random_data = load_results("results_dualPivotQSRandom")
    asc_data = load_results("results_dualPivotQSAsc")
    desc_data = load_results("results_dualPivotQSDesc")
except FileNotFoundError as e:
    print(e)
    exit(1)

# Analiza danych
def analyze_data(data, title, output_file):
    n_values = data[:, 0]  # Pierwsza kolumna: n
    comparison_counts = data[:, 1]  # Druga kolumna: liczba porównań

    # Oblicz n * log2(n)
    n_log_n = n_values * np.log2(n_values)

    # Wyznacz stałą C dla każdej wartości n
    C_values = comparison_counts / n_log_n

    # Oblicz średnią wartość stałej C
    C_mean = np.mean(C_values)

    # Wyświetl wyniki
    print(f"{title}: Średnia wartość stałej C = {C_mean:.4f}")

    # Wykres liczby porównań w zależności od n * log2(n)
    plt.figure(figsize=(10, 6))
    plt.plot(n_log_n, comparison_counts, 'o-', label="Liczba porównań")
    plt.plot(n_log_n, C_mean * n_log_n, 'r--', label=f"Dopasowanie: C = {C_mean:.4f}")
    plt.xlabel("n * log2(n)")
    plt.ylabel("Liczba porównań")
    plt.title(title)
    plt.legend()
    plt.grid()
    plt.savefig(output_file)
    plt.show()

# Analiza dla różnych typów danych
analyze_data(random_data, "DualPivotQS - Random", "dualPivotQS_random.png")
analyze_data(asc_data, "DualPivotQS - Ascending", "dualPivotQS_asc.png")
analyze_data(desc_data, "DualPivotQS - Descending", "dualPivotQS_desc.png")