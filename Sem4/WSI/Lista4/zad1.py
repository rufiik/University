import numpy as np
import struct
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
import os

def load_idx(filename):
    with open(filename, 'rb') as f:
        magic, size = struct.unpack(">II", f.read(8))
        if magic == 2051:  # Plik obrazów
            rows, cols = struct.unpack(">II", f.read(8))
            data = np.fromfile(f, dtype=np.uint8).reshape(size, rows, cols)
        elif magic == 2049:  # Plik etykiet
            data = np.fromfile(f, dtype=np.uint8)
        else:
            raise ValueError("Nieprawidłowy magic number w pliku IDX")
        return data

# Ścieżki do plików
train_images_path = "emnist/emnist-mnist-train-images-idx3-ubyte"
train_labels_path = "emnist/emnist-mnist-train-labels-idx1-ubyte"

# Wczytaj dane
X = load_idx(train_images_path)
y = load_idx(train_labels_path)

X = np.transpose(X, (0, 2, 1))  # obrót o 90 stopni
# X = np.flip(X, axis=2)          # odbicie lustrzane w pionie
# Spłaszcz obrazy do wektorów
X_flat = X.reshape(X.shape[0], -1)

# Klasteryzacja k-średnich (10 klastrów, k-means++)
def najlepszy_kmeans(X, n_clusters, n_init=10):
    najlepszy_model = None
    najmniejsza_inercja = np.inf
    for _ in range(n_init):
        model = KMeans(n_clusters=n_clusters, init='k-means++', n_init=1)
        model.fit(X)
        if model.inertia_ < najmniejsza_inercja:
            najmniejsza_inercja = model.inertia_
            najlepszy_model = model
    return najlepszy_model

model = najlepszy_kmeans(X_flat, 10)

# Macierz procentowego przydziału cyfr do klastrów
def macierz_przydzialu(y, labels, n_clusters):
    macierz = np.zeros((10, n_clusters))
    for cyfra in range(10):
        idx = (y == cyfra)
        for klaster in range(n_clusters):
            macierz[cyfra, klaster] = np.sum(labels[idx] == klaster)
    macierz = macierz / macierz.sum(axis=1, keepdims=True) * 100
    return macierz

os.makedirs("wyniki", exist_ok=True)

for n_clusters in [10, 15, 20, 30]:
    print(f"\nLiczba klastrów: {n_clusters}")
    model = najlepszy_kmeans(X_flat, n_clusters)
    macierz = macierz_przydzialu(y, model.labels_, n_clusters)

    # Wykres macierzy przydziału
    plt.figure(figsize=(10, 8))
    plt.imshow(macierz, cmap='Blues', aspect='auto')
    plt.colorbar(label='Procent')
    plt.xlabel('Klaster')
    plt.ylabel('Cyfra')
    plt.title(f'Procentowy przydział cyfr do klastrów ({n_clusters} klastrów)')
    plt.xticks(range(n_clusters))
    plt.yticks(range(10))
    plt.savefig(f"wyniki/macierz_{n_clusters}.png")
    plt.close()

    # Wykres centroidów
    n_cols = min(n_clusters, 10)
    n_rows = (n_clusters + n_cols - 1) // n_cols
    plt.figure(figsize=(2*n_cols, 2*n_rows))
    for i, centroid in enumerate(model.cluster_centers_):
        plt.subplot(n_rows, n_cols, i+1)
        plt.imshow(centroid.reshape(28, 28), cmap='gray')
        plt.axis('off')
        plt.title(f'Klaster {i}')
    plt.suptitle(f'Centroidy klastrów ({n_clusters})')
    plt.savefig(f"wyniki/centroidy_{n_clusters}.png")
    plt.close()

