#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "core/Activation.hpp"
#include "core/Layer.hpp"
#include "core/Loss.hpp"

namespace nn {

class NeuralNet final {
 public:
  explicit NeuralNet(std::unique_ptr<Loss> loss);
  void add_dense(std::size_t input_size, std::size_t output_size, ActivationPtr activation,
                 std::uint64_t seed = 42);
  [[nodiscard]] Vector predict(const Vector& input);
  [[nodiscard]] double train_sample(const Vector& input, const Vector& target,
                                    double learning_rate);
  [[nodiscard]] double evaluate(const std::vector<Vector>& inputs,
                                const std::vector<Vector>& targets);
  [[nodiscard]] std::size_t parameter_count() const noexcept;
  [[nodiscard]] std::size_t layer_count() const noexcept { return layers_.size(); }

 private:
  std::vector<std::unique_ptr<Layer>> layers_;
  std::unique_ptr<Loss> loss_;
};

}  // namespace nn
