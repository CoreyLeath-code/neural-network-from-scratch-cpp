#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

#include "core/NeuralNet.hpp"
#include "impl/Activations.hpp"
#include "impl/Losses.hpp"

int main() {
  const std::vector<nn::Vector> inputs{{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};
  const std::vector<nn::Vector> targets{{0.0}, {1.0}, {1.0}, {0.0}};
  nn::NeuralNet network(std::make_unique<nn::MeanSquaredError>());
  network.add_dense(2, 4, std::make_shared<nn::Tanh>(), 7);
  network.add_dense(4, 1, std::make_shared<nn::Sigmoid>(), 11);
  for (std::size_t epoch = 0; epoch < 8'000; ++epoch) {
    for (std::size_t sample = 0; sample < inputs.size(); ++sample) {
      static_cast<void>(network.train_sample(inputs[sample], targets[sample], 0.25));
    }
  }
  std::cout << "XOR predictions (research demo)\n" << std::fixed << std::setprecision(4);
  for (const auto& input : inputs) {
    std::cout << input[0] << " xor " << input[1] << " -> " << network.predict(input)[0] << '\n';
  }
  return network.evaluate(inputs, targets) < 0.02 ? 0 : 1;
}
