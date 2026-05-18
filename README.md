# Proyecto INFO145 - Grupo 4

## Instrucciones de Compilacion
El codigo es compatible con Linux y Windows, requiriendo el compilador g++ con soporte para el estandar C++17.

### En Linux
Abra la terminal en la carpeta raiz del proyecto y ejecute:
1. make clean
2. make

### En Windows
Abra la consola de MSYS2 UCRT64 en el directorio de los archivos y ejecute:
1. make clean
2. make

Esto creara el archivo ejecutable llamado 'main' en la misma carpeta raiz.

## Instrucciones de Ejecucion

Una vez generado el ejecutable, el programa se puede lanzar desde la terminal de la siguiente forma segun el modo que necesite:

### 1. Modo Automatico (--benchmark)
Para correr las pruebas masivas de 1 millon y 10 millones de datos en todas las estructuras de forma automatica, escriba en su terminal:
./main --benchmark

* Argumentos recibidos: `--benchmark`
* Salida producida: Crea la carpeta resultados/ y genera adentro el archivo salida_metricas.csv con las mediciones en limpio.

### 2. Modo Archivo Interactivo
Para cargar su propio archivo CSV con numeros separados por comas e interactuar con el menu de busqueda, escriba en su terminal:
./main -i ruta/del/archivo.csv

* Argumentos recibidos: `-i` seguido por la ruta del archivo.
* Salida producida: Despliega un menu interactivo en la consola que pide un valor a buscar y muestra su indice real junto con los milisegundos que tomo encontrarlo.
