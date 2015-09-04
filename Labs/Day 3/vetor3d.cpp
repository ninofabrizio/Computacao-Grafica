/* 
 * File:   vetor3d.cpp
 * Author: Nino
 * 
 * Created on September 4, 2015, 10:28 AM
 */

#include "vetor3d.h"

vetor3d::vetor3d( ){ }

vetor3d::vetor3d( const vetor3d& orig ) { }

double vetor3d::getX () const {
    
    return x;
}

double vetor3d::getY () const {
    
    return y;
}

double vetor3d::getZ () const {
    
    return z;
}

vetor3d::vetor3d (const double x, const double y, const double z) {
    
    this->x = x;
    this->y = y;
    this->z = z;
}

const vetor3d vetor3d::operator+ ( const vetor3d &v ) const {
    
    vetor3d ret;
    
    ret.x = this->x + v.x;
    ret.y = this->y + v.y;
    ret.z = this->z + v.z;
    
    return ret;
}

const vetor3d vetor3d::operator- ( const vetor3d &v ) const {
    
    vetor3d ret;
    
    ret.x = this->x - v.x;
    ret.y = this->y - v.y;
    ret.z = this->z - v.z;
    
    return ret;
}

// Produto escalar
const double vetor3d::operator* ( const vetor3d &v ) const {
    
    double esc = this->x * v.x;
    esc = esc + (this->y * v.y);
    esc = esc + (this->z * v.z);
    
    return esc;
}

// Produto vetorial
/*const vetor3d vetor3d::operator* ( const vetor3d &v ) const {
    
    double esc = this->x * v.x;
    esc = esc + (this->y * v.y);
    esc = esc + (this->z * v.z);
    
    return esc;
}*/