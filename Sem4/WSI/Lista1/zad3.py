import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import classification_report, accuracy_score, precision_score, recall_score
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt
import struct

def load_idx(filename):
    with open(filename, 'rb') as f:
        magic, size = struct.unpack(">II", f.read(8))
        if magic == 2051:  # Plik obrazów
            rows, cols = struct.unpack(">II", f.read(8))
            data = np.fromfile(f, dtype=np.uint8).reshape(size, rows * cols)  # Spłaszczenie obrazów
        elif magic == 2049:  # Plik etykiet
            data = np.fromfile(f, dtype=np.uint8)
        else:
            raise ValueError("Nieprawidłowy magic number w pliku IDX")
        return data


train_images_path = "emnist/emnist-mnist-train-images-idx3-ubyte"
train_labels_path = "emnist/emnist-mnist-train-labels-idx1-ubyte"
test_images_path = "emnist/emnist-mnist-test-images-idx3-ubyte"
test_labels_path = "emnist/emnist-mnist-test-labels-idx1-ubyte"

x_train = load_idx(train_images_path)
y_train = load_idx(train_labels_path)
x_test = load_idx(test_images_path)
y_test = load_idx(test_labels_path)

x_train = x_train / 255.0
x_test = x_test / 255.0

rf_model = RandomForestClassifier(n_estimators=100, random_state=42)

rf_model.fit(x_train, y_train)

y_pred = rf_model.predict(x_test)

accuracy = accuracy_score(y_test, y_pred)
precision = precision_score(y_test, y_pred, average=None)
recall = recall_score(y_test, y_pred, average=None)
classes = range(10)  # Klasy od 0 do 9

# Wyświetlenie wyników
print(f"Współczynnik prawidłowej rozpoznawalności (dokładność): {accuracy:.4f}")
print(f"Precyzja dla każdej klasy: {precision}")
print(f"Czułość dla każdej klasy: {recall}")
print("\nSzczegółowy raport klasyfikacji:")
print(classification_report(y_test, y_pred))        
# Tworzenie wykresów
fig, axes = plt.subplots(1, 2, figsize=(18, 6))  
# Wykres precyzji
axes[0].plot(classes, precision, marker='o', label='Precyzja', color='blue')
axes[0].set_title('Precyzja dla każdej klasy')
axes[0].set_xlabel('Klasa')
axes[0].set_ylabel('Precyzja')
axes[0].set_xticks(classes)
axes[0].grid()
axes[0].legend()
# Wykres czułości
axes[1].plot(classes, recall, marker='o', label='Czułość', color='green')
axes[1].set_title('Czułość dla każdej klasy')
axes[1].set_xlabel('Klasa')
axes[1].set_ylabel('Czułość')
axes[1].set_xticks(classes)
axes[1].grid()
axes[1].legend()
# Dopasowanie układu i zapisanie wykresów jako plik
plt.tight_layout()
plt.savefig('wykresy/random_forest_wykresy.png')  # Zapisuje wykresy jako plik PNG
plt.show()