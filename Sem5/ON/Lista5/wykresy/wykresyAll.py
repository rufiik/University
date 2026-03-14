import os
import re
import numpy as np
import matplotlib.pyplot as plt
from collections import defaultdict
from scipy.stats import linregress
opcje = [1, 2, 3, 4]
opcje_opisy = {
    1: "Eliminacja Gaussa",
    2: "Eliminacja Gaussa z częściowym wyborem",
    3: "Rozkład LU",
    4: "Rozkład LU z częściowym wyborem"
}
litery = ["B", "C", "D", "E", "F", "G", "H", "I", "J", "K"]
ns = [100, 1000, 2500, 5000, 7500, 10000, 25000, 50000, 100000, 250000, 500000, 750000, 1000000]

# Słowniki na wyniki
czasy = defaultdict(list)
pamieci_alg = defaultdict(list)
pamieci_macierz = defaultdict(list)
pamieci_sum = defaultdict(list)  # suma algorytm + macierz

def parse_memory(mem_str):
    match = re.match(r"([0-9.eE+-]+)\s*([kKmMgG][bB])", mem_str)
    if not match:
        return None
    value = float(match.group(1))
    unit = match.group(2).upper()
    if unit == "KB":
        return value / 1024
    elif unit == "MB":
        return value
    elif unit == "GB":
        return value * 1024
    else:
        return None

def parse_bytes(mem_str):
    match = re.match(r"([0-9.eE+-]+)\s*B", mem_str)
    if not match:
        return None
    value = float(match.group(1))
    return value / 1024 / 1024  # MB

# Zbieranie danych
for litera in litery:
    for suf in range(1, 11):
        for n in ns:
            macierz = f"{litera}{suf}_{n}.txt"
            # --- PAMIĘĆ MACIERZY (z wyniki_last2) ---
            memfile = os.path.join("macierze3/wyniki_last2", f"wynik_{litera}{suf}_{n}_mem.txt")
            mem_mac = None
            if os.path.exists(memfile):
                with open(memfile) as f:
                    line = f.readline().strip()
                    mem_mac = parse_bytes(line)
            pamieci_macierz[n].append(mem_mac)
            # --- CZASY I PAMIĘCI ALGORYTMÓW (z wyniki_last) ---
            # --- CZASY I PAMIĘCI ALGORYTMÓW (z wyniki_last) ---
            for opcja in opcje:
                wynikfile = os.path.join("macierze3/wyniki_last", f"wynik_{litera}{suf}_{n}.txt_opt{opcja}.txt")
                if not os.path.exists(wynikfile):
                    czasy[(opcja, n)].append(None)
                    pamieci_alg[(opcja, n)].append(None)
                    pamieci_sum[(opcja, n)].append(None)
                    continue
                with open(wynikfile) as f:
                    lines = f.readlines()
                    if len(lines) < 3:
                        czasy[(opcja, n)].append(None)
                        pamieci_alg[(opcja, n)].append(None)
                        pamieci_sum[(opcja, n)].append(None)
                        continue
                    czas_str = lines[1].strip()
                    pamiec_str = lines[-1].strip()
                    try:
                        czas = float(re.match(r"([0-9.eE+-]+)", czas_str).group(1))
                    except:
                        czas = None
                    pamiec = parse_memory(pamiec_str)
                    czasy[(opcja, n)].append(czas)
                    pamieci_alg[(opcja, n)].append(pamiec)
                    # Dodaj sumę pamięci algorytmu i macierzy wejściowej
                    if opcja == 3:
                        # Dla LU bez pivotowania licz tylko pamięć macierzy
                        if mem_mac is not None:
                            pamieci_sum[(opcja, n)].append(mem_mac)
                        else:
                            pamieci_sum[(opcja, n)].append(None)
                    else:
                        if pamiec is not None and mem_mac is not None:
                            pamieci_sum[(opcja, n)].append(pamiec + mem_mac)
                        else:
                            pamieci_sum[(opcja, n)].append(None)

# Uśrednianie
czasy_srednie = {opcja: [] for opcja in opcje}
pamieci_alg_srednie = {opcja: [] for opcja in opcje}
pamieci_macierz_srednie = []
pamieci_sum_srednie = {opcja: [] for opcja in opcje}

