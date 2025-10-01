import os
import numpy as np
from PIL import Image, ImageOps
from tensorflow.keras.models import load_model
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix, ConfusionMatrixDisplay, precision_score, recall_score
import matplotlib.pyplot as plt
import struct
import seaborn as sns
def load_idx(filename):
    with open(filename, 'rb') as f:
        magic, size = struct.unpack(">II", f.read(8))
        if magic == 2051:  # Plik z obrazami
            rows, cols = struct.unpack(">II", f.read(8))
            data = np.fromfile(f, dtype=np.uint8).reshape(size, rows, cols)
        elif magic == 2049:  # Plik z etykietami
            data = np.fromfile(f, dtype=np.uint8)
        else:
            raise ValueError("Nieprawidłowy magic number w pliku IDX")
        return data
    
emnist_images_path = "emnist/emnist-mnist-test-images-idx3-ubyte"
emnist_labels_path = "emnist/emnist-mnist-test-labels-idx1-ubyte"

x_emnist = load_idx(emnist_images_path)
y_emnist = load_idx(emnist_labels_path)

x_emnist = x_emnist / 255.0
x_emnist = x_emnist.reshape(-1, 28, 28)

def load_custom_images(folder_path):
    images = []
    labels = []
    for label in range(10):  # Cyfry od 0 do 9
        label_folder = os.path.join(folder_path, str(label))
        for file_name in os.listdir(label_folder):
            file_path = os.path.join(label_folder, file_name)
            img = Image.open(file_path).convert('L')  # Konwersja do skali szarości

            img = img.rotate(-90,expand=True) # Obrót o 90 stopni w lewo
            img = ImageOps.mirror(img)  # Odbicie lustrzane
            img_array = np.array(img) / 255.0  # Normalizacja do zakresu [0, 1]
            images.append(img_array.flatten())  # Spłaszczenie obrazu
            labels.append(label)
    return np.array(images), np.array(labels)

custom_test_folder = "liczby"

x_custom_test, y_custom_test = load_custom_images(custom_test_folder)
x_custom_test = 1 - x_custom_test # Odwrócenie kolorów (tło na czarny, liczby na biały)
x_custom_test = x_custom_test.reshape(-1, 28, 28)  
model = load_model('model_mnist.keras')

# Predykcja na własnym zbiorze testowym
y_custom_pred = np.argmax(model.predict(x_custom_test), axis=1)

custom_accuracy = accuracy_score(y_custom_test, y_custom_pred)
print(f"Współczynnik prawidłowej rozpoznawalności (dokładność) na własnym zbiorze: {custom_accuracy:.4f}")

print("\nSzczegółowy raport klasyfikacji:")
print(classification_report(y_custom_test, y_custom_pred))

batch_size = 10
# Wyświetlanie obrazów w partiach
for batch_start in range(0, len(x_custom_test), batch_size):
    batch_end = min(batch_start + batch_size, len(x_custom_test)) 
    plt.figure(figsize=(15, 6))  # Rozmiar wykresu
    for i, idx in enumerate(range(batch_start, batch_end)):
        plt.subplot(2, 5, i + 1)  #2x5
        rotated_image = np.rot90(x_custom_test[idx], k=-1)  # Obrót o 90 stopni w prawo
        flipped_image = np.fliplr(rotated_image)
        plt.imshow(flipped_image, cmap='gray')
        plt.title(f"Label: {y_custom_test[idx]}\nPred: {y_custom_pred[idx]}")
        plt.axis('off')
    plt.tight_layout()
    plt.show()

# 1. Macierz pomyłek
cm = confusion_matrix(y_custom_test, y_custom_pred)
disp = ConfusionMatrixDisplay(confusion_matrix=cm, display_labels=range(10))
disp.plot(cmap='Blues', xticks_rotation=45)
plt.title("Macierz pomyłek")
plt.savefig("wykresy/2macierz_pomylek.png") 
plt.show()
# 2. Precyzja, czułość 
precision = precision_score(y_custom_test, y_custom_pred, average=None)
recall = recall_score(y_custom_test, y_custom_pred, average=None)
classes = range(10)  # Klasy od 0 do 9
plt.figure(figsize=(12, 6))
plt.plot(classes, precision, marker='o', label='Precyzja', color='blue')
plt.plot(classes, recall, marker='o', label='Czułość', color='green')
plt.title('Precyzja, Czułość  dla każdej klasy')

plt.xlabel('Klasa')
plt.ylabel('Wartość')
plt.xticks(classes)
plt.legend()
plt.grid()
plt.savefig("wykresy/2precyzja_czulosc.png")
plt.show()
# 3. Rozkład przewidywań
predictions_count = np.bincount(y_custom_pred, minlength=10)
plt.figure(figsize=(10, 6))
sns.barplot(x=list(range(10)), y=predictions_count, palette='viridis')
plt.title("Rozkład przewidywań modelu")
plt.xlabel("Klasa")
plt.ylabel("Liczba przewidywań")
plt.savefig("wykresy/2rozklad_przewidywan.png")
plt.show()