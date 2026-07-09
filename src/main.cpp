#include <iostream>
#include <memory>
#include "core/NeuralNet.hpp"
#include "impl/DenseLayer.hpp"
#include "impl/Activations.hpp"
#include "impl/Losses.hpp"

int main() {
    std::cout << "Neural Network from Scratch (C++) Demo\n";
    std::cout << "========================================\n\n";

    // Build a simple 2-4-1 network with MSE loss (seed=42 for reproducible demo)
    NeuralNet nn(std::make_unique<MSE>());
    nn.addLayer(std::make_unique<DenseLayer>(2, 4, 42));
    nn.addLayer(std::make_unique<DenseLayer>(4, 1, 43));

    // XOR-like dataset (linearly separable approximation)
    std::vector<std::vector<double>> X = {
        {0.1, 0.2}, {0.2, 0.3}, {0.8, 0.9}, {0.9, 0.8}
    };
    std::vector<std::vector<double>> Y = {{0.0}, {0.0}, {1.0}, {1.0}};

    const int epochs = 500;
    const double lr = 0.05;

    double loss = 0.0;
    for (int epoch = 0; epoch < epochs; ++epoch) {
        loss = 0.0;
        for (size_t i = 0; i < X.size(); ++i) {
            loss += nn.train(X[i], Y[i], lr);
        }
        if ((epoch + 1) % 100 == 0) {
            std::cout << "Epoch " << (epoch + 1) << " | Loss: "
                      << loss / static_cast<double>(X.size()) << "\n";
        }
    }

    std::cout << "\nPredictions after training:\n";
    for (size_t i = 0; i < X.size(); ++i) {
        auto pred = nn.forward(X[i]);
        std::cout << "  Input: [" << X[i][0] << ", " << X[i][1]
                  << "]  ->  Predicted: " << pred[0]
                  << "  (Expected: " << Y[i][0] << ")\n";
    }

    return 0;
}
