#include <iostream>
#include <cmath>
#include <complex>
#include "ALGLIB/dataanalysis.h"

#define M_PI 3.14159265358979323846

double factorial(int n)
{
	double result = 1;
	for (int i = 1; i <= n; i++)
	{
		result *= i;
	}
	return result;
}

double LegendrePolynomial(int n, double x)
{
	return alglib::legendrecalculate(n, x);
}

std::complex<double> SphericalHarmonic(int l, int m, double theta, double phi)
{
	double numerator = (2 * l + 1) * factorial(l - std::abs(m));
	double denominator = 4 * M_PI * factorial(l + std::abs(m));

	double legendrePolynomial = LegendrePolynomial(l, std::cos(theta));

	std::complex<double> exponent = std::exp(std::complex<double>(0, m * phi));

	std::complex<double> result = std::sqrt(numerator / denominator) * std::pow(legendrePolynomial, std::abs(m)) * exponent;

	return result;
}

int main()
{
	int l = 1;
	int m = -1;
	double theta = M_PI / 2;
	double phi = M_PI / 2;
	std::cout << "Spherical Harmonic Y(" << l << ", " << m << ") = " << SphericalHarmonic(l, m, theta, phi) << std::endl;
	return 0;
}