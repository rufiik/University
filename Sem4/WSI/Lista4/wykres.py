import pandas as pd
import matplotlib.pyplot as plt

# Wczytaj dane
df = pd.read_csv("wyniki/dbscan_gridsearch.csv")

fig, axs = plt.subplots(1, 3, figsize=(18, 6))

# Liczba klastrów vs eps
for ms in sorted(df['min_samples'].unique()):
    subset = df[df['min_samples'] == ms]
    axs[0].plot(subset['eps'], subset['n_clusters'], label=f"min_samples={ms}")
axs[0].set_xlabel("eps")
axs[0].set_ylabel("Liczba klastrów")
axs[0].set_title("Liczba klastrów vs eps")
axs[0].legend()
axs[0].grid(True)

# Purity vs eps
for ms in sorted(df['min_samples'].unique()):
    subset = df[df['min_samples'] == ms]
    axs[1].plot(subset['eps'], subset['purity'], label=f"min_samples={ms}")
axs[1].set_xlabel("eps")
axs[1].set_ylabel("Purity (%)")
axs[1].set_title("Purity vs eps")
axs[1].legend()
axs[1].grid(True)

# Szum vs eps
for ms in sorted(df['min_samples'].unique()):
    subset = df[df['min_samples'] == ms]
    axs[2].plot(subset['eps'], subset['noise_percent'], label=f"min_samples={ms}")
axs[2].set_xlabel("eps")
axs[2].set_ylabel("Procent szumu")
axs[2].set_title("Procent szumu vs eps")
axs[2].legend()
axs[2].grid(True)

plt.tight_layout()
plt.savefig("wyniki/dbscan_wszystko_obok.png")
plt.show()