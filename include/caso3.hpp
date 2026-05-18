#ifndef CASO3_HPP
#define CASO3_HPP

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

// Estructura unificada para almacenar los datos compactados en bits
struct EstructuraCompresion {
    vector<uint64_t> bitstream;
    vector<uint64_t> sample;
    vector<uint64_t> bit_offsets;
    size_t b;
    size_t n;
};

// Escribe un bit individual en el flujo de memoria
inline void escribirBit(vector<uint64_t>& stream, uint64_t& cont_bits, bool bit) {
    size_t pos_v = cont_bits / 64;
    size_t pos_b = cont_bits % 64;
    if (pos_v >= stream.size()) {
        stream.push_back(0);
    }
    if (bit) {
        stream[pos_v] |= (1ULL << (63 - pos_b));
    }
    cont_bits++;
}

// Lee un bit individual desde el flujo de memoria
inline bool leerBit(const vector<uint64_t>& stream, uint64_t& puntero) {
    size_t pos_v = puntero / 64;
    size_t pos_b = puntero % 64;
    bool bit = (stream[pos_v] >> (63 - pos_b)) & 1;
    puntero++;
    return bit;
}

// Codifica un numero entero usando el metodo Elias Gamma
inline void codificarEliasGamma(vector<uint64_t>& stream, uint64_t& cont_bits, uint64_t valor) {
    int L = 63 - __builtin_clzll(valor);
    for (int i = 0; i < L; ++i) {
        escribirBit(stream, cont_bits, false);
    }
    for (int i = L; i >= 0; --i) {
        escribirBit(stream, cont_bits, (valor >> i) & 1);
    }
}

// Decodifica un numero entero en formato Elias Gamma
inline uint64_t decodificarEliasGamma(const vector<uint64_t>& stream, uint64_t& puntero) {
    int cont_ceros = 0;
    while (!leerBit(stream, puntero)) {
        cont_ceros++;
    }
    uint64_t valor = 1;
    for (int i = 0; i < cont_ceros; ++i) {
        valor = (valor << 1) | leerBit(stream, puntero);
    }
    return valor;
}

// Construye la estructura comprimida usando Elias Gamma
EstructuraCompresion construirEliasGamma(const vector<uint64_t>& A, size_t salto_b) {
    EstructuraCompresion res;
    res.b = salto_b;
    res.n = A.size();
    if (A.empty()) return res;

    uint64_t cont_bits = 0;
    for (size_t i = 0; i < res.n; ++i) {
        if (i % salto_b == 0) {
            res.sample.push_back(A[i]);
            res.bit_offsets.push_back(cont_bits);
        }
        uint64_t gap = (i == 0) ? A[0] : (A[i] - A[i - 1]);
        codificarEliasGamma(res.bitstream, cont_bits, gap + 1);
    }
    return res;
}

// Busca un elemento en el flujo comprimido con Elias Gamma
int64_t busquedaEliasGamma(const EstructuraCompresion& res, uint64_t elemento) {
    if (res.n == 0) return -1;
    auto it = upper_bound(res.sample.begin(), res.sample.end(), elemento);
    size_t pos_bloque = distance(res.sample.begin(), it);
    if (pos_bloque > 0) pos_bloque--;

    size_t ini = pos_bloque * res.b;
    size_t fin = min(ini + res.b, res.n);

    uint64_t puntero = res.bit_offsets[pos_bloque];
    uint64_t val_act = res.sample[pos_bloque];

    if (val_act == elemento) return ini;

    decodificarEliasGamma(res.bitstream, puntero);

    for (size_t i = ini + 1; i < fin; ++i) {
        uint64_t gap = decodificarEliasGamma(res.bitstream, puntero) - 1;
        val_act += gap;
        if (val_act == elemento) return i;
        if (val_act > elemento) return -1;
    }
    return -1;
}

// Codifica un numero entero usando el metodo Elias Delta
inline void codificarEliasDelta(vector<uint64_t>& stream, uint64_t& cont_bits, uint64_t valor) {
    int L = 63 - __builtin_clzll(valor);
    codificarEliasGamma(stream, cont_bits, L + 1);
    for (int i = L - 1; i >= 0; --i) {
        escribirBit(stream, cont_bits, (valor >> i) & 1);
    }
}

// Decodifica un numero entero en formato Elias Delta
inline uint64_t decodificarEliasDelta(const vector<uint64_t>& stream, uint64_t& puntero) {
    uint64_t L_mas_uno = decodificarEliasGamma(stream, puntero);
    uint64_t L = L_mas_uno - 1;
    uint64_t valor = 1;
    for (uint64_t i = 0; i < L; ++i) {
        valor = (valor << 1) | leerBit(stream, puntero);
    }
    return valor;
}

// Construye la estructura comprimida usando Elias Delta
EstructuraCompresion construirEliasDelta(const vector<uint64_t>& A, size_t salto_b) {
    EstructuraCompresion res;
    res.b = salto_b;
    res.n = A.size();
    if (A.empty()) return res;

    uint64_t cont_bits = 0;
    for (size_t i = 0; i < res.n; ++i) {
        if (i % salto_b == 0) {
            res.sample.push_back(A[i]);
            res.bit_offsets.push_back(cont_bits);
        }
        uint64_t gap = (i == 0) ? A[0] : (A[i] - A[i - 1]);
        codificarEliasDelta(res.bitstream, cont_bits, gap + 1);
    }
    return res;
}

// Busca un elemento en el flujo comprimido con Elias Delta
int64_t busquedaEliasDelta(const EstructuraCompresion& res, uint64_t elemento) {
    if (res.n == 0) return -1;
    auto it = upper_bound(res.sample.begin(), res.sample.end(), elemento);
    size_t pos_bloque = distance(res.sample.begin(), it);
    if (pos_bloque > 0) pos_bloque--;

    size_t ini = pos_bloque * res.b;
    size_t fin = min(ini + res.b, res.n);

    uint64_t puntero = res.bit_offsets[pos_bloque];
    uint64_t val_act = res.sample[pos_bloque];

    if (val_act == elemento) return ini;

    decodificarEliasDelta(res.bitstream, puntero);

    for (size_t i = ini + 1; i < fin; ++i) {
        uint64_t gap = decodificarEliasDelta(res.bitstream, puntero) - 1;
        val_act += gap;
        if (val_act == elemento) return i;
        if (val_act > elemento) return -1;
    }
    return -1;
}

// Computa la memoria ocupada por vectores internos
inline size_t calcularEspacioCompresion(const EstructuraCompresion& res) {
    return (res.bitstream.size() * 8) + (res.sample.size() * 8) + (res.bit_offsets.size() * 8);
}

#endif