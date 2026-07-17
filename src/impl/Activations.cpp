#include "impl/Activations.hpp"

#include <algorithm>
#include <cmath>

namespace nn {

double Sigmoid::forward(double value) const noexcept {
  if (value >= 0.0) {
    const double exponential = std::exp(-value);
    return 1.0 / (1.0 + exponential);
  }
  const double exponential = std::exp(value);
  return exponential / (1.0 + exponential);
}

double Sigmoid::derivative_from_output(double output) const noexcept {
  return output * (1.0 - output);
}

double Relu::forward(double value) const noexcept { return std::max(0.0, value); }
double Relu::derivative_from_output(double output) const noexcept { return output > 0.0 ? 1.0 : 0.0; }

double Tanh::forward(double value) const noexcept { return std::tanh(value); }
double Tanh::derivative_from_output(double output) const noexcept { return 1.0 - output * output; }

}  // namespace nn
