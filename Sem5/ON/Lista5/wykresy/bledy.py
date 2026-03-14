import os
import re
import csv
from collections import defaultdict

opcje = [1, 2, 3, 4]
opcje_opisy = {
    1: "Gauss",
    2: "Gauss+partial",
    3: "LU",
    4: "LU+partial"
}
litery = ["B", "C", "D", "E", "F", "G", "H", "I", "J", "K"]
ns = [10, 100, 1000, 2500, 5000, 7500, 10000, 25000, 50000, 100000, 250000, 500000, 750000, 1000000]

bledy = defaultdict(list)
bledy_srednie = {opcja: [] for opcja in opcje}

for litera in litery:
    for suf in range(1, 11):
        for n in ns:
            for opcja in opcje:
                wynikfile = os.path.join("macierze3/wyniki_last", f"wynik_{litera}{suf}_{n}.txt_opt{opcja}.txt")
                if not os.path.exists(wynikfile):
                    bledy[(opcja, n)].append(None)
                    continue
                with open(wynikfile) as f:
                    line = f.readline().strip()
                    try:
                        blad = float(line)
                    except:
                        blad = None
                    bledy[(opcja, n)].append(blad)

# Uśrednianie norm błędu
for n in ns:
    for opcja in opcje:
        bl = [v for v in bledy[(opcja, n)] if v is not None]
        if bl:
            bledy_srednie[opcja].append((n, sum(bl)/len(bl)))
        else:
            bledy_srednie[opcja].append((n, None))

# Zapis do CSV
with open("bledy_srednie.csv", "w", newline='') as csvfile:
    writer = csv.writer(csvfile)
    header = ["n"] + [opcje_opisy[o] for o in opcje]
    writer.writerow(header)
    for idx, n in enumerate(ns):
        row = [n]
        for opcja in opcje:
            val = bledy_srednie[opcja][idx][1]
            row.append("" if val is None else f"{val:.16e}")
        writer.writerow(row)

print("Zapisano do bledy_srednie.csv")