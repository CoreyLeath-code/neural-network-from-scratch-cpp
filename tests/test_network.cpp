#include <gtest/gtest.h>
#include <memory>
#include <vector>

// These paths are relative to the project root, which is now
// mapped as the include directory in your CMakeLists.txt.
#include "src/impl/NeuralNet.hpp"
#include "src/impl/DenseLayer.hpp"
#include "src/impl/Activations.hpp"
#include "src/impl/Losses.hpp"

// ── DenseLayer & Activation Unit Tests ────────────────────────────────────────

TEST(DenseLayerTest, ForwardOutputSize) {
    DenseLayer layer(3, 2, 42);
    std::vector<double> input = {1.0, 2.0, 3.0};
    auto out = layer.forward(input);
    ASSERT_EQ(out.size(), 2u);
}

TEST(ActivationTest, ReLUFunctionality) {
    ReLU relu;
    // Test positive input
    EXPECT_DOUBLE_EQ(relu.activate(2.0), 2.0);
    // Test negative input
    EXPECT_DOUBLE_EQ(relu.activate(-3.0), 0.0);
}

// ── NeuralNet Integration & Convergence Tests ─────────────────────────────────

TEST(NeuralNetTest, ConvergenceTest) {
    // Initialize Neural Network with Mean Squared Error loss
    NeuralNet nn(std::make_unique<MSE>());
    
    // Add layers: 2 inputs -> 4 hidden nodes -> 1 output
    nn.addLayer(std::make_unique<DenseLayer>(2, 4, 42));
    nn.addLayer(std::make_unique<DenseLayer>(4, 1, 43));

    // XOR Problem data
    std::vector<std::vector<double>> X = {{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};
    std::vector<std::vector<double>> Y = {{0.0}, {1.0}, {1.0}, {0.0}};

    // Training loop
    for (int epoch = 0; epoch < 1000; ++epoch) {
        for (size_t i = 0; i < X.size(); ++i) {
            nn.train(X[i], Y[i], 0.1);
        }
    }
    
    // Validation: Prediction for {0.0, 1.0} should be near 1.0
    auto pred = nn.forward({0.0, 1.0});
    EXPECT_NEAR(pred[0], 1.0, 0.2); 
}
