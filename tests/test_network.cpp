#include <gtest/gtest.h>
#include <memory>
#include <vector>

// Assuming your directory structure is: include/core/NeuralNet.hpp
#include "core/NeuralNet.hpp"
#include "core/DenseLayer.hpp"
#include "core/Activations.hpp"
#include "core/Losses.hpp"

// ── DenseLayer & Activation Unit Tests ────────────────────────────────────────

TEST(DenseLayerTest, ForwardOutputSize) {
    DenseLayer layer(3, 2, 42);
    auto out = layer.forward({1.0, 2.0, 3.0});
    ASSERT_EQ(out.size(), 2u);
}

TEST(ActivationTest, ReLUFunctionality) {
    ReLU relu;
    EXPECT_DOUBLE_EQ(relu.activate(2.0), 2.0);
    EXPECT_DOUBLE_EQ(relu.activate(-3.0), 0.0);
}

// ── NeuralNet Integration & Convergence Tests ─────────────────────────────────

TEST(NeuralNetTest, ConvergenceTest) {
    NeuralNet nn(std::make_unique<MSE>());
    nn.addLayer(std::make_unique<DenseLayer>(2, 4, 42));
    nn.addLayer(std::make_unique<DenseLayer>(4, 1, 43));

    std::vector<std::vector<double>> X = {{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};
    std::vector<std::vector<double>> Y = {{0.0}, {1.0}, {1.0}, {0.0}};

    for (int epoch = 0; epoch < 1000; ++epoch) {
        for (size_t i = 0; i < X.size(); ++i) {
            nn.train(X[i], Y[i], 0.1);
        }
    }
    
    auto pred = nn.forward({0.0, 1.0});
    EXPECT_NEAR(pred[0], 1.0, 0.2); 
}