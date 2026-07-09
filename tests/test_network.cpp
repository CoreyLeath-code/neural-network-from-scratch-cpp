#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "core/NeuralNet.hpp"
#include "impl/Activations.hpp"
#include "impl/DenseLayer.hpp"
#include "impl/Losses.hpp"

TEST(DenseLayerTest, ForwardOutputSize) {
    DenseLayer layer(3, 2, 42);
    const auto out = layer.forward({1.0, 2.0, 3.0});

    ASSERT_EQ(out.size(), 2u);
}

TEST(ActivationTest, ReLUFunctionality) {
    ReLU relu;

    EXPECT_DOUBLE_EQ(relu.activate(2.0), 2.0);
    EXPECT_DOUBLE_EQ(relu.activate(-3.0), 0.0);
    EXPECT_DOUBLE_EQ(relu.derivative(0.0), 1.0);
}

TEST(NeuralNetTest, LearnsLinearMapping) {
    NeuralNet nn(std::make_unique<MSE>());
    nn.addLayer(std::make_unique<DenseLayer>(1, 1, 42));

    const std::vector<std::vector<double>> inputs = {
        {-1.0}, {0.0}, {1.0}, {2.0}
    };
    const std::vector<std::vector<double>> targets = {
        {-1.0}, {1.0}, {3.0}, {5.0}
    };

    double final_loss = 0.0;
    for (int epoch = 0; epoch < 1500; ++epoch) {
        final_loss = 0.0;
        for (std::size_t i = 0; i < inputs.size(); ++i) {
            final_loss += nn.train(inputs[i], targets[i], 0.02);
        }
    }

    const auto prediction = nn.forward({1.5});
    ASSERT_EQ(prediction.size(), 1u);
    EXPECT_LT(final_loss, 1e-6);
    EXPECT_NEAR(prediction[0], 4.0, 1e-3);
}
