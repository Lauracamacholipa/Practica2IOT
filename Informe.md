# **4. Pruebas y Validaciones**

## 4.1. Objetivo de las pruebas

El objetivo de las pruebas es verificar que el sistema desarrollado cumple con los requerimientos funcionales definidos, evaluando:

- la precisión de medición del sensor ultrasónico
- la correcta transimisión de datos mediante TCP con control de secuencia
- la correcta clasificación de los rangos de distancia y activación del actuador
- el funcionamiento del mecanismo de confirmación y control de mensajes
- la estabilidad y comportamiento del sistema ante condiciones de operación

## 4.2. Metodología de pruebas

Las pruebas se realizaron utilizando el prototipo completo, compuesto por el cliente sensor, el servidor TCP y el cliente actuador.

El procedimiento general fue el siguiente:

1. Colocar un objeto frente al sensor ultrasónico a diferentes distancias.
2. Medir la distancia real utilizando una regla.
3. Observar la distancia enviada por el cliente sensor y procesada por el sistema.
4. Verificar la recepción del mensaje en el servidor y la generación del comando correspondiente.
5. Observar la respuesta del cliente actuador (LED activado).
6. Evaluar el comportamiento del sistema ante fallos (desconexión, pérdida de mensajes, duplicados).

Se realizaron pruebas específicas para cada requerimiento, incluyendo:
- mediciones de distancia
- pruebas de comunicación TCP
- validación de ACK y reintentos
- prueba de reconexión
- evaluación de estabilidad del sistema

## 4.3 Validación de la comunicación

Durante la ejecución del sistema se verificó el correcto funcionamiento del proceso de comunicación entre los componentes del sistema distribuido (cliente sensor, servidor y cliente actuador).

Se observó que:

- El cliente sensor establece conexión con el servidor mediante el protocolo TCP sobre red WiFi.
- Los mensajes enviados incluyen un número de secuencia único.
- El servidor recibe los datos enviados por el sensor y procesa la información.
- El cliente actuador se registra en el servidor y permanece en espera de instrucciones.
- Los comandos generados por el servidor son recibidos por el actuador y ejecutados correctamente.

Asimismo, se verificó el funcionamiento del mecanismo de control implementado, incluyendo:

- El envío de mensajes estructurados entre los dispositivos.
- La confirmación de recepción de mensajes.
- La continuidad del sistema ante reconexiones de red.
  
También se observo que el sistema mantiene la comunicación tras reconexiones y continúa operando ante condiciones anómalas.

## 4.4 Resultados experimentales

Durante las pruebas se obtuvieron las siguiente mediciones y validaciones experimentales:

### 4.4.1 Registro de mediciones

<img width="572" height="667" alt="image" src="https://github.com/user-attachments/assets/7fdf2f26-f7d5-4578-ae62-1ff7c9ffb258" />

### 4.4.2 Envío TCP con secuencia

<img width="725" height="279" alt="image" src="https://github.com/user-attachments/assets/0d6d633f-1045-4640-816d-af9dce4f19bd" />

### 4.4.3 Clasificación de LEDs y comandos

<img width="865" height="215" alt="image" src="https://github.com/user-attachments/assets/c259ba11-530b-487a-b53f-05c72d6d01d0" />

### 4.4.4 Reintentos de conexión con ACK y tiempo de reconexión automática

<img width="865" height="448" alt="image" src="https://github.com/user-attachments/assets/90ade78d-1039-4571-be32-3fab048ad4f6" />
<img width="733" height="148" alt="image" src="https://github.com/user-attachments/assets/2eeaa5a9-3243-4fbd-a94f-ddd40c50d3d3" />

### 4.4.5 Manejo de valores inválidos

<img width="730" height="179" alt="image" src="https://github.com/user-attachments/assets/ba5e9918-74f2-401b-9358-efa5d1d57b8e" />

### 4.4.6 Estabilidad del sistema

<img width="728" height="211" alt="image" src="https://github.com/user-attachments/assets/0e077601-8afd-44aa-b0ed-ad4b06ae4480" />

# **5. Resultados**

Los resultados obtenidos evidencian un comportamiento consistente y adecuado del sistema en relación con los requerimientos planteados. En cuanto a la medición de distancia, se obtuvo un error absoluto promedio de 1.46 cm y un error porcentual promedio de 2.23 %, lo cual indica un nivel de precisión aceptable dentro del rango operativo definido. Se observa que el error tiende a incrementarse ligeramente en distancias cercanas al límite superior, lo cual es coherente con las limitaciones propias del sensor ultrasónico, influenciadas por factores como la dispersión de la señal y las características de la superficie del objeto.

