import numpy as np
from scipy.stats import binom

n_values = [100, 1000, 10000]
p = 0.5

for n in n_values:
    wartosc_oczekiwana = n * p
    wariancja = n * p * (1 - p)
    binom_dist = binom(n, p)

    # (a) P(X >= 1.2 * E(X))
    a = 1.2 * wartosc_oczekiwana
    k_a = int(np.ceil(a))
    # Markov
    markov_a = wartosc_oczekiwana / a

    # Chebyshev
    chebyshev_a = wariancja / ((a - wartosc_oczekiwana) ** 2)

    # Exact Probability
    exact_a =binom_dist.sf(k_a - 1)

    # (b) P(|X - E(X)| >= 0.1 * E(X))
    b = 0.1 * wartosc_oczekiwana
    k_b = int(np.ceil(b))

    # Markov
    markov_b = wartosc_oczekiwana / b

    # Chebyshev
    chebyshev_b = wariancja / (b ** 2)

    # Exact Probability
    exact_b = 1.0 - binom_dist.cdf(int(wartosc_oczekiwana + k_b - 1)) + binom_dist.cdf(int(wartosc_oczekiwana - k_b))

    # Print Results
    print(f"n = {n}")
    print(f"(a) Wartosc oczekiwana {wartosc_oczekiwana:.1f} Markov: {markov_a:.20g}, Chebyshev: {chebyshev_a:.20g}, Exact: {exact_a:.20g}")
    print(f"(b) Wartosc oczekiwana {wartosc_oczekiwana:.1f} Markov: {markov_b:.20g}, Chebyshev: {chebyshev_b:.20g}, Exact: {exact_b:.20g}")
    print("--------------------------")