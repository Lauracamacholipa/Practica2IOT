# **4. Pruebas y Validaciones**

## 4.1. Objetivo de las pruebas

El objetivo de las pruebas es verificar que el sistema desarrollado cumple con los requerimientos funcionales definidos, evaluando:

- la precisión de medición del sensor ultrasónico
- la correcta clasificación de los rangos de distancia
- la transmisión confiable de datos entre el cliente sensor y el servidor
- la correcta ejecución de comandos en el cliente actuador

## 4.2. Metodología de pruebas

Las pruebas se realizaron utilizando el prototipo completo, compuesto por el cliente sensor, el servidor TCP y el cliente actuador.

El procedimiento seguido fue:

1. Colocar un objeto frente al sensor ultrasónico a diferentes distancias.
2. Medir la distancia real utilizando una regla.
3. Observar la distancia enviada por el cliente sensor y procesada por el sistema.
4. Verificar la recepción del mensaje en el servidor y la generación del comando correspondiente.
5. Observar el comportamiento del cliente actuador (LED activado).
6. Comparar los resultados obtenidos con los valores esperados.

Se realizaron 30 mediciones experimentales, distribuidas en los tres rangos definidos en el sistema:

| Rango de distancia | LED esperado |
| --- | --- |
| 2 – 30 cm | Rojo |
| 30 - 100 cm | Amarillo |
| 100 – 150 cm | Verde |

## 4.3 Validación de la comunicación

Durante la ejecución del sistema se verificó el correcto funcionamiento del proceso de comunicación entre los componentes del sistema distribuido (cliente sensor, servidor y cliente actuador).

Se observó que:

- El cliente sensor logra establecer conexión con el servidor mediante el protocolo TCP sobre red WiFi.
- El servidor recibe los datos enviados por el sensor y procesa correctamente la información.
- El cliente actuador se registra en el servidor y permanece en espera de instrucciones.
- Los comandos generados por el servidor son recibidos por el actuador y ejecutados correctamente.

Asimismo, se verificó el funcionamiento del mecanismo de control implementado, incluyendo:

- El envío de mensajes estructurados entre los dispositivos.
- La confirmación de recepción de mensajes.
- La continuidad del sistema ante reconexiones de red.

Estas observaciones permiten validar el correcto funcionamiento de la comunicación dentro del sistema distribuido.

## 4.4 Registro de mediciones

Durante las pruebas se obtuvieron las siguientes mediciones experimentales.

<img width="572" height="667" alt="image" src="https://github.com/user-attachments/assets/7fdf2f26-f7d5-4578-ae62-1ff7c9ffb258" />

# **5. Resultados**

En esta sección se analizan los datos obtenidos durante las pruebas experimentales realizadas al sistema de medición de distancia con sensor ultrasónico.

A partir de las 30 mediciones realizadas, se calcularon los valores de error absoluto y error porcentual comparando la distancia real con la distancia medida por el sensor.

Los resultados muestran que el sistema presenta un error absoluto promedio de aproximadamente **1.46 cm** y un error porcentual promedio cercano al **2.23 %**.

Se observó que la diferencia entre la distancia real y la distancia medida aumenta ligeramente a distancias mayores, lo cual es un comportamiento esperado en sensores ultrasónicos debido a factores como la dispersión de la onda ultrasónica y las condiciones del entorno.

En cuanto al funcionamiento del sistema de clasificación por rangos de distancia, los resultados obtenidos fueron:

- Rango cercano (2 – 30 cm): activación correcta del LED rojo en todas las pruebas.
- Rango medio (30 – 100 cm): activación correcta del LED amarillo en todas las pruebas.
- Rango lejano (100 – 150 cm): activación correcta del LED verde en todas las pruebas.

Durante las 30 pruebas realizadas, el LED observado coincidió con el LED esperado en todos los casos, lo que representa un **100 % de precisión en la clasificación de distancias**.

Estos resultados indican que el sistema funciona correctamente y cumple con los requerimientos funcionales establecidos para la medición y señalización de distancias.

Adicionalmente, durante la ejecución del sistema distribuido, se observó que la integración entre el cliente sensor, el servidor y el cliente actuador permite mantener el comportamiento esperado del sistema, conservando la lógica de clasificación y señalización definida en la implementación original.

# **6. Conclusiones**

A partir del desarrollo e implementación del sistema distribuido de medición de distancia, se concluye que el sistema cumple satisfactoriamente con los objetivos planteados, tanto en la medición de distancias como en la integración de los componentes mediante comunicación en red.

Las pruebas realizadas permitieron verificar el correcto funcionamiento del sensor ultrasónico y del algoritmo de medición, obteniendo un error promedio de aproximadamente **1.46 cm**, valor que se encuentra dentro de un rango aceptable para este tipo de sensores.

Asimismo, el sistema de señalización mediante LEDs demostró un funcionamiento correcto, activando el indicador correspondiente según el rango de distancia detectado, logrando una coincidencia del **100 %** entre el resultado esperado y el observado.

Por otro lado, la implementación del modelo cliente-servidor permitió separar las funciones del sistema en distintos componentes, manteniendo la lógica de operación y permitiendo la interacción entre dispositivos mediante comunicación TCP, lo cual representa una mejora respecto a la implementación anterior.

En general, los resultados obtenidos demuestran que el sistema es capaz de medir, procesar y representar la información de manera correcta dentro del rango de operación establecido, cumpliendo con los requerimientos funcionales definidos.

Finalmente, este proyecto permitió aplicar conceptos de sistemas embebidos, comunicación en red y arquitectura cliente-servidor, fortaleciendo el aprendizaje práctico en el desarrollo de sistemas distribuidos.

# **7. Recomendaciones**

A partir de los resultados obtenidos y de la implementación del sistema, se proponen las siguientes recomendaciones para mejorar su desempeño y facilitar futuras implementaciones:

- Realizar calibraciones periódicas del sensor ultrasónico para reducir el error en las mediciones, especialmente en distancias mayores.
- Implementar técnicas de filtrado de datos, como el promedio de múltiples mediciones, para mejorar la estabilidad de los valores obtenidos.
- Optimizar el manejo de la comunicación entre los dispositivos, incorporando mecanismos más robustos de control de errores y reconexión.
- Considerar la implementación de un sistema de monitoreo o visualización en tiempo real desde el servidor, para facilitar la supervisión del sistema.
- Evaluar el uso de protocolos de comunicación alternativos o más eficientes en aplicaciones con mayor escala o requerimientos de tiempo real.
- Asegurar condiciones adecuadas de operación del sensor, evitando interferencias y superficies que afecten la reflexión de la señal ultrasónica.

Estas recomendaciones pueden contribuir a mejorar la precisión, estabilidad y escalabilidad del sistema en futuras aplicaciones.
