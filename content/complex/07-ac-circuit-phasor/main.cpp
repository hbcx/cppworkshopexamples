#include <complex>
#include <iostream>
#include <iomanip>

// A series RLC circuit driven by a sinusoidal source. In AC analysis each
// component becomes a complex impedance, and Ohm's law I = V / Z then works
// with complex numbers -- one value carries both the amplitude ratio and the
// phase shift, which is exactly why engineers reach for complex here.
int main() {
    std::cout << std::fixed << std::setprecision(3);
    using cd = std::complex<double>;

    const double R = 50.0;       // resistance, ohms
    const double L = 0.1;        // inductance, henries
    const double C = 1e-4;       // capacitance, farads
    const double omega = 377.0;  // angular frequency, rad/s (~60 Hz)

    // Z = R + j*(omega*L - 1/(omega*C)): resistor real, inductor and capacitor
    // reactances imaginary and opposite in sign.
    cd Z(R, omega * L - 1.0 / (omega * C));
    std::cout << "impedance Z = " << Z << " ohms\n";
    std::cout << "|Z|         = " << std::abs(Z) << " ohms\n";
    std::cout << "phase(Z)    = " << std::arg(Z) << " rad\n";

    // Drive with a 120 V phasor at angle 0; the current phasor is V / Z.
    cd V(120.0, 0.0);
    cd I = V / Z;
    std::cout << "\ncurrent |I|   = " << std::abs(I) << " A\n";
    std::cout << "current phase = " << std::arg(I) << " rad  (negative: current lags voltage)\n";
    return 0;
}
