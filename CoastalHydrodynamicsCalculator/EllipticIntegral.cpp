#include "EllipticIntegral.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

/// <summary>
/// Calculate the first and second kinds of complete elliptic integrals.
/// </summary>
/// <param name="Kappa">Elliptic modulus.</param>
/// <param name="K">Complete elliptic integral of the first kind.</param>
/// <param name="E">Complete elliptic integral of the second kind.</param>
/// <returns></returns>
EllipticIntegralCalcError_t EllipticIntegralCalc(const double& Kappa, double& K, double& E, const double& Accuracy) noexcept
{
    /*
    // Calculate K(¦Ê).
    double K_kappa = 0.0;
    int slices = 0;
    for (double theta = Accuracy; theta < M_PI_2; theta += Accuracy)
    {
        K_kappa += 1 / (sqrt(1 - pow(Kappa, 2) * pow(sin(theta), 2)));
        slices++;
    }
    K_kappa = K_kappa * M_PI_2 / slices;
    K = K_kappa;

    // Calculate E(¦Ê).
    double E_kappa = 0.0;
    slices = 0;
    for (double theta = Accuracy; theta < M_PI_2; theta += Accuracy)
    {
        E_kappa += sqrt(1 - pow(Kappa, 2) * pow(sin(theta), 2));
        slices++;
    }
    E_kappa = E_kappa * M_PI_2 / slices;
    E = E_kappa;
    */

    // Copy from matlab.
    // Faster and more accurate.
    double m = pow(Kappa, 2);
    double a0 = 1;
    double a1 = 0.0;
    double b0 = sqrt(1 - m);
    double c0 = NAN;
    double s0 = m;
    double i1 = 0;
    double mm = INFINITY;
    while (fabs(mm) > Accuracy)
    {
        a1 = (a0 + b0) / 2.0;
        double b1 = sqrt(a0 * b0);
        double c1 = (a0 - b0) / 2.0;
        i1 += 1;
        double w1 = pow(2, i1) * pow(c1, 2);
        mm = w1;
        s0 += w1;
        a0 = a1;
        b0 = b1;
        c0 = c1;
    }

    K = M_PI / (2 * a1);
    E = K * (1 - s0 / 2.0);

	return EllipticIntegralCalcOK;
}
