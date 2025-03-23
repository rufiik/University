import numpy as np
import matplotlib.pyplot as plt

# Wczytaj dane z plików
def load_data(filename):
    data = np.loadtxt(filename, delimiter=';', skiprows=1)
    n_values = data[:, 0]
    max_values = data[:, 1]
    return n_values, max_values

n1, max1 = load_data('wynikimax1.txt')
n2, max2 = load_data('wynikimax2.txt')

# Oblicz średnie wartości maksymalnego zapełnienia dla każdego n
unique_n1 = np.unique(n1)
unique_n2 = np.unique(n2)

avg_max1 = [np.mean(max1[n1 == n]) for n in unique_n1]
avg_max2 = [np.mean(max2[n2 == n]) for n in unique_n2]

# Oblicz funkcje fn1 i fn2
fn1 = avg_max1 / (np.log(unique_n1) / np.log(np.log(unique_n1)))
fn2 = avg_max2 / (np.log(np.log(unique_n2)) / np.log(2))

# Wykresy
plt.figure(figsize=(12, 6))

plt.subplot(1, 2, 1)
plt.plot(unique_n1, fn1, label='fn1')
plt.xlabel('n')

plt.title(r'$\frac{l^{(1)}_n}{\frac{\ln n}{\ln \ln n}}$', fontsize=25)
plt.legend()

plt.subplot(1, 2, 2)
plt.plot(unique_n2, fn2, label='fn2')
plt.xlabel('n')

plt.title(r'$\frac{l^{(2)}_n}{\frac{\ln \ln n}{\ln 2}}$', fontsize=25)
plt.legend()

plt.tight_layout()
plt.show()