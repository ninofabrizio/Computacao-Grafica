/* 
 * File:   vetor3d.h
 * Author: Nino
 *
 * Created on September 4, 2015, 10:28 AM
 *
 * Arquivo que possui a definição da classe vetor3d com seus
 * atributos, construtores e métodos.
 */

#ifndef VETOR3D_H
#define	VETOR3D_H

class vetor3d {

	public:

	/**
	* Construtor default da classe
	*
	* @return Nova instância da classe vetor3d
	*/
    vetor3d();
    
	/**
	* Construtor para copiar atributos de uma instância a uma nova
	*
	* @param &orig Referência à instância a ser copiada
	* @return      Nova instância da classe vetor3d
	*/
    vetor3d(const vetor3d &orig);

	/**
	* Construtor principal a ser usado na execução
	*
	* @param x Coordenada x do vetor
	* @param y Coordenada y do vetor
	* @param z Coordenada z do vetor
	* @return  Nova instância da classe vetor3d
	*/
    vetor3d(const double x, const double y, const double z);
    
	/**
	* Método que retorna atributo x
	*
	* @return Atributo x
	*/
    double getX() const;
    
	/**
	* Método que retorna atributo y
	*
	* @return Atributo y
	*/
    double getY() const;
    
	/**
	* Método que retorna atributo z
	*
	* @return Atributo z
	*/
    double getZ() const;
    
    /**
    * Operador da soma
    * 
    * @param &v Segundo vetor da operação
    * @return   Vetor resultado da operação
	*/
    const vetor3d operator+ (const vetor3d &v) const;
    
     /**
     * Operador da subtração
     * 
     * @param &v Segundo vetor da operação
     * @return   Vetor resultado da operação
     */
    const vetor3d operator- (const vetor3d &v) const;
    
    /**
    * Operador da multiplicação por escalar
    * 
    * @param &e Escalar da operação
    * @return   Vetor resultado da operação
    */
    const vetor3d operator* (const double &e) const;
    
    /**
    * Operador da divisão por escalar
    * 
    * @param &e Escalar da operação
    * @return   Vetor resultado da operação
    */
    const vetor3d operator/ (const double &e) const;

    /**
    * Operador do produto escalar
    * 
    * @param &v Segundo vetor da operação
    * @return   Escalar resultado da operação
    */
    const double operator* (const vetor3d &v) const;
    
    /**
    * Operador do produto vetorial
    * 
    * @param &v Segundo vetor da operação
    * @return   Vetor resultado da operação
    */
    const vetor3d operator& (const vetor3d &v) const;
    
    /**
    * Operador da norma do vetor
	*
    * @param  Serve para sobrecarregar o operador
	*		  em V++ (postfix ++). Para sobrecarregar o operador
	*		  em ++V (prefix ++), o parâmetro precisa ser nulo.
    * @return Escalar resultado da operação
    */
    const double operator++ (const int) const;

    /**
    * Operador da área do paralelogramo
    * 
    * @param &v Segundo vetor da operação
    * @return   Escalar resultado da operação
    */
    const double operator/ (const vetor3d &v) const;

private:

    double x, y, z;

};

#endif	/* VETOR3D_H */
