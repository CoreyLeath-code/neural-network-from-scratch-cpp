#pragma once

#include <cstddef>
#include <vector>

namespace nn {

using Vector = std::vector<double>;

class Layer {
 public:
  virtual ~Layer() = default;
  [[nodiscard]] virtual const Vector& forward(const Vector& input) = 0;
  [[nodiscard]] virtual Vector backward(const Vector& output_gradient, double learning_rate) = 0;
  [[nodiscard]] virtual std::size_t input_size() const noexcept = 0;
  [[nodiscard]] virtual std::size_t output_size() const noexcept = 0;
  [[nodiscard]] virtual std::size_t parameter_count() const noexcept = 0;
};

}  // namespace nn
