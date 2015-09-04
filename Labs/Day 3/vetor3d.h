/* 
 * File:   vetor3d.h
 * Author: Nino
 *
 * Created on September 4, 2015, 10:28 AM
 */

#ifndef VETOR3D_H
#define	VETOR3D_H

class vetor3d {
public:
    vetor3d();
    
    vetor3d(const vetor3d& orig);
    
 /**
 * 
 * @param x coordenada x do vetor
 * @param y coordenada y do vetor
 * @param z coordenada z do vetor
 * @return 
 */
    vetor3d(double x, double y, double z);
    
    double getX() const;
    
    double getY() const;
    
    double getZ() const;
    
     /**
     * Operador da soma
     * 
     * @param v vetor em se faz a operação
     * @return  vetor resultado da operação
     */
    const vetor3d operator+ ( const vetor3d &v ) const;
    
     /**
     * Operador da soma
     * 
     * @param v vetor em se faz a operação
     * @return  vetor resultado da operação
     */
    const vetor3d operator- ( const vetor3d &v ) const;
    
    /**
     * Operador do produto escalar
     * 
     * @param v vetor em que se faz a operação
     * @return  escalar resultado da operação
     */
    const double operator* ( const vetor3d &v ) const;
    
    /**
     * Operador do produto vetorial
     * 
     * @param v vetor em se faz a operação
     * @return  vetor resultado da operação
     */
    //const vetor3d operator* ( const vetor3d &v ) const;
    
    /**
     * Operador da divisão por escalar
     * 
     * @param esc esc com que se faz a operação
     * @return  vetor resultado da operação
     */
    //const vetor3d operator/ ( const vetor3d &v ) const;
    
        /**
     * Operador da norma
     * 
     * @param esc esc com que se faz a operação
     * @return  vetor resultado da operação
     */
    //const vetor3d operator% ( const vetor3d &v ) const;

private:

    double x, y, z;

};

#endif	/* VETOR3D_H */

