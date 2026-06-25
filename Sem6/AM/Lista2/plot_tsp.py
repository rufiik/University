import matplotlib.pyplot as plt
import os
import glob

def plot_tsp_special_format(file_path):
    indices = []
    coords_x = []
    coords_y = []
    
    # Słownik nazw krajów (dodane Oman, Canada, Tanzania, Egypt, Ireland)
    countries = {
        "zi": "Zimbabwe",
        "wi": "Western Sahara",
        "uy": "Uruguay",
        "qa": "Qatar",
        "dj": "Djibouti",
        "om": "Oman",
        "ca": "Canada",
        "tz": "Tanzania",
        "eg": "Egypt",
        "ei": "Ireland"
    }
    
    # Rozpoznawanie kraju po nazwie pliku
    country_name = "Unknown"
    base_name = os.path.basename(file_path).lower()
    for code, name in countries.items():
        if code in base_name:
            country_name = name
            break
            
    # Rozpoznawanie metody (MST, Local Search, Local Search 2)
    if "mst_ls" in base_name:
        method = "MST + Local Search"
    elif "mst" in base_name:
        method = "MST"
    elif "local2" in base_name:
        method = "Local Search 2 (losowe sąsiedztwo)"
    elif "local" in base_name:
        method = "Local Search"
    else:
        method = "Random/Other"
    
    try:
        with open(file_path, 'r') as f:
            lines = [line.strip() for line in f if line.strip() and not line.startswith('#')]
            if not lines: return

            indices = lines[0].split()
            for line in lines[1:]:
                parts = line.split()
                if len(parts) >= 2:
                    coords_x.append(float(parts[0]))
                    coords_y.append(float(parts[1]))
    except Exception as e:
        print(f"Błąd odczytu {file_path}: {e}")
        return

    n = len(coords_x)
    
    if n > 500:
        dot_size, line_width, alpha_val, show_labels = 2, 0.5, 0.4, False
    elif n > 100:
        dot_size, line_width, alpha_val, show_labels = 10, 0.8, 0.6, False
    else:
        dot_size, line_width, alpha_val, show_labels = 30, 1.0, 0.7, True

    x_plot = coords_x + [coords_x[0]]
    y_plot = coords_y + [coords_y[0]]

    plt.figure(figsize=(15, 10))
    plt.plot(x_plot, y_plot, color='royalblue', lw=line_width, alpha=alpha_val, zorder=1)
    
    if n < 100:
        for i in range(len(x_plot) - 1):
            plt.annotate('', xy=(x_plot[i+1], y_plot[i+1]), xytext=(x_plot[i], y_plot[i]),
                         arrowprops=dict(arrowstyle='->', color='blue', lw=0.5, alpha=0.3))

    plt.scatter(coords_x, coords_y, c='crimson', s=dot_size, zorder=5, alpha=0.8)
    
    if show_labels:
        for i, label in enumerate(indices):
            plt.text(coords_x[i], coords_y[i], f' {label}', fontsize=8, alpha=0.7)

    plt.scatter(coords_x[0], coords_y[0], c='green', s=dot_size*4, marker='s', 
                label=f'Start (ID: {indices[0]})', zorder=10)

    plt.title(f"{country_name} - {method} (Liczba miast: {n})", fontsize=18, fontweight='bold')
    
    plt.grid(True, linestyle=':', alpha=0.5)
    plt.legend(loc='upper right')
    plt.axis('equal') 
    
    output_name = os.path.splitext(file_path)[0] + ".png"
    plt.savefig(output_name, dpi=300, bbox_inches='tight')
    print(f"Zapisano: {output_name} dla {country_name} ({method})")
    plt.close()  

# Obsługa plików 
search_patterns = [
    "best_local_*.tsp",
    "best_local2_*.tsp",
    "best_MST_LS_*.tsp",
    "best_MST_*.tsp",
    "best_*.tsp",
    "*mst*.tsp"
]
files_to_process = []
for pattern in search_patterns:
    files_to_process.extend(glob.glob(pattern))

# Usunięcie duplikatów jeśli plik pasuje do obu wzorców
for file in sorted(list(set(files_to_process))):
    print(f"Przetwarzanie: {file}")
    plot_tsp_special_format(file)