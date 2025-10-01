import tensorflow as tf
from tensorflow import keras
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import classification_report, confusion_matrix
from sklearn.metrics import ConfusionMatrixDisplay
import struct
from sklearn.metrics import precision_score, recall_score

# Funkcja do załadowania plików IDX
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

train_images_path = "emnist/emnist-mnist-train-images-idx3-ubyte"
train_labels_path = "emnist/emnist-mnist-train-labels-idx1-ubyte"
test_images_path = "emnist/emnist-mnist-test-images-idx3-ubyte"
test_labels_path = "emnist/emnist-mnist-test-labels-idx1-ubyte"

x_train = load_idx(train_images_path)
y_train = load_idx(train_labels_path)
x_test = load_idx(test_images_path)
y_test = load_idx(test_labels_path)

x_train, x_test = x_train / 255.0, x_test / 255.0  # Normalizacja do zakresu [0,1]

y_train = keras.utils.to_categorical(y_train, 10)
y_test_cat = keras.utils.to_categorical(y_test, 10)

# (MLP - Multi-Layer Perceptron)
model = keras.Sequential([
    keras.layers.Flatten(input_shape=(28, 28)),  # Spłaszczenie obrazu 28x28 do wektora
    keras.layers.Dense(128, activation='relu'),  # Warstwa ukryta z 128 neuronami
    keras.layers.Dropout(0.5),  # Regularizacja, aby zapobiec przeuczeniu
    keras.layers.Dense(10, activation='softmax')  # Warstwa wyjściowa dla 10 klas
])
# Kompilacja
model.compile(optimizer='adam',
              loss='categorical_crossentropy',
              metrics=['accuracy'])
# Trenowanie
history = model.fit(x_train, y_train, epochs=10, batch_size=32, validation_split=0.1)
test_loss, test_acc = model.evaluate(x_test, y_test_cat)
print(f'Dokładność na zbiorze testowym: {test_acc:.4f}')
y_pred = model.predict(x_test)
y_pred_classes = np.argmax(y_pred, axis=1)
print(classification_report(y_test, y_pred_classes))
# WYKRESY
precision = precision_score(y_test, y_pred_classes, average=None)  # Precyzja dla każdej klasy
recall = recall_score(y_test, y_pred_classes, average=None)        # Czułość dla każdej klasy       
accuracy = np.mean(y_test == y_pred_classes)
classes = range(10)  
epochs = range(1, len(history.history['accuracy']) + 1)
fig, axes = plt.subplots(1, 3, figsize=(18, 6)) 

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

# Wykres dokładności
axes[2].plot(epochs, history.history['accuracy'], marker='o', label='Dokładność treningowa', color='blue')
axes[2].plot(epochs, history.history['val_accuracy'], marker='o', label='Dokładność walidacyjna', color='orange')
axes[2].set_title('Dokładność w zależności od epoki')
axes[2].set_xlabel('Epoka')
axes[2].set_ylabel('Dokładność')
axes[2].set_xticks(epochs)
axes[2].grid()
axes[2].legend()

plt.tight_layout()
plt.savefig('wykresy/1wykresy.png') 
plt.show()

# model.save('model_mnist.keras')  # Zapisanie modelu do pliku