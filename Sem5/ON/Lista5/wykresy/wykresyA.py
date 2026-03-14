import os
import re
import numpy as np
import matplotlib.pyplot as plt
from collections import defaultdict
from matplotlib.ticker import ScalarFormatter
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import PolynomialFeatures
# Parametry
folder2 = "wyniki3"
litery = ["A"]
ns_A = [10, 100, 1000, 2500, 5000, 10000, 25000, 50000, 100000, 250000, 500000, 750000, 1000000]
ns_rest = [10, 100, 1000, 2500, 5000, 10000, 25000, 50000, 100000]
opcje = [1, 2, 3, 4]
opcje_opisy = {
    1: "Eliminacja Gaussa",
    2: "Eliminacja Gaussa z częściowym wyborem",
    3: "Rozkład LU",
    4: "Rozkład LU z częściowym wyborem"
}

czasy = {opcja: defaultdict(list) for opcja in opcje}
pamieci = {opcja: defaultdict(list) for opcja in opcje}

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

for opcja in opcje:
    # Macierze A z folderu wyniki (duże rozmiary)
    for n in ns_A:
        fname = f"wynik_A_{n}.txt_opt{opcja}.txt"
        fpath = os.path.join(folder2, fname)
        if not os.path.exists(fpath):
            continue
        with open(fpath) as f:
            lines = f.readlines()
            if len(lines) < 3:
                continue
            czas_str = lines[1].strip()
            match_czas = re.match(r"([0-9.eE+-]+)", czas_str)
            pamiec_str = lines[-1].strip()
            pamiec = parse_memory(pamiec_str)
            if match_czas and pamiec is not None:
                czas = float(match_czas.group(1))
                czasy[opcja][n].append(czas)
                pamieci[opcja][n].append(pamiec)
czasy_srednie = {opcja: [] for opcja in opcje}
pamieci_srednie = {opcja: [] for opcja in opcje}

all_ns = sorted(set(ns_A + ns_rest))

for opcja in opcje:
    for n in all_ns:
        if czasy[opcja][n]:
            avg_czas = sum(czasy[opcja][n]) / len(czasy[opcja][n])
            czasy_srednie[opcja].append((n, avg_czas))
        if pamieci[opcja][n]:
            avg_pamiec = sum(pamieci[opcja][n]) / len(pamieci[opcja][n])
            pamieci_srednie[opcja].append((n, avg_pamiec))

# Wybrane ticki na osi X
xticks_custom = [1000, 10000, 100000, 250000, 500000, 750000, 1000000]

# Wykresy czasu dla każdej opcji osobno
for opcja in opcje:
    plt.figure(figsize=(10,6))
    dane = sorted(czasy_srednie[opcja])
    if not dane:
        continue
    x, y = zip(*dane)
    x_np = np.array(x).reshape(-1, 1)
    y_np = np.array(y)
    plt.plot(x, y, marker='o', label=opcje_opisy[opcja])
    # Dopasowanie prostej
    model = LinearRegression()
    model.fit(x_np, y_np)
    y_pred = model.predict(x_np)
    plt.plot(x, y_pred, linestyle='--', label=f"Regresja liniowa (R²={model.score(x_np, y_np):.2f})")
    plt.xlabel('Rozmiar macierzy n')
    plt.ylabel('Średni czas [s]')
    plt.title(f'Średni czas vs n ({opcje_opisy[opcja]})')
    plt.legend()
    plt.grid(True)
    plt.xticks(xticks_custom, rotation=45)
    plt.tight_layout()
    plt.savefig(f"czas_{opcja}.png")
    plt.close()

# Wykresy pamięci dla każdej opcji osobno
for opcja in opcje:
    plt.figure(figsize=(10,6))
    dane = sorted(pamieci_srednie[opcja])
    if not dane:
        continue
    x, y = zip(*dane)
    x_np = np.array(x).reshape(-1, 1)
    y_np = np.array(y)
    plt.plot(x, y, marker='o', label=opcje_opisy[opcja])
    model = LinearRegression()
    model.fit(x_np, y_np)
    y_pred = model.predict(x_np)
    plt.plot(x, y_pred, linestyle='--', label=f"Regresja liniowa (R²={model.score(x_np, y_np):.2f})")
    plt.xlabel('Rozmiar macierzy n')
    plt.ylabel('Średnia pamięć [MB]')
    plt.title(f'Średnia pamięć vs n ({opcje_opisy[opcja]})')
    plt.legend()
    plt.grid(True)
    plt.xticks(xticks_custom, rotation=45)
    plt.tight_layout()
    plt.savefig(f"pamiec_{opcja}.png")
    plt.close()