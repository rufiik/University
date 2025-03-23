import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm

# Funkcja do symulacji S_N
def simulate_sn(N, num_samples):
    X = np.random.choice([-1, 1], size=(num_samples, N))
    S_N = np.sum(X, axis=1)
    return S_N

# Funkcja do obliczania dystrybuanty empirycznej
def empirical_cdf(data, bins):
    counts, bin_edges = np.histogram(data, bins=bins, density=True)
    cdf = np.cumsum(counts * np.diff(bin_edges))
    return bin_edges, cdf

# Parametry symulacji
Ns = [5, 10, 15, 20, 25, 30, 100]
num_samples = 1000000  # liczba próbek dla każdej symulacji

# Symulacja i wykresy
for N in Ns:
    S_N = simulate_sn(N, num_samples)
    bins = np.arange(-N - 0.5, N + 1.5, 1)  # Przedziały histogramu

    # Histogram i dystrybuanta empiryczna
    plt.figure(figsize=(12, 6))

    # Histogram
    plt.subplot(1, 2, 1)
    plt.hist(S_N, bins=bins, density=True, alpha=0.7, color='blue', label=f'N = {N}')
    plt.title(f'Histogram S_N dla N = {N}')
    plt.xlabel('Wartość')
    plt.ylabel('Prawdopodobieństwo')
    plt.legend()

    # Dystrybuanta empiryczna
    bin_edges, cdf = empirical_cdf(S_N, bins)
    plt.subplot(1, 2, 2)
    plt.step(bin_edges[:-1], cdf, where='post', label='Empiryczna CDF', color='blue')

    # Dystrybuanta rozkładu normalnego
    x = np.linspace(-N, N, 1000)
    cdf_normal = norm.cdf(x, loc=0, scale=np.sqrt(N))
    plt.plot(x, cdf_normal, label='Normalna CDF', color='red', linestyle='--')

    plt.title(f'Dystrybuanta S_N dla N = {N}')
    plt.xlabel('Wartość')
    plt.ylabel('Dystrybuanta')
    plt.legend()

    plt.tight_layout()
    plt.show()
