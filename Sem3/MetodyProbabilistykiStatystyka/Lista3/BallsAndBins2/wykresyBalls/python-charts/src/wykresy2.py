import pandas as pd
import matplotlib.pyplot as plt

def read_data(file_path):
    data = pd.read_csv(file_path, sep=';')
    return data

def plot_scatter(data, title, file_name):
    plt.figure(figsize=(12, 6))
    
    plt.scatter(data['n'], data['max'], color='blue' , zorder=5)
    
    plt.title(title)
    plt.xlabel('n')
    plt.ylabel('Maksymalna liczba kul w pojemniku')
    plt.grid(True, which="both", ls="-", zorder=0)  # Ustawienie zorder dla siatki
    
    # Obliczanie średniej wartości dla każdego 'n'
    mean_values = data.groupby('n')['max'].mean()
    for n, mean_value in mean_values.items():
        plt.scatter(n, mean_value, color='red', zorder=5)  # Dodanie czerwonej kropki dla średniej wartości z wyższym zorder
    
    plt.savefig(file_name)
    plt.show()

if __name__ == "__main__":
    data1 = read_data('wynikimax1.txt')
    data2 = read_data('wynikimax2.txt')
    
    plot_scatter(data1, 'Maksymalna liczba kul w pojemnikach - Wyniki dla d=1', 'scatter_wyniki1.png')
    plot_scatter(data2, 'Maksymalna liczba kul w pojemnikach - Wyniki dla d=2', 'scatter_wyniki2.png')