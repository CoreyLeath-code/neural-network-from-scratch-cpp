#pragma once

#include <cstddef>
#include <cstdint>

#include "core/Activation.hpp"
#include "core/Layer.hpp"

namespace nn {

class DenseLayer final : public Layer {
 public:
  DenseLayer(std::size_t input_size, std::size_t output_size, ActivationPtr activation,
             std::uint64_t seed = 42);
  [[nodiscard]] const Vector& forward(const Vector& input) override;
  [[nodiscard]] Vector backward(const Vector& output_gradient, double learning_rate) override;
  [[nodiscard]] std::size_t input_size() const noexcept override { return input_size_; }
  [[nodiscard]] std::size_t output_size() const noexcept override { return output_size_; }
  [[nodiscard]] std::size_t parameter_count() const noexcept override;
  [[nodiscard]] const Vector& weights() const noexcept { return weights_; }
  [[nodiscard]] const Vector& biases() const noexcept { return biases_; }

 private:
  std::size_t input_size_;
  std::size_t output_size_;
  ActivationPtr activation_;
  Vector weights_;
  Vector biases_;
  Vector last_input_;
  Vector last_output_;
  bool has_forward_cache_{false};
};

}  // namespace nn
