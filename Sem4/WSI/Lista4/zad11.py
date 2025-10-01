import numpy as np
import struct
import matplotlib.pyplot as plt
import os
class KMeansManual:
    def __init__(self, n_clusters=10, max_iter=10, tol=1e-4, random_state=None):
        self.n_clusters = n_clusters
        self.max_iter = max_iter
        self.tol = tol
        self.random_state = random_state
    def fit(self, X):
        np.random.seed(self.random_state)
        n_samples, n_features = X.shape

        # Losowa inicjalizacja centroidów
        indices = np.random.choice(n_samples, self.n_clusters, replace=False)
        self.cluster_centers_ = X[indices]

        for _ in range(self.max_iter):
            # Szybkie liczenie kwadratów odległości
            X_sq = np.sum(X**2, axis=1, keepdims=True)           # shape: (n_samples, 1)
            C_sq = np.sum(self.cluster_centers_**2, axis=1)      # shape: (n_clusters,)
            XC = X @ self.cluster_centers_.T                     # shape: (n_samples, n_clusters)
            distances = X_sq + C_sq - 2 * XC                     # shape: (n_samples, n_clusters)
            labels = np.argmin(distances, axis=1)

            # Oblicz nowe centroidy
            new_centers = np.array([
                X[labels == i].mean(axis=0) if np.any(labels == i) else self.cluster_centers_[i]
                for i in range(self.n_clusters)
            ])

            # Sprawdź zbieżność
            if np.linalg.norm(self.cluster_centers_ - new_centers) < self.tol:
                break
            self.cluster_centers_ = new_centers

        self.labels_ = labels
        # Inercja (suma kwadratów odległości do najbliższego centroidu)
        self.inertia_ = np.sum((X - self.cluster_centers_[labels])**2)
        return self


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
X_flat = X.reshape(X.shape[0], -1)

def najlepszy_kmeans_manual(X, n_clusters, n_init=10):
    najlepszy_model = None
    najmniejsza_inercja = np.inf
    for i in range(n_init):
        model = KMeansManual(n_clusters=n_clusters, random_state=i)
        model.fit(X)
        if model.inertia_ < najmniejsza_inercja:
            najmniejsza_inercja = model.inertia_
            najlepszy_model = model
    return najlepszy_model

def macierz_przydzialu(y, labels, n_clusters):
    macierz = np.zeros((10, n_clusters))
    for cyfra in range(10):
        idx = (y == cyfra)
        for klaster in range(n_clusters):
            macierz[cyfra, klaster] = np.sum(labels[idx] == klaster)
    macierz = macierz / macierz.sum(axis=1, keepdims=True) * 100
    return macierz

os.makedirs("wyniki_manual", exist_ok=True)

for n_clusters in [10, 15, 20, 30]:
    print(f"\nLiczba klastrów: {n_clusters}")
    model = najlepszy_kmeans_manual(X_flat, n_clusters)
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
    plt.savefig(f"wyniki_manual/macierz_{n_clusters}.png")
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
    plt.savefig(f"wyniki_manual/centroidy_{n_clusters}.png")
    plt.close()