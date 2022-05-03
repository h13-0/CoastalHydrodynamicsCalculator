#pragma once

typedef enum
{
	EllipticIntegralCalcOK,
} EllipticIntegralCalcError_t;

/// <summary>
/// Calculate the first and second kinds of complete elliptic integrals.
/// </summary>
/// <param name="Kappa">Elliptic modulus.</param>
/// <param name="K">Complete elliptic integral of the first kind.</param>
/// <param name="E">Complete elliptic integral of the second kind.</param>
/// <returns></returns>
EllipticIntegralCalcError_t EllipticIntegralCalc(const double& Kappa, double& K, double& E, const double& Accuracy = 0.001) noexcept;
