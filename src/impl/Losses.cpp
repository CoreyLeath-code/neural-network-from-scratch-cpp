#include "impl/Losses.hpp"

#include <cmath>
#include <stdexcept>

namespace nn {
namespace {
void validate(const Vector& prediction, const Vector& target) {
  if (prediction.empty() || prediction.size() != target.size()) {
    throw std::invalid_argument("prediction and target must have the same non-zero size");
  }
  for (std::size_t index = 0; index < prediction.size(); ++index) {
    if (!std::isfinite(prediction[index]) || !std::isfinite(target[index])) {
      throw std::invalid_argument("loss inputs must be finite");
    }
  }
}
}  // namespace

double MeanSquaredError::value(const Vector& prediction, const Vector& target) const {
  validate(prediction, target);
  double total = 0.0;
  for (std::size_t index = 0; index < prediction.size(); ++index) {
    const double difference = prediction[index] - target[index];
    total += difference * difference;
  }
  return total / static_cast<double>(prediction.size());
}

Vector MeanSquaredError::gradient(const Vector& prediction, const Vector& target) const {
  validate(prediction, target);
  Vector result(prediction.size());
  const double scale = 2.0 / static_cast<double>(prediction.size());
  for (std::size_t index = 0; index < prediction.size(); ++index) {
    result[index] = scale * (prediction[index] - target[index]);
  }
  return result;
}

}  // namespace nn
