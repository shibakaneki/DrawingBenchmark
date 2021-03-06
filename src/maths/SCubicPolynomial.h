/*
 * SCubicPolynomial.h
 *
 *  Created on: May 31, 2012
 *      Author: kindov
 */

#ifndef SCUBICPOLYNOMIAL_H_
#define SCUBICPOLYNOMIAL_H_

class SCubicPolynomial{
public:
	SCubicPolynomial();
	SCubicPolynomial(float a, float b, float c, float d);
	~SCubicPolynomial();
	float eval(float u);

private:
	float mA;
	float mB;
	float mC;
	float mD;
};

#endif /* SCUBICPOLYNOMIAL_H_ */