for n in ns:
    # Pamięć macierzy
    mems = [v for v in pamieci_macierz[n] if v is not None]
    if mems:
        pamieci_macierz_srednie.append((n, sum(mems)/len(mems)))
    # Algorytmy
    for opcja in opcje:
        cz = [v for v in czasy[(opcja, n)] if v is not None]
        pm = [v for v in pamieci_alg[(opcja, n)] if v is not None]
        pms = [v for v in pamieci_sum[(opcja, n)] if v is not None]
        if cz:
            czasy_srednie[opcja].append((n, sum(cz)/len(cz)))
        if pm:
            pamieci_alg_srednie[opcja].append((n, sum(pm)/len(pm)))
        if pms:
            pamieci_sum_srednie[opcja].append((n, sum(pms)/len(pms)))

def plot_with_regression(x, y, label, xlabel, ylabel, title, filename):
    plt.figure(figsize=(10,6))
    plt.plot(x, y, 'o', label=label)
    # Regresja liniowa w skali log-log
    logx = np.log10(x)
    logy = np.log10(y)
    slope, intercept, r_value, p_value, std_err = linregress(logx, logy)
    y_fit = 10**(intercept + slope*logx)
    plt.plot(x, y_fit, '-', label=f'y ~ n^{slope:.2f}')
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    plt.grid(True)
    plt.xscale('log')
    plt.yscale('log')
    plt.tight_layout()
    plt.savefig(filename)
    plt.close()

# Osobne wykresy dla każdej opcji: CZAS
for opcja in opcje:
    dane = sorted(czasy_srednie[opcja])
    if dane:
        x, y = zip(*dane)
        x = np.array(x)
        y = np.array(y)
        plot_with_regression(
            x, y,
            label=opcje_opisy[opcja],
            xlabel='Rozmiar macierzy n',
            ylabel='Średni czas [s]',
            title=f'Średni czas vs n ({opcje_opisy[opcja]})',
            filename=f"czas_{opcja}_reg.png"
        )

# Osobne wykresy dla każdej opcji: PAMIĘĆ
for opcja in opcje:
    dane = sorted(pamieci_sum_srednie[opcja])
    if dane:
        x, y = zip(*dane)
        x = np.array(x)
        y = np.array(y)
        plot_with_regression(
            x, y,
            label=opcje_opisy[opcja],
            xlabel='Rozmiar macierzy n',
            ylabel='Średnia pamięć (algorytm + macierz) [MB]',
            title=f'Średnia pamięć całkowita vs n ({opcje_opisy[opcja]})',
            filename=f"pamiec_{opcja}_reg.png"
        )

# Porównanie wszystkich opcji na jednym wykresie: CZAS
plt.figure(figsize=(10,6))
for opcja in opcje:
    dane = sorted(czasy_srednie[opcja])
    if dane:
        x, y = zip(*dane)
        x = np.array(x)
        y = np.array(y)
        plt.plot(x, y, 'o-', label=opcje_opisy[opcja])
        # Regresja
        logx = np.log10(x)
        logy = np.log10(y)
        slope, intercept, *_ = linregress(logx, logy)
        y_fit = 10**(intercept + slope*logx)
        plt.plot(x, y_fit, '--', label=f"{opcje_opisy[opcja]} regresja: n^{slope:.2f}")
plt.xlabel('Rozmiar macierzy n')
plt.ylabel('Średni czas [s]')
plt.title('Porównanie czasów i regresji')
plt.legend()
plt.grid(True)
plt.xscale('log')
plt.yscale('log')
plt.tight_layout()
plt.savefig("czas_all_reg.png")
plt.close()

# Porównanie wszystkich opcji na jednym wykresie: PAMIĘĆ
plt.figure(figsize=(10,6))
for opcja in opcje:
    dane = sorted(pamieci_sum_srednie[opcja])
    if dane:
        x, y = zip(*dane)
        x = np.array(x)
        y = np.array(y)
        plt.plot(x, y, 'o-', label=opcje_opisy[opcja])
        # Regresja
        logx = np.log10(x)
        logy = np.log10(y)
        slope, intercept, *_ = linregress(logx, logy)
        y_fit = 10**(intercept + slope*logx)
plt.xlabel('Rozmiar macierzy n')
plt.ylabel('Średnia pamięć  [MB]')
plt.title('Porównanie pamięci i regresji')
plt.legend()
plt.grid(True)
plt.xscale('log')
plt.yscale('log')
plt.tight_layout()
plt.savefig("pamiec_all.png")
plt.close()