Respecto a la clasificación de distancias, el sistema presentó un comportamiento completamente correcto, ya que en todos los casos evaluados el LED activado coincidió con el esperado según el rango definido, los resultados obtenidos fueron:

- Rango cercano (2 – 30 cm): activación correcta del LED rojo en todas las pruebas.
- Rango medio (30 – 100 cm): activación correcta del LED amarillo en todas las pruebas.
- Rango lejano (100 – 150 cm): activación correcta del LED verde en todas las pruebas.

En términos de comunicación, la transmisión de datos entre cliente y servidor se realizó de manera confiable, manteniendo el orden correcto de los mensajes y garantizando su recepción. Asimismo, los mecanismos de control implementados permitieron confirmar la entrega de mensajes mediante ACK, ejecutar reintentos cuando fue necesario y detectar correctamente situaciones de pérdida de mensajes. En los casos donde no se recibió confirmación, el sistema aplicó el número máximo de intentos establecidos, evidenciando un comportamiento esperado ante fallos de comunicación.

Por otro lado, el sistema demostró capacidad de recuperación ante interrupciones, logrando restablecer la conexión automáticamente en tiempos reducidos. Además, las condiciones anómalas, como la ausencia de eco o mediciones fuera de rango, fueron gestionadas adecuadamente sin afectar la continuidad del sistema. Durante las pruebas de funcionamiento continuo, el sistema se mantuvo estable, registrando únicamente eventos menores que no comprometieron su operación.

# **6. Conclusiones**

A partir del desarrollo e implementación del sistema distribuido de medición de distancia, se concluye que el sistema cumple satisfactoriamente con los objetivos planteados, tanto en la medición de distancias como en la integración de los componentes mediante comunicación en red.

Las pruebas realizadas permitieron verificar el correcto funcionamiento del sensor ultrasónico y del algoritmo de medición, obteniendo un error promedio de aproximadamente **1.46 cm**, valor que se encuentra dentro de un rango aceptable para este tipo de sensores. El sistema de clasificación por rangos demostró un desempeño óptimo, alcanzando un 100 % de precisión, lo que indica que la lógica implementada es adecuada y robusta frente a variaciones en la medición.

Por otro lado, la implementación del modelo cliente-servidor permitió separar las funciones del sistema en distintos componentes, manteniendo la lógica de operación y permitiendo la interacción entre dispositivos mediante comunicación TCP, lo cual representa una mejora respecto a la implementación anterior. Adicionalmente, el sistema demostró ser capaz de recuperarse automáticamente ante fallos de conexión, continuar operando ante condiciones anómalas y mantener estabilidad durante ejecuciones prolongadas

En general, los resultados obtenidos demuestran que el sistema es capaz de medir, procesar y representar la información de manera correcta dentro del rango de operación establecido, cumpliendo con los requerimientos funcionales definidos.

Finalmente, este proyecto permitió aplicar conceptos de sistemas embebidos, comunicación en red y arquitectura cliente-servidor, fortaleciendo el aprendizaje práctico en el desarrollo de sistemas distribuidos.

# **7. Recomendaciones**

A partir de los resultados obtenidos y de la implementación del sistema, se proponen las siguientes recomendaciones para mejorar su desempeño y facilitar futuras implementaciones:

- Realizar calibraciones periódicas del sensor ultrasónico, ya que se evidenció un incremento del error en distancias cercanas al límite superior del rango, lo cual puede afectar la precisión en escenarios más exigentes.
- Implementar técnicas de filtrado de datos, como el promedio o la mediana de múltiples mediciones consecutivas, para mejorar la estabilidad de los valores obtenidos.
- Mejorar el mecanismo de reconexión automática, considerando estrategias como tiempos de espera progresivos (backoff), para hacer el sistema más eficiente frente a interrupciones frecuentes de red.
- Considerar la incorporación de una interfaz de visualización en tiempo real, que permita observar el estado del sistema, las mediciones recibidas y las acciones ejecutadas, mejorando la supervisión y usabilidad.
- Evaluar el uso de protocolos de comunicación alternativos o más eficientes en aplicaciones con mayor escala o requerimientos de tiempo real.
- Asegurar condiciones adecuadas de operación del sensor, evitando superficies irregulares, materiales absorbentes o ángulos de incidencia desfavorables, ya que estos factores pueden influir directamente en la precisión de las mediciones observadas.

Estas recomendaciones pueden contribuir a mejorar la precisión, estabilidad y escalabilidad del sistema en futuras aplicaciones.
