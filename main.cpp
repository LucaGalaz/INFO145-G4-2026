#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>
#include <filesystem>
#include <algorithm>
#include "include/caso1.hpp"
#include "include/caso2.hpp"
#include "include/caso3.hpp"

using namespace std;
using namespace std::chrono;

// Ejecuta las mediciones de construccion y busqueda para una distribucion dada
void medirYRegistrar(ofstream& archivo_csv, const string& distribucion, const vector<uint64_t>& arr, size_t N, size_t salto_b, size_t num_busquedas, double t_c1, int64_t& suma_total) {
    auto inicio = high_resolution_clock::now();
    EstructuraGapCoding c2 = construirGapCoding(arr, salto_b);
    auto fin = high_resolution_clock::now();
    duration<double, milli> t_c2 = fin - inicio;

    inicio = high_resolution_clock::now();
    EstructuraCompresion c3g = construirEliasGamma(arr, salto_b);
    fin = high_resolution_clock::now();
    duration<double, milli> t_c3g = fin - inicio;

    inicio = high_resolution_clock::now();
    EstructuraCompresion c3d = construirEliasDelta(arr, salto_b);
    fin = high_resolution_clock::now();
    duration<double, milli> t_c3d = fin - inicio;

    vector<uint64_t> valores_busq(num_busquedas);
    for (size_t i = 0; i < num_busquedas; ++i) {
        valores_busq[i] = arr[rand() % N];
    }

    auto ini_busq = high_resolution_clock::now();
    for (size_t i = 0; i < num_busquedas; ++i) suma_total += busquedaBinaria(arr, valores_busq[i]);
    auto fin_busq = high_resolution_clock::now();
    duration<double, milli> t_b1 = fin_busq - ini_busq;

    ini_busq = high_resolution_clock::now();
    for (size_t i = 0; i < num_busquedas; ++i) suma_total += busquedaGapCoding(c2, valores_busq[i]);
    fin_busq = high_resolution_clock::now();
    duration<double, milli> t_b2 = fin_busq - ini_busq;

    ini_busq = high_resolution_clock::now();
    for (size_t i = 0; i < num_busquedas; ++i) suma_total += busquedaEliasGamma(c3g, valores_busq[i]);
    fin_busq = high_resolution_clock::now();
    duration<double, milli> t_b3g = fin_busq - ini_busq;

    ini_busq = high_resolution_clock::now();
    for (size_t i = 0; i < num_busquedas; ++i) suma_total += busquedaEliasDelta(c3d, valores_busq[i]);
    fin_busq = high_resolution_clock::now();
    duration<double, milli> t_b3d = fin_busq - ini_busq;

    archivo_csv << "Caso 1," << distribucion << "," << N << "," << t_c1 << "," << t_b1.count() << "," << calcularEspacioCaso1(arr) << "\n";
    archivo_csv << "Caso 2," << distribucion << "," << N << "," << t_c2.count() << "," << t_b2.count() << "," << calcularEspacioCaso2(c2) << "\n";
    archivo_csv << "Caso 3 (Elias Gamma)," << distribucion << "," << N << "," << t_c3g.count() << "," << t_b3g.count() << "," << calcularEspacioCompresion(c3g) << "\n";
    archivo_csv << "Caso 3 (Elias Delta)," << distribucion << "," << N << "," << t_c3d.count() << "," << t_b3d.count() << "," << calcularEspacioCompresion(c3d) << "\n";
}

