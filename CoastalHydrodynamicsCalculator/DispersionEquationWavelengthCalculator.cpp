#include "DispersionEquationWavelengthCalculator.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

/// <summary>
/// Calculate the next iteration of dispersion equation wavelength.
/// </summary>
/// <param name="CycleTime">Wave period.</param>
/// <param name="Depth">Wave depth.</param>
/// <param name="LastWaveLength">The wavelength calculated in the last iteration.</param>
/// <returns>WaveLength</returns>
double DispersionEquationWavelengthCalcForward(const double& Period, const double& Depth, const double& LastWaveLength) noexcept
{
	return ((9.8 * pow(Period, 2)) / (2 * M_PI)) * tanh((2 * M_PI / LastWaveLength) * Depth);
}
