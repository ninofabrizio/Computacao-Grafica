/* 
 * File:   main.cpp
 * Author: Nino
 *
 * Created on September 4, 2015, 10:27 AM
 */

#include <cstdlib>
#include<cstdio>
#include "vetor3d.h"
using namespace std;

/*
 * 
 */
int main( int argc, char** argv )
{
    vetor3d v1 = vetor3d(1.0, 2.0, 3.0);
    vetor3d v2 = vetor3d(4.0, 5.0, 6.0);
    
    vetor3d v3 = v1+v2;
    
    printf("( %f , %f , %f )\n", v3.getX(), v3.getY(), v3.getZ());
    
    return 0;
}
