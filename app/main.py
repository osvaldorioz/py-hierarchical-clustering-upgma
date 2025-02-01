from fastapi import FastAPI
from fastapi.responses import FileResponse
from pydantic import BaseModel
import numpy as np
from typing import List
import matplotlib
import matplotlib.pyplot as plt
import scipy.cluster.hierarchy as sch
import hierarchical_clustering_upgma
import json

matplotlib.use('Agg')  # Usar backend no interactivo
app = FastAPI()

# Definir el modelo para el vector
class VectorF(BaseModel):
    vector: List[float]
    
def plot_dendrogram(data, output_file:str):
    # Ejecutar el algoritmo de clustering desde el módulo compilado con Pybind11
    merges, num_points = hierarchical_clustering_upgma.hierarchical_clustering_upgma(data)
    
    # Convertir la lista de merges en un array compatible con scipy
    linkage_matrix = np.zeros((num_points - 1, 4))
    cluster_ids = list(range(num_points))  # Control de índices de cluster
    next_cluster_id = num_points  # Índices nuevos para los clusters fusionados
    
    for i, (a, b, dist) in enumerate(merges):
        linkage_matrix[i, 0] = cluster_ids[a]
        linkage_matrix[i, 1] = cluster_ids[b]
        linkage_matrix[i, 2] = dist
        linkage_matrix[i, 3] = len(cluster_ids)  # Tamaño del nuevo cluster
        
        # Asignar un nuevo ID al cluster fusionado
        cluster_ids[a] = next_cluster_id
        del cluster_ids[b]  # Remover el índice del cluster eliminado
        next_cluster_id += 1
    
    # Graficar el dendrograma
    plt.figure(figsize=(10, 5))
    sch.dendrogram(linkage_matrix, labels=np.arange(num_points))
    plt.title("Dendrograma - UPGMA")
    plt.xlabel("Índice del Punto")
    plt.ylabel("Distancia")
    #plt.show()
    plt.savefig(output_file)
    plt.close()

@app.post("/upgma-linkage")
def calculo(n_points: int):
    output_file = 'hierarchical_clustering_upgma_linkage.png'
    data = np.random.rand(n_points, 2)  # n puntos en 2D
    plot_dendrogram(data, output_file)
    
    j1 = {
        "Grafica": output_file
    }
    jj = json.dumps(str(j1))

    return jj

@app.get("/upgma-linkage-graph")
def getGraph(output_file: str):
    return FileResponse(output_file, media_type="image/png", filename=output_file)