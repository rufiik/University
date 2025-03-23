import pandas as pd
import matplotlib.pyplot as plt

def read_data(file_path):
    data = pd.read_csv(file_path, sep=';')
    return data

def plot_boxplot(data, title, file_name):
    plt.figure(figsize=(12, 6))
    
    plt.boxplot(data['max'], vert=False, patch_artist=True)
    
    plt.title(title)
    plt.xlabel('Maksymalna liczba kul w pojemniku')
    plt.grid(True, which="both", ls="--")
    plt.savefig(file_name)
    plt.show()

def plot_histogram(data, title, file_name):
    plt.figure(figsize=(12, 6))
    
    # Filtruj dane, aby usunąć wartości 0
    filtered_data = data[data['max'] > 0]
    
    counts, bins, patches = plt.hist(filtered_data['max'], bins=20, color='blue', edgecolor='black')  # Zwiększ liczbę binów
    
    plt.title(title)
    plt.xlabel('Maksymalna liczba kul w pojemniku')
    plt.ylabel('Częstotliwość')
    plt.grid(True, which="both", ls="--")
    
    # Dodanie wartości pod słupkami
    bin_centers = 0.5 * (bins[:-1] + bins[1:])
    for count, x in zip(counts, bin_centers):
        if count > 0:  # Wyświetlaj tylko wartości większe od 0
            plt.text(x, count, int(count), ha='center', va='bottom')
    
    plt.savefig(file_name)
    plt.show()

if __name__ == "__main__":
    data1 = read_data('wynikimax1.txt')
    data2 = read_data('wynikimax2.txt')
    
    plot_histogram(data1, 'Histogram - Maksymalna liczba kul w pojemnikach - Wyniki dla d=1', 'histogram_wyniki1.png')
    plot_histogram(data2, 'Histogram - Maksymalna liczba kul w pojemnikach - Wyniki dla d=2', 'histogram_wyniki2.png')