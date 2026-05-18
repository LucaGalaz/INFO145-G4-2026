#ifndef CASO2_HPP
#define CASO2_HPP

#include <vector>
#include <algorithm>

using namespace std;

// Estructura para almacenar los gaps y el indice de muestreo
struct EstructuraGapCoding {
    vector<uint64_t> GC;
    vector<uint64_t> sample;
    size_t b;
    size_t n;
};

// Construye el arreglo de diferencias y toma muestras cada b elementos
EstructuraGapCoding construirGapCoding(const vector<uint64_t>& A, size_t salto_b) {
    EstructuraGapCoding res;
    res.b = salto_b;
    res.n = A.size();
    
    if (A.empty()) return res;

    res.GC.resize(res.n);
    res.GC[0] = A[0];
    for (size_t i = 1; i < res.n; ++i) {
        res.GC[i] = A[i] - A[i - 1];
    }

    for (size_t i = 0; i < res.n; i += salto_b) {
        res.sample.push_back(A[i]);
    }

    return res;
}

// Busca un elemento usando el indice sample y luego recorre los gaps
int64_t busquedaGapCoding(const EstructuraGapCoding& est, uint64_t elemento) {
    if (est.n == 0) return -1;

    auto it = upper_bound(est.sample.begin(), est.sample.end(), elemento);
    size_t pos_bloque = distance(est.sample.begin(), it);
    if (pos_bloque > 0) {
        pos_bloque--;
    }

    size_t ini = pos_bloque * est.b;
    size_t fin = min(ini + est.b, est.n);
    uint64_t val_act = est.sample[pos_bloque];

    if (val_act == elemento) return ini;

    for (size_t i = ini + 1; i < fin; ++i) {
        val_act += est.GC[i];
        if (val_act == elemento) return i;
        if (val_act > elemento) return -1;
    }
    return -1;
}

// Calcula el espacio total usado por el caso 2
size_t calcularEspacioCaso2(const EstructuraGapCoding& est) {
    return (est.GC.size() * sizeof(uint64_t)) + (est.sample.size() * sizeof(uint64_t));
}

#endif