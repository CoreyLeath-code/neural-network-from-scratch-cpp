#include "core/NeuralNet.hpp"

#include <cmath>
#include <numeric>
#include <stdexcept>
#include <utility>

#include "impl/DenseLayer.hpp"

namespace nn {

NeuralNet::NeuralNet(std::unique_ptr<Loss> loss) : loss_(std::move(loss)) {
  if (!loss_) throw std::invalid_argument("loss must not be null");
}

void NeuralNet::add_dense(std::size_t input_size, std::size_t output_size,
                          ActivationPtr activation, std::uint64_t seed) {
  if (!layers_.empty() && layers_.back()->output_size() != input_size) {
    throw std::invalid_argument("layer dimensions do not connect");
  }
  layers_.push_back(std::make_unique<DenseLayer>(input_size, output_size,
                                                 std::move(activation), seed));
}

Vector NeuralNet::predict(const Vector& input) {
  if (layers_.empty()) throw std::logic_error("network has no layers");
  Vector output = input;
  for (auto& layer : layers_) output = layer->forward(output);
  return output;
}

double NeuralNet::train_sample(const Vector& input, const Vector& target,
                               double learning_rate) {
  Vector prediction = predict(input);
  const double sample_loss = loss_->value(prediction, target);
  Vector gradient = loss_->gradient(prediction, target);
  for (auto layer = layers_.rbegin(); layer != layers_.rend(); ++layer) {
    gradient = (*layer)->backward(gradient, learning_rate);
  }
  return sample_loss;
}

double NeuralNet::evaluate(const std::vector<Vector>& inputs,
                           const std::vector<Vector>& targets) {
  if (inputs.empty() || inputs.size() != targets.size()) {
    throw std::invalid_argument("evaluation sets must have the same non-zero size");
  }
  double total = 0.0;
  for (std::size_t index = 0; index < inputs.size(); ++index) {
    total += loss_->value(predict(inputs[index]), targets[index]);
  }
  return total / static_cast<double>(inputs.size());
}

std::size_t NeuralNet::parameter_count() const noexcept {
  return std::accumulate(layers_.begin(), layers_.end(), std::size_t{0},
                         [](std::size_t total, const auto& layer) {
                           return total + layer->parameter_count();
                         });
}

}  // namespace nn
