/* 
 * File:   vetor3d.cpp
 * Author: Nino
 * 
 * Created on September 4, 2015, 10:28 AM
 *
 * Arquivo que contém todas as implementações dos construtores e
 * métodos da classe vetor3d definida em vetor3d.h.
 */

#include "vetor3d.h"

vetor3d::vetor3d( ){ }

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

const vetor3d vetor3d::operator+ (const vetor3d &v) const {
    
    vetor3d ret;
    
	// Operadores podem acessar atributos private...
    ret.x = this->x + v.x;
    ret.y = this->y + v.y;
    ret.z = this->z + v.z;
    
    return ret;
}

const vetor3d vetor3d::operator- (const vetor3d &v) const {
    
    vetor3d ret;
    
    ret.x = this->x - v.x;
    ret.y = this->y - v.y;
    ret.z = this->z - v.z;
    
    return ret;
}

const vetor3d vetor3d::operator* (const double &e) const {
    
	vetor3d ret;

    ret.x = this->x * e;
    ret.y = this->y * e;
    ret.z = this->z * e;
    
    return ret;
}

const double vetor3d::operator* (const vetor3d &v) const {
    
    double xNovo = this->x * v.x;
    double yNovo = this->y * v.y;
    double zNovo = this->z * v.z;
    
    return (xNovo + yNovo + zNovo);
}

// Produto vetorial
/*const vetor3d vetor3d::operator* ( const vetor3d &v ) const {
    
    double esc = this->x * v.x;
    esc = esc + (this->y * v.y);
    esc = esc + (this->z * v.z);
    
    return esc;
}*/
