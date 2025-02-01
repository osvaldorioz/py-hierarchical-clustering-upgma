### **Método UPGMA (Unweighted Pair Group Method with Arithmetic Mean)**  

**UPGMA** es un algoritmo de **clustering jerárquico aglomerativo** que se basa en la **media aritmética de las distancias** entre los puntos de diferentes clusters. Se usa principalmente en **biología evolutiva** y en **análisis de datos jerárquicos**.  

#### **Pasos del algoritmo UPGMA:**  
1. **Inicialización:** Cada punto es un cluster individual.  
2. **Cálculo de distancias:** Se construye una matriz de distancias entre todos los puntos.  
3. **Fusión de clusters:** Se eligen los dos clusters más cercanos y se fusionan en un nuevo cluster.  
4. **Actualización de distancias:** La distancia entre el nuevo cluster y los demás se calcula como el **promedio de las distancias entre los puntos de ambos clusters**.  
5. **Repetición:** Se repiten los pasos hasta que todos los puntos formen un único cluster.  

#### **Características de UPGMA:**  
✅ **Genera un dendrograma ultramétrico** (las distancias en el dendrograma representan tiempos de divergencia).  
✅ **Asume una tasa de evolución constante** en aplicaciones biológicas.  
❌ **Sensibilidad a datos no balanceados**, lo que puede afectar la calidad de los clusters.  

Es un método eficiente y ampliamente usado en la clasificación jerárquica de datos en diversas disciplinas. 🚀
