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
ns = [10, 100, 1000, 2500, 5000, 7500, 10000, 25000, 50000, 100000, 250000, 500000, 750000, 1000000]
backslash_ns = [10, 100, 1000, 2500, 5000, 7500, 10000, 25000, 50000]

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

# Zbieranie danych Twoich algorytmów
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

# --- Wczytaj dane dla A\b ---
backslash_czasy = []
backslash_pamieci = []

for n in backslash_ns:
    czasy_n = []
    pamieci_n = []
    for suf in range(1, 11):
        plik = f"wyniki_backslash/B{suf}_{n}_backslash.txt"
        if not os.path.exists(plik):
            continue
        with open(plik) as f:
            lines = f.readlines()
            if len(lines) < 3:
                continue
            try:
                czas = float(lines[1].split()[0])
                pamiec = float(lines[2].split()[0]) / 1024 / 1024  # MB
                czasy_n.append(czas)
                pamieci_n.append(pamiec)
            except Exception:
                continue
    # Średnia dla danego n
    if czasy_n:
        backslash_czasy.append(sum(czasy_n)/len(czasy_n))
        backslash_pamieci.append(sum(pamieci_n)/len(pamieci_n))
    else:
        backslash_czasy.append(None)
        backslash_pamieci.append(None)

# --- Wykresy porównawcze ---

# CZAS
plt.figure(figsize=(10,6))
for opcja in opcje:
    dane = [item for item in czasy_srednie[opcja] if item[0] in backslash_ns]
    if dane:
        x, y = zip(*dane)
        plt.plot(x, y, 'o-', label=opcje_opisy[opcja])
plt.plot(backslash_ns, backslash_czasy, 's--', color='black', label='A\\b (średni)')
plt.xlabel('Rozmiar macierzy n')
plt.ylabel('Średni czas [s]')
plt.title('Porównanie czasów: algorytmy vs A\\b')
plt.legend()
plt.grid(True)
plt.xscale('log')
plt.yscale('log')
plt.tight_layout()
plt.savefig("czas_vs_backslash.png")
plt.close()

# PAMIĘĆ
plt.figure(figsize=(10,6))
for opcja in opcje:
    dane = [item for item in pamieci_sum_srednie[opcja] if item[0] in backslash_ns]
    if dane:
        x, y = zip(*dane)
        plt.plot(x, y, 'o-', label=opcje_opisy[opcja])
plt.plot(backslash_ns, backslash_pamieci, 's--', color='black', label='A\\b (średnia)')
plt.xlabel('Rozmiar macierzy n')
plt.ylabel('Średnia pamięć [MB]')
plt.title('Porównanie pamięci: algorytmy vs A\\b')
plt.legend()
plt.grid(True)
plt.xscale('log')
plt.yscale('log')
plt.tight_layout()
plt.savefig("pamiec_vs_backslash.png")
plt.close()