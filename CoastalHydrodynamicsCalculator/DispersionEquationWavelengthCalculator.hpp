#pragma once

/// <summary>
/// Calculate the next iteration of dispersion equation wavelength.
/// </summary>
/// <param name="CycleTime">Wave period.</param>
/// <param name="Depth">Wave depth.</param>
/// <param name="LastWaveLength">The wavelength calculated in the last iteration.</param>
/// <returns>WaveLength</returns>
double DispersionEquationWavelengthCalcForward(const double& Period, const double& Depth, const double& LastWaveLength) noexcept;
