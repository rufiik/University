import pandas as pd
import matplotlib.pyplot as plt

operations = ['insert_H1', 'insert_H2', 'union', 'getMin', 'extractMin']

for exp in range(1, 6):
    df = pd.read_csv(f'comparisons_exp{exp}.csv')
    fig, axes = plt.subplots(3, 1, figsize=(10, 15), sharey=False)

    # Wykres insert_H1 (tylko kropki)
    subset_h1 = df[df['operation'] == 'insert_H1']
    axes[0].scatter(subset_h1['index'], subset_h1['comparisons'], label='insert_H1')
    axes[0].set_title(f'Eksperyment {exp} - insert_H1')
    axes[0].set_xlabel('Numer operacji')
    axes[0].set_ylabel('Liczba porównań')
    axes[0].legend()

    # Wykres insert_H2 (tylko kropki)
    subset_h2 = df[df['operation'] == 'insert_H2']
    axes[1].scatter(subset_h2['index'], subset_h2['comparisons'], color='orange', label='insert_H2')
    axes[1].set_title(f'Eksperyment {exp} - insert_H2')
    axes[1].set_xlabel('Numer operacji')
    axes[1].set_ylabel('Liczba porównań')
    axes[1].legend()

    # Wykres union (kropka) + getMin (kropki) + extractMin (kropki)
    subset_union = df[df['operation'] == 'union']
    subset_getmin = df[df['operation'] == 'getMin']
    subset_extractmin = df[df['operation'] == 'extractMin']
    axes[2].scatter(subset_union['index'], subset_union['comparisons'], color='red', label='union', s=80, zorder=3)
    axes[2].scatter(subset_getmin['index'], subset_getmin['comparisons'], color='green', label='getMin')
    axes[2].scatter(subset_extractmin['index'], subset_extractmin['comparisons'], color='purple', label='extractMin')
    axes[2].set_title(f'Eksperyment {exp} - union, getMin, extractMin')
    axes[2].set_xlabel('Numer operacji')
    axes[2].set_ylabel('Liczba porównań')
    axes[2].legend()

    plt.tight_layout()
    plt.savefig(f'zad3_exp{exp}.png')
    plt.close()