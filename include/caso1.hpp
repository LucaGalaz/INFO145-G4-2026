#ifndef CASO1_HPP
#define CASO1_HPP

#include <vector>
#include <random>
#include <algorithm>

using namespace std;

// Genera datos ordenados con incrementos lineales aleatorios
vector<uint64_t> generarDatosLineal(size_t n, uint64_t epsilon) {
    vector<uint64_t> A(n);
    if (n == 0) return A;
    
    A[0] = rand() % 1000;
    for (size_t i = 1; i < n; ++i) {
        A[i] = A[i - 1] + (rand() % epsilon);
    }
    return A;
}

// Genera datos con distribucion normal y luego los ordena
vector<uint64_t> generarDatosNormal(size_t n, double media, double desv_estandar) {
    vector<uint64_t> A(n);
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> d(media, desv_estandar);

    for (size_t i = 0; i < n; ++i) {
        double val = d(gen);
        A[i] = (val < 0) ? 0 : static_cast<uint64_t>(val);
    }
    sort(A.begin(), A.end());
    return A;
}

// Busqueda binaria estandar en un arreglo ordenado
int64_t busquedaBinaria(const vector<uint64_t>& A, uint64_t elemento) {
    int64_t izq = 0;
    int64_t der = A.size() - 1;

    while (izq <= der) {
        int64_t medio = izq + (der - izq) / 2;
        if (A[medio] == elemento) return medio;
        if (A[medio] < elemento) izq = medio + 1;
        else der = medio - 1;
    }
    return -1;
}

// Calcula el espacio ocupado por el arreglo explicito
size_t calcularEspacioCaso1(const vector<uint64_t>& A) {
    return A.size() * sizeof(uint64_t);
}

#endif