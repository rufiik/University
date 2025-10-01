import numpy as np
import struct
import matplotlib.pyplot as plt
import os
from sklearn.decomposition import PCA
from collections import deque

def load_idx(filename):
    with open(filename, 'rb') as f:
        magic, size = struct.unpack(">II", f.read(8))
        if magic == 2051:
            rows, cols = struct.unpack(">II", f.read(8))
            data = np.fromfile(f, dtype=np.uint8).reshape(size, rows, cols)
        elif magic == 2049:
            data = np.fromfile(f, dtype=np.uint8)
        else:
            raise ValueError("Nieprawidłowy magic number w pliku IDX")
        return data
def euklides(a, b):
    return np.linalg.norm(a - b)

def region_query(X, point_idx, eps):
    # Zwraca indeksy punktów w sąsiedztwie eps od point_idx
    dists = np.linalg.norm(X - X[point_idx], axis=1)
    return np.where(dists <= eps)[0]

def expand_cluster(X, labels, point_idx, cluster_id, eps, min_samples):
    neighbors = region_query(X, point_idx, eps)
    if len(neighbors) < min_samples:
        labels[point_idx] = -1  # szum
        return False
    else:
        labels[point_idx] = cluster_id
        queue = deque(neighbors[neighbors != point_idx])
        while queue:
            n_idx = queue.popleft()
            if labels[n_idx] == -1:
                labels[n_idx] = cluster_id
            if labels[n_idx] != 0:
                continue
            labels[n_idx] = cluster_id
            n_neighbors = region_query(X, n_idx, eps)
            if len(n_neighbors) >= min_samples:
                queue.extend(n for n in n_neighbors if labels[n] == 0)
        return True

def dbscan(X, eps, min_samples):
    labels = np.zeros(X.shape[0], dtype=int)
    cluster_id = 0
    for i in range(X.shape[0]):
        if labels[i] != 0:
            continue
        if expand_cluster(X, labels, i, cluster_id + 1, eps, min_samples):
            cluster_id += 1
    labels[labels == 0] = -1  # nieprzypisane punkty to szum
    return labels
# Wczytaj dane
train_images_path = "emnist/emnist-mnist-train-images-idx3-ubyte"
train_labels_path = "emnist/emnist-mnist-train-labels-idx1-ubyte"
X = load_idx(train_images_path)
y = load_idx(train_labels_path)

# Popraw orientację obrazów
X = np.transpose(X, (0, 2, 1))
X = np.flip(X, axis=2)
X_flat = X.reshape(X.shape[0], -1)
# --- TUTAJ ZMIENIASZ PARAMETRY ---
N = 60000  # rozmiar próbki 
eps = 3.95
min_samples = 5
# ---------------------------------

X_sample = X_flat[:N] / 255.0  # normalizacja do zakresu [0, 1]
y_sample = y[:N] 

X_reduced = PCA(n_components=28).fit_transform(X_sample)
labels = dbscan(X_reduced, eps=eps, min_samples=min_samples)
n_clusters = len(set(labels)) - (1 if -1 in labels else 0)
n_noise = np.sum(labels == -1)
print(f"eps={eps}: liczba klastrów={n_clusters}, szum={n_noise/len(labels)*100:.2f}%")

# Analiza jakości klasteryzacji
def cluster_purity(y_true, labels):
    mask = labels != -1
    labels = labels[mask]
    y_true = y_true[mask]
    clusters = set(labels)
    correct = 0
    total = len(labels)
    for c in clusters:
        idx = (labels == c)
        if np.sum(idx) == 0:
            continue
        most_common = np.bincount(y_true[idx]).argmax()
        correct += np.sum(y_true[idx] == most_common)
    return correct / total if total > 0 else 0

purity = cluster_purity(y_sample, labels)
print(f"Czystość klastrów (purity): {purity*100:.2f}%")

# Procent błędnych klasyfikacji w klastrach (nie szum)
mask = labels != -1
labels_in_clusters = labels[mask]
y_in_clusters = y_sample[mask]
wrong = 0
for c in set(labels_in_clusters):
    idx = (labels_in_clusters == c)
    most_common = np.bincount(y_in_clusters[idx]).argmax()
    wrong += np.sum(y_in_clusters[idx] != most_common)
wrong_percent = wrong / len(labels_in_clusters) * 100 if len(labels_in_clusters) > 0 else 0
print(f"Procent błędnych klasyfikacji w klastrach: {wrong_percent:.2f}%")

# Procent szumu
noise_percent = n_noise / len(labels) * 100
print(f"Procent szumu: {noise_percent:.2f}%")

# Opcjonalnie: zapisz wykres liczności klastrów
os.makedirs("wyniki", exist_ok=True)
plt.figure(figsize=(10,4))
unique, counts = np.unique(labels, return_counts=True)
plt.bar([str(u) for u in unique], counts)
plt.title("Liczność klastrów (w tym szum: -1)")
plt.xlabel("Klaster")
plt.ylabel("Liczba próbek")
plt.savefig("wyniki/dbscan_hist.png")
plt.close()
print("\nLiczność każdego klastra:")
for klaster, liczba in zip(unique, counts):
    print(f"Klaster {klaster}: {liczba} próbek")