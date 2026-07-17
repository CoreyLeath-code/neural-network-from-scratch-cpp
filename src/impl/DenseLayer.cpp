#include "impl/DenseLayer.hpp"

#include <cmath>
#include <random>
#include <stdexcept>
#include <utility>

namespace nn {

DenseLayer::DenseLayer(std::size_t input_size, std::size_t output_size, ActivationPtr activation,
                       std::uint64_t seed)
    : input_size_(input_size),
      output_size_(output_size),
      activation_(std::move(activation)),
      weights_(input_size * output_size),
      biases_(output_size, 0.0) {
  if (input_size == 0 || output_size == 0 || !activation_) {
    throw std::invalid_argument("dense layer dimensions and activation must be valid");
  }
  std::mt19937_64 generator(seed);
  const double limit = std::sqrt(6.0 / static_cast<double>(input_size + output_size));
  std::uniform_real_distribution<double> distribution(-limit, limit);
  for (double& weight : weights_) weight = distribution(generator);
}

const Vector& DenseLayer::forward(const Vector& input) {
  if (input.size() != input_size_) throw std::invalid_argument("dense input size mismatch");
  last_input_ = input;
  last_output_.assign(output_size_, 0.0);
  for (std::size_t output = 0; output < output_size_; ++output) {
    double sum = biases_[output];
    for (std::size_t input_index = 0; input_index < input_size_; ++input_index) {
      if (!std::isfinite(input[input_index])) throw std::invalid_argument("dense input must be finite");
      sum += weights_[output * input_size_ + input_index] * input[input_index];
    }
    last_output_[output] = activation_->forward(sum);
  }
  has_forward_cache_ = true;
  return last_output_;
}

Vector DenseLayer::backward(const Vector& output_gradient, double learning_rate) {
  if (!has_forward_cache_) throw std::logic_error("forward must be called before backward");
  if (output_gradient.size() != output_size_) throw std::invalid_argument("gradient size mismatch");
  if (!std::isfinite(learning_rate) || learning_rate <= 0.0) {
    throw std::invalid_argument("learning rate must be finite and positive");
  }
  Vector input_gradient(input_size_, 0.0);
  Vector deltas(output_size_);
  for (std::size_t output = 0; output < output_size_; ++output) {
    if (!std::isfinite(output_gradient[output])) throw std::invalid_argument("gradient must be finite");
    deltas[output] = output_gradient[output] * activation_->derivative_from_output(last_output_[output]);
    for (std::size_t input = 0; input < input_size_; ++input) {
      input_gradient[input] += weights_[output * input_size_ + input] * deltas[output];
    }
  }
  for (std::size_t output = 0; output < output_size_; ++output) {
    for (std::size_t input = 0; input < input_size_; ++input) {
      weights_[output * input_size_ + input] -= learning_rate * deltas[output] * last_input_[input];
    }
    biases_[output] -= learning_rate * deltas[output];
  }
  return input_gradient;
}

std::size_t DenseLayer::parameter_count() const noexcept { return weights_.size() + biases_.size(); }

}  // namespace nn
