#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

#include "core/NeuralNet.hpp"
#include "impl/DenseLayer.hpp"
#include "impl/Losses.hpp"

int main() {
    NeuralNet nn(std::make_unique<MSE>());
    nn.addLayer(std::make_unique<DenseLayer>(4, 8, 42));
    nn.addLayer(std::make_unique<DenseLayer>(8, 2, 43));

    const std::vector<double> input{0.2, 0.4, 0.6, 0.8};
    constexpr int iterations = 100000;

    const auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        const auto output = nn.forward(input);
        if (output.empty()) {
            return 1;
        }
    }
    const auto end = std::chrono::high_resolution_clock::now();

    const auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    const double avg_ns = static_cast<double>(elapsed_us) * 1000.0 / static_cast<double>(iterations);

    std::cout << "forward_iterations=" << iterations << '\n';
    std::cout << "total_us=" << elapsed_us << '\n';
    std::cout << "avg_forward_ns=" << avg_ns << '\n';

    return 0;
}
