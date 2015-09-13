/*
* File: vetor3d.cpp
* Author: Nino
*
* Created on September 4, 2015, 10:28 AM
*
* Arquivo que contém todas as implementações dos construtores e
* métodos da classe vetor3d definida em vetor3d.h.
*/

#include <cmath>
#include "vetor3d.h"

vetor3d::vetor3d( ) { }

vetor3d::vetor3d(const vetor3d &orig) {

    this->x = orig.x;
    this->y = orig.y;
    this->z = orig.z;
}

vetor3d::vetor3d (const double x, const double y, const double z) {

    this->x = x;
    this->y = y;
    this->z = z;
}

double vetor3d::getX () const {

    return x;
}

double vetor3d::getY () const {

    return y;
}

double vetor3d::getZ () const {

    return z;
}

// Soma de Vetores -> V3 = V1 + v2
// Obs.: V3 = ( V1x + V2x , V1y + V2y , V1z + V2z)
const vetor3d vetor3d::operator+ (const vetor3d &v) const {

    vetor3d ret;

    // Operadores podem acessar atributos private...
    ret.x = this->x + v.x;
    ret.y = this->y + v.y;
    ret.z = this->z + v.z;

    return ret;
}

// Subtração de Vetores -> V3 = V1 - V2
// Obs.: V3 = ( V1x - V2x , V1y - V2y , V1z - V2z)
const vetor3d vetor3d::operator- (const vetor3d &v) const {

    vetor3d ret;

    ret.x = this->x - v.x;
    ret.y = this->y - v.y;
    ret.z = this->z - v.z;

    return ret;
}

// Multiplicação por Escalar -> V2 = V1 * e
// Obs.: V2 = ( V1x * e , V1y * e , V1z * e)
const vetor3d vetor3d::operator* (const double &e) const {

    vetor3d ret;

    ret.x = this->x * e;
    ret.y = this->y * e;
    ret.z = this->z * e;

    return ret;
}

// Divisão por Escalar -> V2 = V1 / e
// Obs.: V2 = ( V1x / e , V1y / e , V1z / e)
const vetor3d vetor3d::operator/ (const double &e) const {

    vetor3d ret;

    ret.x = this->x / e;
    ret.y = this->y / e;
    ret.z = this->z / e;

    return ret;
}

// Produto Escalar -> e = V1 * V2
// Obs.: e = (V1x * V2x) + (V1y * V2y) + (V1z * V2z)
const double vetor3d::operator* (const vetor3d &v) const {

    double xNovo = this->x * v.x;
    double yNovo = this->y * v.y;
    double zNovo = this->z * v.z;

    return (xNovo + yNovo + zNovo);
}

// Produto Vetorial -> V3 = V1 & V2
// Obs.: V1 x V2 = ( (V1y * V2z) - (V2y * V1z) , (V1x * V2z) - (V2x * V1z) , (V1x * V2y) - (V2x * V1y))
const vetor3d vetor3d::operator& (const vetor3d &v) const {

    vetor3d ret;

    ret.x = (this->y * v.z) - (v.y * this->z);
    ret.y = (this->x * v.z) - (v.x * this->z);
    ret.z = (this->x * v.y) - (v.x * this->y);

    return ret;
}

// Norma de um Vetor -> e = V1++
// Obs.: || V1 || = sqrt(x² + y² + z²)
const double vetor3d::operator++ (const int) const {

    double xNovo = pow(this->x, 2);
    double yNovo = pow(this->y, 2);
    double zNovo = pow(this->z, 2);
    double resp = xNovo + yNovo + zNovo;

    return sqrt(resp);
}

// Área do Triângulo -> e = V1 / V2
// Obs.: A = 0.5 * || V1 x V2 ||
const double vetor3d::operator/ (const vetor3d &v) const {

    vetor3d temp = *this & v;

    double resp = temp++;
    resp = resp * 0.5;

    return resp;
}

// Normalização de um Vetor
// Obs.: V2 = V1 / || V1 ||
const vetor3d vetor3d::normalize () const {

    vetor3d temp = vetor3d(*this);
    double norma = temp++;

    vetor3d ret = temp / norma;

    return ret;
}
