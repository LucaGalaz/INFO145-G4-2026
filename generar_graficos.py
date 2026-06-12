import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

ruta_csv = 'resultados/salida_metricas.csv'
if not os.path.exists(ruta_csv):
    ruta_csv = 'salida_metricas.csv'

df = pd.read_csv(ruta_csv)

plt.rcParams.update({
    'font.size': 10,
    'axes.labelsize': 11,
    'xtick.labelsize': 9,
    'ytick.labelsize': 9,
    'legend.fontsize': 9,
    'figure.titlesize': 12
})

casos = ['Caso 1', 'Caso 2', 'Caso 3 (Elias Gamma)', 'Caso 3 (Elias Delta)']
tamanos = [1000000, 10000000]
labels_x = ['1M', '10M']
colores = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
width = 0.18

fig_time, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5.5), sharey=True)
x = np.arange(len(tamanos))

limite_tiempo = 2.4
ticks_tiempo = np.arange(0, limite_tiempo + 0.1, 0.2)

for ax, dist, titulo in zip([ax1, ax2], ['Lineal', 'Normal'], ['Distribución Lineal', 'Distribución Normal']):
    for i, (caso, color) in enumerate(zip(casos, colores)):
        sub_df = df[(df['Caso'] == caso) & (df['Distribucion'] == dist)].sort_values('Tamano')
        valores = sub_df['TiempoBusquedaTotal_ms'].values
        
        bars = ax.bar(x + (i - 1.5) * width, valores, width, label=caso, color=color)
        
        for bar in bars:
            height = bar.get_height()
            ax.annotate(f'{height:.3f} ms',
                        xy=(bar.get_x() + bar.get_width() / 2, height),
                        xytext=(0, 3),  
                        textcoords="offset points",
                        ha='center', va='bottom', fontsize=8, rotation=15)

    ax.set_title(titulo, fontweight='bold')
    ax.set_xlabel('Tamaño del Arreglo (N)')
    ax.set_xticks(x)
    ax.set_xticklabels(labels_x)
    ax.set_ylim(0, limite_tiempo)
    ax.set_yticks(ticks_tiempo)
    ax.grid(axis='y', linestyle='--', alpha=0.6)

ax1.set_ylabel('Tiempo Total de Búsqueda (ms)')
ax1.legend(loc='upper left')

plt.suptitle('Comparación del Tiempo de Ejecución en Búsquedas (Lote de 10.000 Consultas)', fontweight='bold', y=0.98)
plt.tight_layout()
plt.savefig('resultados/tiempo_busqueda_comparativo.png', dpi=600)
plt.close()

fig_space, (ax3, ax4) = plt.subplots(1, 2, figsize=(12, 5.5), sharey=True)

limite_espacio = 90.0
ticks_espacio = np.arange(0, limite_espacio + 1, 10)

for ax, dist, titulo in zip([ax3, ax4], ['Lineal', 'Normal'], ['Distribución Lineal', 'Distribución Normal']):
    for i, (caso, color) in enumerate(zip(casos, colores)):
        sub_df = df[(df['Caso'] == caso) & (df['Distribucion'] == dist)].sort_values('Tamano')
        valores_mb = sub_df['Espacio_Bytes'].values / (1024 * 1024)
        
        bars = ax.bar(x + (i - 1.5) * width, valores_mb, width, label=caso, color=color)
        
        for bar in bars:
            height = bar.get_height()
            ax.annotate(f'{height:.2f} MB',
                        xy=(bar.get_x() + bar.get_width() / 2, height),
                        xytext=(0, 3),
                        textcoords="offset points",
                        ha='center', va='bottom', fontsize=8, rotation=15)

    ax.set_title(titulo, fontweight='bold')
    ax.set_xlabel('Tamaño del Arreglo (N)')
    ax.set_xticks(x)
    ax.set_xticklabels(labels_x)
    ax.set_ylim(0, limite_espacio)
    ax.set_yticks(ticks_espacio)
    ax.grid(axis='y', linestyle='--', alpha=0.6)

ax3.set_ylabel('Espacio Ocupado en RAM (Megabytes)')
ax3.legend(loc='upper left')

plt.suptitle('Comparación del Consumo de Espacio Físico en Memoria RAM', fontweight='bold', y=0.98)
plt.tight_layout()
plt.savefig('resultados/consumo_espacio_comparativo.png', dpi=600)
plt.close()

print("[+] Gráficos generados.")