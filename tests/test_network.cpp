#include <cmath>
#include <filesystem>
#include <fstream>
#include <limits>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "core/NeuralNet.hpp"
#include "impl/Activations.hpp"
#include "impl/CSVLoader.hpp"
#include "impl/DenseLayer.hpp"
#include "impl/Losses.hpp"

namespace {

TEST(Activations, AreNumericallyStableAndDifferentiable) {
  nn::Sigmoid sigmoid;
  nn::Relu relu;
  nn::Tanh hyperbolic_tangent;
  EXPECT_NEAR(sigmoid.forward(0.0), 0.5, 1e-12);
  EXPECT_DOUBLE_EQ(sigmoid.forward(1'000.0), 1.0);
  EXPECT_DOUBLE_EQ(sigmoid.forward(-1'000.0), 0.0);
  EXPECT_NEAR(sigmoid.derivative_from_output(0.5), 0.25, 1e-12);
  EXPECT_DOUBLE_EQ(relu.forward(-1.0), 0.0);
  EXPECT_DOUBLE_EQ(relu.forward(2.0), 2.0);
  EXPECT_DOUBLE_EQ(relu.derivative_from_output(0.0), 0.0);
  EXPECT_DOUBLE_EQ(relu.derivative_from_output(2.0), 1.0);
  EXPECT_NEAR(hyperbolic_tangent.forward(0.5), std::tanh(0.5), 1e-12);
  EXPECT_NEAR(hyperbolic_tangent.derivative_from_output(0.5), 0.75, 1e-12);
}

TEST(Loss, ComputesMseAndGradient) {
  nn::MeanSquaredError mse;
  EXPECT_DOUBLE_EQ(mse.value({1.0, 3.0}, {0.0, 1.0}), 2.5);
  EXPECT_EQ(mse.gradient({1.0, 3.0}, {0.0, 1.0}), (nn::Vector{1.0, 2.0}));
  EXPECT_THROW(mse.value({}, {}), std::invalid_argument);
  EXPECT_THROW(mse.gradient({1.0}, {1.0, 2.0}), std::invalid_argument);
  EXPECT_THROW(mse.value({std::numeric_limits<double>::infinity()}, {0.0}),
               std::invalid_argument);
}

TEST(DenseLayer, IsDeterministicAndValidatesBoundaries) {
  auto activation = std::make_shared<nn::Sigmoid>();
  nn::DenseLayer first(2, 3, activation, 123);
  nn::DenseLayer second(2, 3, activation, 123);
  EXPECT_EQ(first.weights(), second.weights());
  EXPECT_EQ(first.parameter_count(), 9U);
  EXPECT_EQ(first.forward({0.2, 0.7}), second.forward({0.2, 0.7}));
  EXPECT_THROW(first.forward({0.2}), std::invalid_argument);
  EXPECT_THROW(first.forward({std::numeric_limits<double>::quiet_NaN(), 0.0}),
               std::invalid_argument);
  EXPECT_THROW(nn::DenseLayer(0, 1, activation), std::invalid_argument);
}

TEST(DenseLayer, BackwardRequiresCacheAndUpdatesWeights) {
  auto activation = std::make_shared<nn::Sigmoid>();
  nn::DenseLayer layer(2, 1, activation, 4);
  EXPECT_THROW(layer.backward({1.0}, 0.1), std::logic_error);
  static_cast<void>(layer.forward({1.0, 0.5}));
  const auto before = layer.weights();
  const auto input_gradient = layer.backward({1.0}, 0.1);
  EXPECT_EQ(input_gradient.size(), 2U);
  EXPECT_NE(before, layer.weights());
  EXPECT_THROW(layer.backward({}, 0.1), std::invalid_argument);
  EXPECT_THROW(layer.backward({1.0}, 0.0), std::invalid_argument);
}

TEST(NeuralNet, ValidatesTopologyAndLifecycle) {
  EXPECT_THROW(nn::NeuralNet(nullptr), std::invalid_argument);
  nn::NeuralNet network(std::make_unique<nn::MeanSquaredError>());
  EXPECT_THROW(network.predict({1.0}), std::logic_error);
  network.add_dense(2, 3, std::make_shared<nn::Tanh>(), 1);
  EXPECT_THROW(network.add_dense(2, 1, std::make_shared<nn::Sigmoid>(), 2),
               std::invalid_argument);
  network.add_dense(3, 1, std::make_shared<nn::Sigmoid>(), 2);
  EXPECT_EQ(network.layer_count(), 2U);
  EXPECT_EQ(network.parameter_count(), 13U);
  EXPECT_THROW(network.evaluate({}, {}), std::invalid_argument);
  EXPECT_THROW(network.evaluate({{0.0, 0.0}}, {}), std::invalid_argument);
}

TEST(NeuralNet, LearnsXorRegression) {
  const std::vector<nn::Vector> inputs{{0, 0}, {0, 1}, {1, 0}, {1, 1}};
  const std::vector<nn::Vector> targets{{0}, {1}, {1}, {0}};
  nn::NeuralNet network(std::make_unique<nn::MeanSquaredError>());
  network.add_dense(2, 4, std::make_shared<nn::Tanh>(), 7);
  network.add_dense(4, 1, std::make_shared<nn::Sigmoid>(), 11);
  const double initial_loss = network.evaluate(inputs, targets);
  for (std::size_t epoch = 0; epoch < 8'000; ++epoch) {
    for (std::size_t index = 0; index < inputs.size(); ++index) {
      static_cast<void>(network.train_sample(inputs[index], targets[index], 0.25));
    }
  }
  EXPECT_LT(network.evaluate(inputs, targets), 0.02);
  EXPECT_LT(network.evaluate(inputs, targets), initial_loss / 5.0);
  EXPECT_LT(network.predict(inputs[0])[0], 0.2);
  EXPECT_GT(network.predict(inputs[1])[0], 0.8);
  EXPECT_GT(network.predict(inputs[2])[0], 0.8);
  EXPECT_LT(network.predict(inputs[3])[0], 0.2);
}

TEST(CsvLoader, LoadsStrictNumericDataset) {
  const auto path = std::filesystem::temp_directory_path() / "nn_valid.csv";
  { std::ofstream stream(path); stream << "0,1,1\n1,0,1\n"; }
  const auto dataset = nn::load_csv(path, 2, 1);
  EXPECT_EQ(dataset.features.size(), 2U);
  EXPECT_EQ(dataset.targets[0], (nn::Vector{1.0}));
  std::filesystem::remove(path);
}

TEST(CsvLoader, RejectsMalformedAndOversizedInput) {
  const auto path = std::filesystem::temp_directory_path() / "nn_invalid.csv";
  { std::ofstream stream(path); stream << "0,not-a-number,1\n"; }
  EXPECT_THROW(nn::load_csv(path, 2, 1), std::runtime_error);
  { std::ofstream stream(path); stream << "0,1\n"; }
  EXPECT_THROW(nn::load_csv(path, 2, 1), std::runtime_error);
  { std::ofstream stream(path); stream << "0,1,1\n1,0,1\n"; }
  EXPECT_THROW(nn::load_csv(path, 2, 1, 1), std::runtime_error);
  std::filesystem::remove(path);
  EXPECT_THROW(nn::load_csv(path, 2, 1), std::runtime_error);
  EXPECT_THROW(nn::load_csv(path, 0, 1), std::invalid_argument);
}

}  // namespace
