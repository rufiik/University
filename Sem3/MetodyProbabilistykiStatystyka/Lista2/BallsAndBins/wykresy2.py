import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.ticker import ScalarFormatter

# Wczytaj dane z pliku
data = pd.read_csv('wyniki.txt', delimiter=';')

# Oblicz średnie wartości dla każdej kolumny, grupując dane według 'n'
mean_data = data.groupby('n').mean().reset_index()

# Przypisz kolumny do zmiennych
n = mean_data['n']
Bn = mean_data['Bn']
Un = mean_data['Un']
Cn = mean_data['Cn']
Dn = mean_data['Dn']
Dn_Cn = mean_data['Dn-Cn']

# Oblicz wymagane ilorazy
Bn_n = Bn / n
Bn_sqrt_n = Bn / np.sqrt(n)
Un_n = Un / n
Cn_n = Cn / n
Cn_n_ln_n = Cn / (n * np.log(n))
Cn_n2 = Cn / (n ** 2)
Dn_n = Dn / n
Dn_n_ln_n = Dn / (n * np.log(n))
Dn_n2 = Dn / (n ** 2)
Dn_Cn_n = Dn_Cn / n
Dn_Cn_n_ln_n = Dn_Cn / (n * np.log(n))
Dn_Cn_n_ln_ln_n = Dn_Cn / (n * np.log(np.log(n)))

# Ustawienia formatu osi y
formatter = ScalarFormatter(useOffset=False, useMathText=False)
formatter.set_scientific(False)

# Wykresy
plt.figure(figsize=(15, 10))

# Wykres dla Bn/n oraz Bn/sqrt(n)
plt.subplot(2, 3, 1)
plt.plot(n, Bn_n, label=r'$B_n/n$')
plt.plot(n, Bn_sqrt_n, label=r'$B_n/\sqrt{n}$')
plt.xlabel('n')
plt.ylabel('Iloraz')
plt.title(r'Iloraz $B_n/n$ oraz $B_n/\sqrt{n}$')
plt.legend()
plt.gca().yaxis.set_major_formatter(formatter)

# Wykres dla Un/n
plt.subplot(2, 3, 2)
plt.plot(n, Un_n, label=r'$U_n/n$')
plt.xlabel('n')
plt.ylabel('Iloraz')
plt.title(r'Iloraz $U_n/n$')
plt.legend()
plt.gca().yaxis.set_major_formatter(formatter)

plt.subplot(2, 3, 3)
plt.plot(n, Cn_n, label=r'$C_n/n$')
plt.plot(n, Cn_n_ln_n, label=r'$C_n/(n\ln(n))$')
plt.plot(n, Cn_n2, label=r'$C_n/(n^2)$')
plt.xlabel('n')
plt.ylabel('Iloraz')
plt.title(r'Iloraz $C_n/n$, $C_n/(n\ln(n))$ oraz $C_n/(n^2)$')
plt.legend()
plt.gca().yaxis.set_major_formatter(formatter)

# Wykres dla Dn/n, Dn/(n*ln(n)) oraz Dn/(n^2)
plt.subplot(2, 3, 4)
plt.plot(n, Dn_n, label=r'$D_n/n$')
plt.plot(n, Dn_n_ln_n, label=r'$D_n/(n\ln(n))$')
plt.plot(n, Dn_n2, label=r'$D_n/(n^2)$')
plt.xlabel('n')
plt.ylabel('Iloraz')
plt.title(r'Iloraz $D_n/n$, $D_n/(n\ln(n))$ oraz $D_n/(n^2)$')
plt.legend()
plt.gca().yaxis.set_major_formatter(formatter)

# Wykres dla (Dn-Cn)/n, (Dn-Cn)/(n*ln(n)) oraz (Dn-Cn)/(n*ln(ln(n)))
plt.subplot(2, 3, 5)
plt.plot(n, Dn_Cn_n, label=r'$(D_n-C_n)/n$')
plt.plot(n, Dn_Cn_n_ln_n, label=r'$(D_n-C_n)/(n\ln(n))$')
plt.plot(n, Dn_Cn_n_ln_ln_n, label=r'$(D_n-C_n)/(n\ln(\ln(n)))$')
plt.xlabel('n')
plt.ylabel('Iloraz')
plt.title(r'Iloraz $(D_n-C_n)/n$, $(D_n-C_n)/(n\ln(n))$ oraz $(D_n-C_n)/(n\ln(\ln(n)))$')
plt.legend()
plt.gca().yaxis.set_major_formatter(formatter)

# Wyświetl wykresy
plt.tight_layout()
plt.show()