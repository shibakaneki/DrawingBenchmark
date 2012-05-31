/*
 * SCubicPolynomial.cpp
 *
 *  Created on: May 31, 2012
 *      Author: kindov
 */

#include "SCubicPolynomial.h"

SCubicPolynomial::SCubicPolynomial(){

}

SCubicPolynomial::SCubicPolynomial(float a, float b, float c, float d){
	mA = a;
	mB = b;
	mC = c;
	mD = d;
}

SCubicPolynomial::~SCubicPolynomial(){

}

float SCubicPolynomial::eval(float u){
	return (((mD*u) + mC)*u + mB)*u + mA;
 }
