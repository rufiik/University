import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.ticker import ScalarFormatter
# Wczytaj dane z pliku
data = pd.read_csv('wyniki.txt', delimiter=';')
# Oblicz średnie wartości dla każdej kolumny, grupując dane według 'n'
mean_data = data.groupby('n').mean().reset_index()
# Przypisz kolumny do zmiennych
n = data['n']
Bn = data['Bn']
Un = data['Un']
Cn = data['Cn']
Dn = data['Dn']
Dn_Cn = data['Dn-Cn']

# Przypisz średnie wartości do zmiennych
mean_n = mean_data['n']
mean_Bn = mean_data['Bn']
mean_Un = mean_data['Un']
mean_Cn = mean_data['Cn']
mean_Dn = mean_data['Dn']
mean_Dn_Cn = mean_data['Dn-Cn']

# Stwórz wykresy
formatter = plt.FuncFormatter(lambda x, _: f'{x:.0f}')

# Tworzenie wykresów obok siebie
fig, axs = plt.subplots(3, 2, figsize=(15, 15))

# Wykres dla Bn
axs[0, 0].scatter(n, Bn, label='Bn', s=10)
axs[0, 0].scatter(mean_n, mean_Bn, label='Średnie Bn', color='red', s=20)
axs[0, 0].set_xlabel('n')
axs[0, 0].set_ylabel(r'$B_n$')
axs[0, 0].set_title('Wykres Bn')
axs[0, 0].legend()
axs[0, 0].yaxis.set_major_formatter(formatter)

# Wykres dla Un
axs[0, 1].scatter(n, Un, label='Un', s=10)
axs[0, 1].scatter(mean_n, mean_Un, label='Średnie Un', color='red', s=20)
axs[0, 1].set_xlabel('n')
axs[0, 1].set_ylabel(r'$U_n$')
axs[0, 1].set_title('Wykres Un')
axs[0, 1].legend()
axs[0, 1].yaxis.set_major_formatter(formatter)

# Wykres dla Cn
axs[1, 0].scatter(n, Cn, label='Cn', s=10)
axs[1, 0].scatter(mean_n, mean_Cn, label='Średnie Cn', color='red', s=50)
axs[1, 0].set_xlabel('n')
axs[1, 0].set_ylabel(r'$C_n$')
axs[1, 0].set_title('Wykres Cn')
axs[1, 0].legend()
axs[1, 0].yaxis.set_major_formatter(formatter)

# Wykres dla Dn
axs[1, 1].scatter(n, Dn, label='Dn', s=10)
axs[1, 1].scatter(mean_n, mean_Dn, label='Średnie Dn', color='red', s=50)
axs[1, 1].set_xlabel('n')
axs[1, 1].set_ylabel(r'$D_n$')
axs[1, 1].set_title('Wykres Dn')
axs[1, 1].legend()
axs[1, 1].yaxis.set_major_formatter(formatter)

# Wykres dla Dn-Cn
axs[2, 0].scatter(n, Dn_Cn, label='Dn-Cn', s=10)
axs[2, 0].scatter(mean_n, mean_Dn_Cn, label='Średnie Dn-Cn', color='red', s=50)
axs[2, 0].set_xlabel('n')
axs[2, 0].set_ylabel(r'$D_n - C_n$')
axs[2, 0].set_title('Wykres Dn-Cn')
axs[2, 0].legend()
axs[2, 0].yaxis.set_major_formatter(formatter)

# Usunięcie pustego wykresu
fig.delaxes(axs[2, 1])

# Dostosowanie układu
plt.tight_layout()
plt.show()