// Controla el flujo automatizado de pruebas en las distintas escalas
void ejecutarBenchmark() {
    std::filesystem::create_directories("resultados");
    ofstream archivo_csv("resultados/salida_metricas.csv");
    archivo_csv << "Caso,Distribucion,Tamano,TiempoConstruccion_ms,TiempoBusquedaTotal_ms,Espacio_Bytes\n";

    vector<size_t> tamanos = {1000000, 10000000}; 
    size_t num_busquedas = 10000;
    size_t salto_b = 32; 
    int64_t suma_total = 0; 

    for (size_t N : tamanos) {
        cout << "Evaluando distribucion Lineal para N = " << N << "..." << flush;
        auto inicio = high_resolution_clock::now();
        vector<uint64_t> arr_lin = generarDatosLineal(N, 10);
        auto fin = high_resolution_clock::now();
        duration<double, milli> t_c1_l = fin - inicio;

        medirYRegistrar(archivo_csv, "Lineal", arr_lin, N, salto_b, num_busquedas, t_c1_l.count(), suma_total);
        cout << " Listo." << endl;

        cout << "Evaluando distribucion Normal para N = " << N << "..." << flush;
        inicio = high_resolution_clock::now();
        vector<uint64_t> arr_nor = generarDatosNormal(N, N / 2.0, N / 6.0);
        fin = high_resolution_clock::now();
        duration<double, milli> t_c1_n = fin - inicio;

        medirYRegistrar(archivo_csv, "Normal", arr_nor, N, salto_b, num_busquedas, t_c1_n.count(), suma_total);
        cout << " Listo." << endl;
    }

    archivo_csv.close();
    if (suma_total == -1) cout << "Anti-optimizacion activo." << endl;
    cout << "\nBenchmark completado con exito. Resultados en resultados/salida_metricas.csv" << endl;
}

// Administra la carga externa de archivos y la busqueda interactiva
void ejecutarModoArchivo(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo en " << ruta << endl;
        return;
    }

    vector<uint64_t> datos;
    string aux_str;
    while (getline(archivo, aux_str, ',')) {
        if (!aux_str.empty()) {
            datos.push_back(stoull(aux_str)); 
        }
    }
    archivo.close();

    sort(datos.begin(), datos.end()); 
    size_t salto_b = 32;

    cout << "Cargados y ordenados " << datos.size() << " elementos." << endl;
    cout << "Indexando estructuras en memoria..." << endl;
    
    EstructuraGapCoding c2 = construirGapCoding(datos, salto_b);
    EstructuraCompresion c3g = construirEliasGamma(datos, salto_b);
    EstructuraCompresion c3d = construirEliasDelta(datos, salto_b);

    uint64_t valor;
    int opcion;
    while (true) {
        cout << "\nIngrese el valor a buscar o use otro tipo para salir: ";
        if (!(cin >> valor)) break;

        cout << "Seleccione estructura de consulta:\n";
        cout << " 1. Caso 1 (Representacion Explicita)\n";
        cout << " 2. Caso 2 (Gap-Coding)\n";
        cout << " 3. Caso 3 (Compresion con Elias Gamma y Delta)\n";
        cout << "Escriba opcion 1 a 3: ";
        if (!(cin >> opcion)) break;

        int64_t pos = -1;
        auto start = high_resolution_clock::now();
        
        if (opcion == 1) {
            pos = busquedaBinaria(datos, valor);
        } else if (opcion == 2) {
            pos = busquedaGapCoding(c2, valor);
        } else if (opcion == 3) {
            int sub_opc;
            cout << "\nSeleccione tipo de compresion:\n";
            cout << " 1. Elias Gamma\n";
            cout << " 2. Elias Delta\n";
            cout << "Seleccione opcion 1 o 2: ";
            if (!(cin >> sub_opc)) break;
            
            if (sub_opc == 1) pos = busquedaEliasGamma(c3g, valor);
            else if (sub_opc == 2) pos = busquedaEliasDelta(c3d, valor);
            else { cout << "Opcion invalida.\n"; continue; }
        } else { 
            cout << "Opcion invalida.\n"; 
            continue; 
        }

        auto end = high_resolution_clock::now();
        duration<double, milli> tiempo = end - start;

        if (pos != -1) {
            cout << "ENCONTRADO en indice: " << pos << " | Tiempo: " << tiempo.count() << " ms\n";
        } else {
            cout << "NO ENCONTRADO en la estructura. | Tiempo: " << tiempo.count() << " ms\n";
        }
    }
}

// Enruta la ejecucion segun los argumentos de consola
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Uso incorrecto del programa.\n";
        cout << " Modo automatico: ./main --benchmark\n";
        cout << " Modo archivo:    ./main -i ruta/archivo.csv\n";
        return 1;
    }

    string arg1 = argv[1];
    if (arg1 == "--benchmark") {
        ejecutarBenchmark();
    } else if (arg1 == "-i" && argc == 3) {
        ejecutarModoArchivo(argv[2]);
    } else {
        cout << "Parametros no validos.\n";
        return 1;
    }
    return 0;
}